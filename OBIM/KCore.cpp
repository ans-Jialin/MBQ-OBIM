#include "OBIM.h"
#define INF 0x3f3f3f3f
inline int read()
{
    int x=0,f=1;
    char ch=getchar();
    while(ch!='-' && (ch<'0'||ch>'9'))ch=getchar();
    if(ch=='-')f=-1,ch=getchar();
    while(ch>='0'&&ch<='9')
    {
        x=x*10+ch-'0';
        ch=getchar();
    }
    return x*f;
}
#define N 100010
vector<int>G[N];
atomic<int>deg[N],removed[N];
int core[N],kk;
void solve(OBIM &a,fun x,int tid)
{
    int u=x.x;
    core[u]=kk-1;
    for(int v:G[u])if(!removed[v].load())
    {
        int d=deg[v].fetch_sub(1);
        if(d==kk)
        {
            int old=0;
            if(removed[v].compare_exchange_strong(old,1))
                a.push({v,kk},tid);
        }
    }
}
int main()
{
    read();read();read();read();
    int n=read(),m=read();
    for(int i=1;i<=m;i++)
    {
        int u=read(),v=read(),w=read();
        if(u==v)continue;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    for(int i=1;i<=n;i++)
    {
        sort(G[i].begin(),G[i].end());
        G[i].erase(unique(G[i].begin(),G[i].end()),G[i].end());
        deg[i].store(G[i].size());
        removed[i].store(0);
    }
    auto start=chrono::steady_clock::now();
    int left=n,k=0;
    while(left)
    {
        kk=++k;
        OBIM a;
        int cnt=0;
        for(int i=1;i<=n;i++)if(!removed[i].load() && deg[i].load()<k)
        {
            removed[i].store(1);
            a.push({i,k},0);
            cnt++;
        }
        if(cnt)
        {
        
            vector<thread>workers;
        
                        for(int i=0;i<TNUM;i++)
        
                        {
        
                            workers.emplace_back([&,i]()
        
                            {
        
                                while(1)
        
                                {
        
                                    fun x;
        
                                    if(a.pop(x,i))
        
                                    {
        
                                        solve(a,x,i);
        
                                        a.complete();
        
                                    }
        
                                    else if(a.finished())break;
        
                                    else this_thread::yield();
        
                                }
        
                            });
        
                        }
        
                        for(auto &th:workers)th.join();
        }
        left=0;
        for(int i=1;i<=n;i++)left+=!removed[i].load();
    }
    auto end=chrono::steady_clock::now();
    for(int i=1;i<=n;i++)cout<<core[i]<<" ";
    cout<<endl;
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
