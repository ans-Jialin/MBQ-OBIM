#include "OBIM.h"
#define INF 0x3f3f3f3f
#define N 100010
vector<int>G[N];
int color[N],pick[N],todo[N],n;
void solve(OBIM &a,fun x,int tid)
{
    int u=x.x;
    if(color[u])return;
    for(int v:G[u])if(color[v]==0&&(G[v].size()>G[u].size()||(G[v].size()==G[u].size() && v<u)))return;
    pick[u]=1;
    vector<int>used(G[u].size()+2,0);
    for(int v:G[u])if(color[v] && color[v]<(int)used.size())used[color[v]]=1;
    int c=1;
    while(used[c])c++;
    todo[u]=c;
}
int main()
{
    read();read();read();read();
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
        OBIM a;
        for(int i=1;i<=n;i++)if(!color[i])a.push({i,-(int)G[i].size()},0);
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
