#include "OBIM.h"
#define INF 0x3f3f3f3f
#define N 100010
vector<int>G[N];
vector<pair<int,int>>E;
vector<int>best,used,pick;
void solve(OBIM &a,fun x,int tid)
{
    int i=x.x,u=E[i].first,v=E[i].second;
    if(used[u]||used[v])return;
    if(best[u]==i && best[v]==i)pick[i]=1;
}
int main()
{
    read();read();read();read();
    int n=read(),m=read();
    E.resize(m+1);
    best.resize(n+1);
    used.resize(n+1);
    pick.resize(m+1);
    for(int i=1;i<=m;i++)
    {
        int u=read(),v=read(),w=read();
        E[i]={u,v};
        if(u==v)continue;
        G[u].push_back(i);
        G[v].push_back(i);
    }
    auto start=chrono::steady_clock::now();
    vector<pair<int,int>>ans;
    while(1)
    {
        fill(best.begin(),best.end(),INT_MAX);
        for(int i=1;i<=m;i++)
        {
            int u=E[i].first,v=E[i].second;
            if(u==v||used[u]||used[v])continue;
            best[u]=min(best[u],i);
            best[v]=min(best[v],i);
        }
        fill(pick.begin(),pick.end(),0);
        OBIM a;
        int alive=0;
        for(int i=1;i<=m;i++)
        {
            int u=E[i].first,v=E[i].second;
            if(u!=v && !used[u] && !used[v])
            {
                a.push({i,-(int)(G[u].size()+G[v].size())},0);
                alive++;
            }
        }
        if(!alive)break;
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
        int cnt=0;
        for(int i=1;i<=m;i++)if(pick[i])
        {
            int u=E[i].first,v=E[i].second;
            used[u]=used[v]=1;
            ans.push_back({u,v});
            cnt++;
        }
        if(!cnt)break;
    }
    auto end=chrono::steady_clock::now();
    cout<<ans.size()<<endl;
    for(auto e:ans)cout<<e.first<<" "<<e.second<<endl;
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
