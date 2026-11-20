#include "OBIM.h"
#include "LJ.h"
#define INF 0x3f3f3f3f
#define N 5100000
atomic<int>dis[N];
void solve(OBIM &a,fun x,int tid)
{
    int u=x.x,d=dis[u].load();
    if(d>=INF||x.p!=d)return;
    int nd=d+1;
    for(int i=First[u];~i;i=Next[i])
    {
        int v=ver[i];
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
    if(!loadLJ("data/LJ.txt",0))return 1;
    if(n>N)return 1;
    static OBIM a;
    for(int i=0;i<n;i++)dis[i].store(INF);
    dis[0].store(0);
    a.push({0,0},0);
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
    int mmax=0;
    long long cnt=0,sum=0;
    for(int i=0;i<n;i++)
    {
        int d=dis[i].load();
        if(d==INF)continue;
        mmax=max(mmax,d);
        cnt++;
        sum+=d;
    }
    printf("%d\n",mmax);
    printf("Reachable = %lld\n",cnt);
    printf("Distance Sum = %lld\n",sum);
    double tm=chrono::duration<double,milli>(end-start).count();
    printf("time = %.3f ms\n",tm);
    return 0;
}
