#include "MBQ.h"
#define N 100010
vector<int>G[N];
atomic<int>dis[N];
int main()
{
    GAP=read();
    RANGE=read();
    SIZE=read();
    BATCH=read();
    static MBQ a;
    int n=read();
    int m=read();
    for(int i=1;i<=n;i++)
        dis[i].store(INF);
    for(int i=1;i<=m;i++)
    {
        int u=read();
        int v=read();
        G[u].push_back(v);
        G[v].push_back(u);
    }
    dis[1].store(0);
    fun s;
    s.x=1;
    s.p=0;
    a.push(s);
    a.threadpush();
    auto st=chrono::steady_clock::now();
    vector<thread>workers;
    for(int i=0;i<TNUM;i++)
    {
        workers.emplace_back([&]()
        {
            a.initTID();
            while(1)
            {
                bucket res=a.pop();
                if(res.empty())
                    break;
                fun x=res.take();
                int d=dis[x.x].load();
                if(x.p!=d)continue;
                for(int v:G[x.x])
                {
                    int nd=d+1;
                    int old=dis[v].load();
                    while(nd<old)
                    {
                        if(dis[v].compare_exchange_weak(old,nd))
                        {
                            fun y;
                            y.x=v;
                            y.p=nd;
                            a.push(y);
                            break;
                        }
                    }
                }
            }
        });
    }
    for(auto &t:workers)t.join();
    auto ed=chrono::steady_clock::now();
    int mmax=0;
    for(int i=1;i<=n;i++)
    {
        if(dis[i]==INF)continue;
        mmax=max(mmax,dis[i].load());
    }
    printf("%d\n",mmax);
    double tm=chrono::duration<double, milli>(ed-st).count();
    cout<<"time = "<<tm<<" ms"<<endl;
    return 0;
}