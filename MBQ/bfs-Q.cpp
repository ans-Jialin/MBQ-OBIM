#include "MBQ.h"
#include "LJ.h"
#include<sys/resource.h>
#define N 5000100
atomic<int>dis[N];
double cputime()
{
    rusage r;
    getrusage(RUSAGE_SELF,&r);
    return r.ru_utime.tv_sec+r.ru_utime.tv_usec/1e6+
    r.ru_stime.tv_sec+r.ru_stime.tv_usec/1e6;
}
int main()
{
    GAP=1;
    RANGE=0;
    SIZE=64;
    BATCH=16;
    static MBQ a;
    if(!loadLJ("/Users/jialin/datasets/LJ.txt",0))return 1;
    for(int i=0;i<n;i++)dis[i].store(INF);
    dis[0].store(0);
    fun s;
    s.x=0;
    s.p=0;
    a.push(s);
    a.threadpush();
    double cpu1=cputime();
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
                if(res.empty())break;
                fun x=res.take();
                int d=dis[x.x].load();
                if(x.p!=d)continue;
                for(int i=First[x.x];~i;i=Next[i])
                {
                    int v=ver[i];
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
    double cpu2=cputime();
    int mmax=0;
    for(int i=0;i<n;i++)
    {
        if(dis[i]==INF)continue;
        mmax=max(mmax,dis[i].load());
    }
    printf("%d\n",mmax);
    double tm=chrono::duration<double>(ed-st).count();
    printf("CPU Time = %.3f s\n",cpu2-cpu1);
    printf("Wall Time = %.3f s\n",tm);
    printf("CPU Utilization = %.2f%%\n",(cpu2-cpu1)/tm*100);
    printf("Core Utilization = %.2f%%\n",(cpu2-cpu1)/tm/TNUM*100);
    return 0;
}