#include "MBQ.h"
#define N 100010
vector<int>G[N];
atomic<long long>cnt{0};
void solve(MBQ &a,fun x)
{
    int u=x.x;
    long long s=0;
    for(int v:G[u])if(v>u)
        for(int w:G[v])if(w>v)
            if(binary_search(G[u].begin(),G[u].end(),w))s++;
    cnt.fetch_add(s);
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
    }
    MBQ a;
    for(int i=1;i<=n;i++)a.push({i,-(int)G[i].size()});
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
    cout<<cnt.load()<<endl;
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
