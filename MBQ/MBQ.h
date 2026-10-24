#pragma once
#include<iostream>
#include<cstdio>
#include<thread>
#include<vector>
#include<algorithm>
#include<ctime>
#include<chrono>
#include<iomanip> 
#include<functional>
#include<chrono>
#include<cmath>
#include<climits>
#include<cstdlib>
#include<mutex>
#include<atomic>
#include<random>
using namespace std;
#define INF 0x3f3f3f3f
const int QNUM=80;
const int TNUM=6;
const int STICKY=4;
int GAP,RANGE,SIZE,BATCH;
inline int read()
{
    int wjl=0,jia=1;
    char ch=getchar();
    while(ch<'0'||ch>'9')
    {
        if(ch=='-')jia*=-1;
        ch=getchar();
    }
    while(ch>='0'&&ch<='9')
    {
        wjl=(wjl<<1)+(wjl<<3)+(ch^48);
        ch=getchar();
    }
    return wjl*jia;
}
struct fun
{
    int x;
    int p;
};
struct Buff
{
    vector<fun>pushbuffer;
    vector<fun>popbuffer;
    int popcnt;
    Buff()
    {
        popcnt=0;
    }
};
struct bucket
{
    private:
    vector<fun>b;
    int head,tail,cnt,BNUM;
    public:
    bucket()
    {
        BNUM=8;
        b.resize(BNUM);
        head=0;tail=0;cnt=0;
    }
    void push(fun x)
    {
        if(cnt==BNUM)
        {
            b.resize(BNUM*2);
            head=0,tail=BNUM;
            b[tail++]=x;
            BNUM*=2;
            cnt++;
        }
        else
        {
            b[tail++]=x;
            if(tail>=BNUM)tail%=BNUM;
            cnt++;
        }
    }
    bucket pop(int k)
    {
        bucket res;
        int num=min(k,cnt);
        for(int i=1;i<=num;i++)
        {
            res.push(b[head]);
            head++;
            cnt--;
            if(head>=BNUM)head%=BNUM;
        }
        return res;
    }
    fun take()
    {
        fun ans=b[head];
        head++;
        cnt--;
        if(head>=BNUM)head%=BNUM;
        return ans;
    }
    bool empty()
    {
        return cnt==0;
    }
    int size()
    {
        return cnt;
    }
    void print()
    {
        int i=head;
        for(int j=1;j<=cnt;j++)
        {
            printf("%d ",b[i].x);
            i++;
            if(i>=BNUM)i%=BNUM;
        }
        printf("\n");
    }
};
struct Queue
{
    private:
    vector<bucket>B;
    std::mutex L;
    atomic<int>topBKT;
    atomic<bool>emp;
    int minBKT,Range,gap,size,batchnum;
    public:
    Queue()
    { 
        batchnum=BATCH,gap=GAP,size=SIZE,Range=RANGE;
        B.resize(size+2);
        minBKT=SIZE+2;
        topBKT.store(INF);
        emp.store(1);
    }
    bool try_lock()
    {
        return L.try_lock();
    }
    void unlock()
    {
        L.unlock();
    }
    bool empty()
    {
        return emp.load();
    }
    int getBucket(int p)
    {
        if(p<Range)return 0;//左臂右开
        if(p>=Range+gap*size)return size+1;
        return (p-Range)/gap+1;
    }
    void batchpush(vector<fun> &v)
    {
        for(fun x:v)
        {
            int o=getBucket(x.p);
            B[o].push(x);
            minBKT=min(minBKT,o);
        }
        update();
    }
    void push(fun x)
    {
        int o=getBucket(x.p);
        B[o].push(x);
        minBKT=min(minBKT,o);
        update();
    }
    void enlarge()
    {
        Range+=gap*size;
        int num=B[size+1].size(),minP=INF;
        bool flag=0;
        vector<fun>tmp;
        for(int i=1;i<=num;i++)tmp.push_back(B[size+1].take());
        minBKT=size+2;
        for(fun x:tmp)
        {
            int o=getBucket(x.p);
            if(o==size+1)B[o].push(x),minP=min(minP,x.p);
            else
            {
                B[o].push(x);
                 minBKT=min(minBKT,o);
                  flag=1;
             } 
        }
        if(!flag&&!B[size+1].empty())
        {
            Range=minP-(minP-Range) %(gap*size)-gap*size;
            minBKT=size+1;
        }
    }
    bucket pop()
    {
        while(minBKT==size+1)enlarge();
        if(minBKT==size+2)
        {
            update();
            return bucket();
        }
        if(B[minBKT].empty())
        {
            update();
            return bucket();
        }
        if(B[minBKT].size()<=batchnum)
        {
            bucket res=B[minBKT].pop(batchnum);
            minBKT++;
            while(minBKT<=size+1)
            {
                if(!B[minBKT].empty())
                {
                    break;
                }
                minBKT++;
            }
            while(minBKT==size+1)enlarge();
            update();
            return res;
        }
        else
        {
            bucket res=B[minBKT].pop(batchnum);
            update();
            return res;
        }
    }
    int getminBKT()
    {
        return topBKT.load();
    }
    void update()
    {
        if(minBKT==0)
            topBKT.store(-1);
        else if(minBKT>=size+1)
            topBKT.store(INF);
        else
            topBKT.store(Range+(minBKT-1)*gap);
        emp.store(minBKT==size+2);
    }
};
struct MBQ
{
    private:
    Queue q[QNUM];
    vector<Buff>Buffs;
    inline static thread_local int tID=0;
    inline static thread_local int pQueue=0;
    inline static thread_local int pCnt=0;
    atomic<int>ID{0};
    atomic<int>threadnum{0};
    atomic<int>numEmpty{QNUM};
    public:
    MBQ()
    {
        Buffs.resize(TNUM);
    }
    void initTID()
    {
        tID=ID.fetch_add(1);
        pQueue=tID;
        pCnt=1;
    }
    void threadpush()
    {
        if(Buffs[tID].pushbuffer.size()==0)return ;
        if(pCnt<STICKY&&q[pQueue].try_lock())
        {
            if(q[pQueue].empty())numEmpty--;
            q[pQueue].batchpush(Buffs[tID].pushbuffer);
            Buffs[tID].pushbuffer.clear();
            q[pQueue].unlock();
            pCnt++;
            return ;
            
        }
        while(1)
        {
            int wjl=randomQueue();
            if(q[wjl].try_lock())
            {
                if(q[wjl].empty())numEmpty--;
                q[wjl].batchpush(Buffs[tID].pushbuffer);
                Buffs[tID].pushbuffer.clear();
                q[wjl].unlock();
                pQueue=wjl;
                pCnt=1;
                break;
            }
        }
    }
    int randomQueue()
    {
        thread_local mt19937 gen(chrono::steady_clock::now().time_since_epoch().count()^ hash<thread::id>{}(this_thread::get_id()));
        uniform_int_distribution<int> dis(0,QNUM-1);
        return dis(gen);
    }
    void push(fun x)
    {
        Buffs[tID].pushbuffer.push_back(x);
        if(Buffs[tID].pushbuffer.size()>=BATCH)
        {
            threadpush();
        }
    }
    bucket pop()
    {
        // cout<<tID<<" 1111"<<endl;
        bucket ans=threadpop();
        // cout<<tID<<" 1222"<<endl;
        if(!ans.empty())
        {
            return ans;
        }
        threadnum++;
        // cout<<threadnum<<"hhh"<<endl;
        while(1)
        {
            ans=threadpop();
            if(!ans.empty())
            {
                threadnum--;
                return ans;
            }
            if(threadnum==TNUM)
            {
                return bucket();
            }
        }
    }
    bucket threadpop()
    {
        if(Buffs[tID].popcnt<Buffs[tID].popbuffer.size())
        {
            Buffs[tID].popcnt++;
            bucket res;
            res.push(Buffs[tID].popbuffer[Buffs[tID].popcnt-1]);
            return res;
        }
        Buffs[tID].popbuffer.clear();
        Buffs[tID].popcnt=0;
        // cerr<<"T"<<tID<<" before threadpush"<<endl;
        threadpush();
        // cerr<<"T"<<tID<<" after threadpush"<<" numEmpty="<<numEmpty.load()<<" threadnum="<<threadnum.load()<<endl;
        if(empty())return bucket();
        if(pCnt<STICKY&&q[pQueue].try_lock())//no need for seperate pQueue?
        {
            bool flag=0;
            if(q[pQueue].empty())
            {
                // cout<<"hhh1"<<endl;
                q[pQueue].unlock();
            }
            else
            {
                // cout<<"jjj1"<<endl;
                bucket ans=q[pQueue].pop();
                // cout<<"jjj2"<<endl;
                if(q[pQueue].empty()&&!flag)numEmpty++;
                q[pQueue].unlock();
                // cout<<"jjj1"<<endl;
                while(!ans.empty())Buffs[tID].popbuffer.push_back(ans.take());
                // cout<<"jjj2"<<endl;
                pCnt++;
                
                if(!Buffs[tID].popbuffer.empty())
                {//cout<<"1jjj1"<<endl;
                    bucket res;
                    res.push(Buffs[tID].popbuffer[Buffs[tID].popcnt]);
                    Buffs[tID].popcnt++;
                    // cout<<"2jjj2"<<endl;
                    return res;
                }
                
            }
        }
        int wjl,jia,debugcnt=0;
        while(1)
        {
            // debugcnt++;
            // if(debugcnt==100)
            // {
            //     cout<<"numEmpty = "<<numEmpty.load()<<" threadnum = "<<threadnum.load()<<endl;
            //     debugcnt=0;
            // }
            wjl=randomQueue();jia=randomQueue();
            // cout<<tID<<" before same "<<wjl<<" "<<jia<<endl;
            while(jia==wjl)
            {
                jia=randomQueue();
            }
// cout<<tID<<" after same "<<wjl<<" "<<jia<<endl;
            int xx=q[wjl].getminBKT(),yy=q[jia].getminBKT();
            if(xx>yy)swap(wjl,jia);
            if(!q[wjl].empty()&&q[wjl].try_lock())
            {
                if(q[wjl].empty())
                {
                    q[wjl].unlock();
                    continue;
                }
                // cout<<tID<<" 11jjj1"<<endl;
                bucket ans=q[wjl].pop();
                // cout<<tID<<" 11jjj12"<<endl;
                if(q[wjl].empty())numEmpty++;
                q[wjl].unlock();
                pQueue=wjl;
                pCnt=1;
                
                while(!ans.empty())Buffs[tID].popbuffer.push_back(ans.take());
                
                if(!Buffs[tID].popbuffer.empty())
                {
                    bucket res;
                    res.push(Buffs[tID].popbuffer[Buffs[tID].popcnt]);
                    Buffs[tID].popcnt++;
                    return res;
                }
            }
            if(!q[jia].empty()&&q[jia].try_lock())
            {
                if(q[jia].empty())
                {
                    q[jia].unlock();
                    continue;
                }
                // cout<<tID<<" 2jjj11"<<endl;
                bucket ans=q[jia].pop();
                // cout<<tID<<" 2jjj12"<<endl;
                if(q[jia].empty())numEmpty++;
                q[jia].unlock();
                pQueue=jia;
                pCnt=1;
                
                while(!ans.empty())Buffs[tID].popbuffer.push_back(ans.take());
                
                if(!Buffs[tID].popbuffer.empty())
                {
                    bucket res;
                    res.push(Buffs[tID].popbuffer[Buffs[tID].popcnt]);
                    Buffs[tID].popcnt++;
                    return res;
                }
            }

            if(empty())
            {
                return bucket();
            }
        }
    }
    bool empty()
    {
        return numEmpty>=QNUM;
    }
};
