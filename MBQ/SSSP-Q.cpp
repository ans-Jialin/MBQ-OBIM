#include "MBQ.h"
#define N 100010
vector<pair<int,int>>G[N];
atomic<int>dis[N];
void solve(MBQ &a,fun x)
{
    int u=x.x,d=dis[u].load();
    if(d>=INF || x.p!=d)return;
    for(auto e:G[u])
    {
        int v=e.first,w=e.second;
        long long nd=1LL*d+w;
        if(nd>=INF)continue;
        int old=dis[v].load();
        while(nd<old)
        {
            if(dis[v].compare_exchange_weak(old,(int)nd))
            {
                a.push({v,(int)nd});
                break;
            }
        }
    }
}
int main()
{
    GAP=read();
    RANGE=read();
    SIZE=read();
    BATCH=read();
    static MBQ a;
    int n=read(),m=read();
    for(int i=1;i<=n;i++)dis[i].store(INF);
    for(int i=1;i<=m;i++)
    {
        int u=read(),v=read(),w=read();
        G[u].push_back({v,w});
    }
    int s=read(),t=read();
    dis[s].store(0);
    a.push({s,0});
    a.threadpush();
    auto start=chrono::steady_clock::now();
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
    auto end=chrono::steady_clock::now();
    if(dis[t].load()==INF)cout<<"INF"<<endl;
    else cout<<dis[t].load()<<endl;
    double tm=chrono::duration<double,milli>(end-start).count();
    cout<<"time = "<<tm<<" ms"<<endl;
    return 0;
}
