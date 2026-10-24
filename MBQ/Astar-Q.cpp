#include "MBQ.h"
#define N 100010
vector<pair<int,int>>G[N];
atomic<int>dis[N];
int h[N];
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
    for(int i=1;i<=n;i++)h[i]=read();
    dis[s].store(0);
    fun st;
    st.x=s;
    st.p=h[s];
    a.push(st);
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
                if(res.empty()) break;
                fun x=res.take();
                int d=dis[x.x].load();
                if(x.p!=d+h[x.x])continue;
                for(auto e:G[x.x])
                {
                    int v=e.first;
                    int w=e.second;
                    int nd=d+w;
                    int old=dis[v].load();
                    while(nd<old)
                    {
                        if(dis[v].compare_exchange_weak(old,nd))
                        {
                            fun y;
                            y.x=v;
                            y.p=nd+h[v];
                            a.push(y);
                            break;
                        }
                    }
                }
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