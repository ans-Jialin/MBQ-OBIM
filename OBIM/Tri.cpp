#include "OBIM.h"
#define INF 0x3f3f3f3f
#define N 100010
vector<int>G[N];
atomic<long long>cnt{0};
void solve(OBIM &a,fun x,int tid)
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
    read();read();read();read();
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
    OBIM a;
    for(int i=1;i<=n;i++)a.push({i,-(int)G[i].size()},0);
    auto start=chrono::steady_clock::now();
    vector<thread>workers;
    for(int i=0;i<TNUM;i++)
    {
        workers.emplace_back([&,i]()
        {
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
    cout<<cnt.load()<<endl;
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
