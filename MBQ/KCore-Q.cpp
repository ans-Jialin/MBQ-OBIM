#include "MBQ.h"
#define N 100010
vector<int>G[N];
atomic<int>deg[N],removed[N];
int core[N],kk;
void solve(MBQ &a,fun x)
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
                a.push({v,kk});
        }
    }
}
int main()
{
    GAP=read();
    RANGE=read();
    SIZE=read();
    BATCH=read();
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
        MBQ a;
        int cnt=0;
        for(int i=1;i<=n;i++)if(!removed[i].load() && deg[i].load()<k)
        {
            removed[i].store(1);
            a.push({i,k});
            cnt++;
        }
        if(cnt)
        {
            a.threadpush();
            vector<thread>workers;
            for(int i=0;i<TNUM;i++)
            {
                workers.emplace_back([&]()
                {
                    a.initTID();
                    while(1)
                    {
                        bucket res=a.pop();
                        if(res.empty())break;
                        fun x=res.take();
                        solve(a,x);
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
