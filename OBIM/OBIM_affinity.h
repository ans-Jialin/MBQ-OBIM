#pragma once
#include<cstdio>
#include<cstdlib>
#ifndef OBIM_THREADS
#define OBIM_THREADS 16
#endif
#ifndef OBIM_BIND
#define OBIM_BIND 0
#endif
const int OBIM_CPU[24]={0,2,4,6,8,10,12,14,16,17,18,19,20,21,22,23,1,3,5,7,9,11,13,15};
#if OBIM_BIND
#include<pthread.h>
#include<sched.h>
#endif
inline void bindCPU(int tid)
{
#if OBIM_BIND
    if(tid<0||tid>=OBIM_THREADS||tid>=24)
    {
        printf("Invalid tid=%d\n",tid);
        exit(1);
    }
    cpu_set_t m;
    CPU_ZERO(&m);
    CPU_SET(OBIM_CPU[tid],&m);
    int x=pthread_setaffinity_np(pthread_self(),sizeof(m),&m);
    if(x!=0)
    {
        printf("Thread %d bind failed: %d\n",tid,x);
        exit(1);
    }
#else
    (void)tid;
#endif
}