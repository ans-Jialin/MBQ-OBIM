#include "OBIM.h"
#define INF 0x3f3f3f3f
#define N 5100000
vector<int>G[N];
atomic<int>dis[N];
void solve(OBIM &a,fun x,int tid)
{
    int u=x.x,d=dis[u].load();
    if(d>=INF||x.p!=d)return;
    int nd=d+1;
    for(int v:G[u])
    {
        int old=dis[v].load();
        while(nd<old)
        {
            if(dis[v].compare_exchange_weak(old,nd))
            {
                a.push({v,nd},tid);
                break;
            }
        }
    }
}

int main()
{
    read();read();read();read();
    static OBIM a;
    int n=read(),m=read();
    for(int i=0;i<=n;i++)dis[i].store(INF);
    for(int i=1;i<=m;i++)
    {
        int u=read(),v=read();
        G[u].push_back(v);
    }
    int s=read(),t=read();
    dis[s].store(0);
    a.push({s,0},0);
    auto start=chrono::steady_clock::now();
    vector<thread>workers;
    for(int i=0;i<TNUM;i++)
    {
        workers.emplace_back([&,i]()
        {
            bindCPU(i);
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
    auto end=chrono::steady_clock::now();
    if(dis[t].load()==INF)printf("INF\n");
    else printf("%d\n",dis[t].load());
    double tm=chrono::duration<double,milli>(end-start).count();
    printf("time = %.3f ms\n",tm);
    return 0;
}
