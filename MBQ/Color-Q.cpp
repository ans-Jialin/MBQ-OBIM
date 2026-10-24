#include "MBQ.h"
#define N 100010
vector<int>G[N];
int color[N],pick[N],todo[N],n;
void solve(MBQ &a,fun x)
{
    int u=x.x;
    if(color[u])return;
    for(int v:G[u])
        if(color[v]==0 && (G[v].size()>G[u].size() ||
           (G[v].size()==G[u].size() && v<u)))return;
    pick[u]=1;
    vector<int>used(G[u].size()+2,0);
    for(int v:G[u])
        if(color[v] && color[v]<(int)used.size())used[color[v]]=1;
    int c=1;
    while(used[c])c++;
    todo[u]=c;
}
int main()
{
    GAP=read();
    RANGE=read();
    SIZE=read();
    BATCH=read();
    n=read();int m=read();
    for(int i=1;i<=m;i++)
    {
        int u=read(),v=read(),w=read();
        if(u==v)continue;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    auto start=chrono::steady_clock::now();
    int left=n;
    while(left)
    {
        MBQ a;
        for(int i=1;i<=n;i++)
            if(!color[i])a.push({i,-(int)G[i].size()});
        a.threadpush();
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
        for(int i=1;i<=n;i++)if(pick[i] && !color[i])
        {
            color[i]=todo[i];
            left--;
        }
    }
    auto end=chrono::steady_clock::now();
    int k=0;
    for(int i=1;i<=n;i++)k=max(k,color[i]);
    cout<<k<<endl;
    for(int i=1;i<=n;i++)cout<<color[i]<<" ";
    cout<<endl;
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
