#include "MBQ.h"
#define N 100010
vector<int>G[N];
int state[N],pick[N],n;
void solve(MBQ &a,fun x)
{
    int u=x.x;
    if(state[u])return;
    for(int v:G[u])
        if(state[v]==0 && v<u)return;
    pick[u]=1;
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
            if(!state[i])a.push({i,-(int)G[i].size()});
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
        for(int i=1;i<=n;i++)
            if(pick[i] && !state[i])state[i]=1,left--;
        for(int i=1;i<=n;i++)
            if(pick[i])for(int v:G[i])
                if(!state[v])state[v]=-1,left--;
    }
    auto end=chrono::steady_clock::now();
    int cnt=0;
    for(int i=1;i<=n;i++)cnt+=(state[i]==1);
    cout<<cnt<<endl;
    for(int i=1;i<=n;i++)if(state[i]==1)cout<<i<<" ";
    cout<<endl;
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
