#include "OBIM.h"
#define INF 0x3f3f3f3f
inline int read()
{
    int x=0,f=1;
    char ch=getchar();
    while(ch!='-' && (ch<'0'||ch>'9'))ch=getchar();
    if(ch=='-')f=-1,ch=getchar();
    while(ch>='0'&&ch<='9')
    {
        x=x*10+ch-'0';
        ch=getchar();
    }
    return x*f;
}
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
void solve(OBIM &a,fun x,int tid)
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
    read();read();read();read();
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
        OBIM a;
        for(int i=1;i<=n;i++)a.push({i,0},0);
    
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
