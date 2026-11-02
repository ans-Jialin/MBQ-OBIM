#include "MBQ.h"
#define N 100010
struct Edge
{
    int u,v,w;
};
vector<Edge>E;
vector<int>G[N];
int fa[N],best[N];
mutex L[N];
int root(int u)
{
    while(fa[u]!=u)u=fa[u];
    return u;
}
void solve(MBQ &a,fun x)
{
    int u=x.x,r=root(u),id=0;
    for(int i:G[u])
    {
        int v=E[i].u^E[i].v^u;
        if(root(v)==r)continue;
        if(!id || E[i].w<E[id].w || (E[i].w==E[id].w && i<id))id=i;
    }
    if(id)
    {
        lock_guard<mutex>lock(L[r]);
        if(!best[r] || E[id].w<E[best[r]].w ||
           (E[id].w==E[best[r]].w && id<best[r]))best[r]=id;
    }
}
int main()
{
    GAP=read();
    RANGE=read();
    SIZE=read();
    BATCH=read();
    int n=read(),m=read();
    E.resize(m+1);
    for(int i=1;i<=n;i++)fa[i]=i;
    for(int i=1;i<=m;i++)
    {
        int u=read(),v=read(),w=read();
        E[i]={u,v,w};
        if(u!=v)G[u].push_back(i),G[v].push_back(i);
    }
    auto start=chrono::steady_clock::now();
    long long ans=0;
    vector<int>chosen;
    while(1)
    {
        fill(best,best+n+1,0);
        MBQ a;
        for(int i=1;i<=n;i++)a.push({i,0});
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
        int changes=0;
        for(int i=1;i<=n;i++)if(fa[i]==i && best[i])
        {
            int id=best[i],u=root(E[id].u),v=root(E[id].v);
            if(u==v)continue;
            fa[u]=v;
            ans+=E[id].w;
            chosen.push_back(id);
            changes++;
        }
        if(!changes)break;
    }
    auto end=chrono::steady_clock::now();
    cout<<ans<<" "<<chosen.size()<<endl;
    for(int i:chosen)cout<<E[i].u<<" "<<E[i].v<<" "<<E[i].w<<endl;
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
