#pragma once
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<thread>
#include<ctime>
#include<atomic>
#include<vector>
#include<map>
#include<iomanip> 
#include<chrono>
#include<cmath>
#include<memory>
#include<shared_mutex>
#include<deque>
#include<mutex>
#include<climits>
using namespace std;
const int QNUM=10;
const int TNUM=4;
struct fun
{
    int x,p;
};
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
struct Queue
{
    private:
    vector<fun>q;
    mutex L;
    int head,tail,cnt,capacity;
    void enlarge()
    {
        vector<fun>tmp(capacity*2);
        for(int i=0;i<cnt;i++)tmp[i]=q[(head+i)%capacity];
        q.swap(tmp);
        capacity*=2;
        head=0;
        tail=cnt;
    }
    public:
    Queue()
    {
        head=tail=cnt=0;
        capacity=8;
        q.resize(capacity);
    }
    void push(fun x)
    {
        lock_guard<mutex>lock(L);
        if(cnt==capacity)enlarge();
        q[tail++]=x;
        if(tail>=capacity)tail=0;
        cnt++;
    }
    bool pop(fun &x)
    {
        lock_guard<mutex>lock(L);
        if(cnt==0)return 0;
        tail=(tail-1+capacity)%capacity;
        x=q[tail];
        cnt--;
        return 1;
    }
    bool steal(fun &x)
    {
        lock_guard<mutex>lock(L);
        if(cnt==0)return 0;
        x=q[head++];
        if(head>=capacity)head=0;
        cnt--;
        return 1;
    }
    bool empty()
    {
        lock_guard<mutex>lock(L);
        return cnt==0;
    }
};
struct bucket
{
    private:
    Queue q[TNUM];
    atomic<int>cnt{0};
    public:
    void push(fun x,int tid)
    {
        cnt.fetch_add(1,memory_order_relaxed);
        q[tid].push(x);
    }
    bool pop(fun &x,int tid)
    {
        if(q[tid].pop(x))
        {
            cnt.fetch_sub(1,memory_order_relaxed);
            return 1;
        }
        for(int i=1;i<TNUM;i++)
        {
            int o=(tid+i)%TNUM;
            if(q[o].steal(x))
            {
                cnt.fetch_sub(1,memory_order_relaxed);
                return 1;
            }
        }
        return 0;
    }
};
struct OBIM
{
    private:
    map<int,shared_ptr<bucket>>globalMap;
    shared_mutex L;
    atomic<long long>pending{0};

    struct ThreadData
    {
        shared_ptr<bucket>current;
        int curIndex=INT_MIN;
        int scanStart=INT_MIN;
    };
    ThreadData data[TNUM];
    atomic<int>newStart[TNUM];

    void notify(int p)
    {
        for(int i=0;i<TNUM;i++)
        {
            int old=newStart[i].load(memory_order_relaxed);
            while(p<old && !newStart[i].compare_exchange_weak(old,p,memory_order_release,memory_order_relaxed)){}
        }
    }
    bool slowPop(fun &x,int tid)
    {
        shared_lock<shared_mutex>lock(L);
        for(auto it=globalMap.lower_bound(data[tid].scanStart);it!=globalMap.end();it++)
        {
            if(it->second->pop(x,tid))
            {
                data[tid].current=it->second;
                data[tid].curIndex=it->first;
                data[tid].scanStart=it->first;
                return 1;
            }
        }
        return 0;
    }
    public:
    OBIM()
    {
        for(int i=0;i<TNUM;i++)newStart[i].store(INT_MAX,memory_order_relaxed);
    }
    void push(fun x,int tid)
    {
        pending.fetch_add(1,memory_order_relaxed);
        shared_ptr<bucket>b;
        {
            shared_lock<shared_mutex>lock(L);
            auto it=globalMap.find(x.p);
            if(it!=globalMap.end())b=it->second;
        }
        if(!b)
        {
            unique_lock<shared_mutex>lock(L);
            auto &t=globalMap[x.p];
            if(!t)t=make_shared<bucket>();
            b=t;
        }
        b->push(x,tid);
        notify(x.p);
    }
    bool pop(fun &x,int tid)
    {
        int p=newStart[tid].exchange(INT_MAX,memory_order_acq_rel);
        if(p<data[tid].scanStart)data[tid].scanStart=p;
        if(data[tid].current && data[tid].scanStart>=data[tid].curIndex)
            if(data[tid].current->pop(x,tid))return 1;
        return slowPop(x,tid);
    }
    void complete()
    {
        pending.fetch_sub(1,memory_order_release);
    }
    bool finished()
    {
        return pending.load(memory_order_acquire)==0;
    }
};
