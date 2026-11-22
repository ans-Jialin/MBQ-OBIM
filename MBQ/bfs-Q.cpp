#include "MBQ.h"
#include "LJ.h"
#include<sys/resource.h>
#define N 5000100
#define RUN 100
atomic<int>dis[N];
long long pushns[TNUM],popns[TNUM],threadns[TNUM],pushcnt[TNUM],popcnt[TNUM];
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
    if(!loadLJ("data/LJ.txt",0))return 1;
    double totalcpu=0,totalwall=0;
    for(int k=1;k<=RUN;k++)
    {
    MBQ a;
    for(int i=0;i<n;i++)dis[i].store(INF);
    dis[0].store(0);
    fun s;
    s.x=0;
    s.p=0;
    a.push(s);
    a.threadpush();
    atomic<int>ready{0};
    atomic<bool>go{false};
    vector<thread>workers;
    for(int i=0;i<TNUM;i++)
    {
        workers.emplace_back([&,i]()
        {
            a.initTID();
            ready.fetch_add(1);
            while(!go.load())this_thread::yield();
            auto ts=chrono::steady_clock::now();
            while(1)
            {
                auto tp1=chrono::steady_clock::now();
                bucket res=a.pop();
                if(res.empty())
                {
                    auto tp2=chrono::steady_clock::now();
                    popns[i]+=chrono::duration_cast<chrono::nanoseconds>(tp2-tp1).count();
                    popcnt[i]++;
                    break;
                }
                fun x=res.take();
                auto tp2=chrono::steady_clock::now();
                popns[i]+=chrono::duration_cast<chrono::nanoseconds>(tp2-tp1).count();
                popcnt[i]++;
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
                            auto tq1=chrono::steady_clock::now();
                            a.push(y);
                            auto tq2=chrono::steady_clock::now();
                            pushns[i]+=chrono::duration_cast<chrono::nanoseconds>(tq2-tq1).count();
                            pushcnt[i]++;
                            break;
                        }
                    }
                }
            }
            auto te=chrono::steady_clock::now();
            threadns[i]+=chrono::duration_cast<chrono::nanoseconds>(te-ts).count();
        });
    }
    while(ready.load()!=TNUM)this_thread::yield();
    double cpu1=cputime();
    auto st=chrono::steady_clock::now();
    go.store(true);
    for(auto &t:workers)t.join();
    auto ed=chrono::steady_clock::now();
    double cpu2=cputime();
    double tm=chrono::duration<double>(ed-st).count();
    totalcpu+=cpu2-cpu1;
    totalwall+=tm;
    printf("Run %d: %.3f s\n",k,tm);
    }
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
    printf("Total CPU Time = %.3f s\n",totalcpu);
    printf("Total Wall Time = %.3f s\n",totalwall);
    printf("Average CPU Time = %.3f s\n",totalcpu/RUN);
    printf("Average Wall Time = %.3f s\n",totalwall/RUN);
    printf("CPU Utilization = %.2f%%\n",totalcpu/totalwall*100);
    printf("Core Utilization = %.2f%%\n",totalcpu/totalwall/TNUM*100);
    long long psh=0,pp=0,all=0,pc=0,qc=0;
    for(int i=0;i<TNUM;i++)
    {
        psh+=pushns[i];
        pp+=popns[i];
        all+=threadns[i];
        pc+=pushcnt[i];
        qc+=popcnt[i];
    }
    printf("Work + Other Time = %.6f s/run\n",(all-psh-pp)/1e9/RUN);
    printf("Push Time = %.6f s/run, calls = %lld, avg = %.2f ns/op\n",psh/1e9/RUN,pc,pc?double(psh)/pc:0);
    printf("Pop Time = %.6f s/run, calls = %lld, avg = %.2f ns/op\n",pp/1e9/RUN,qc,qc?double(pp)/qc:0);
    printf("Queue Overhead (thread-time) = %.2f%%\n",all?100.0*(psh+pp)/all:0);
    return 0;
}
