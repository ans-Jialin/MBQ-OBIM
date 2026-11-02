#include "MBQ.h"
#define N 100010
vector<int>in[N];
int deg[N],n;
vector<double>r,ans;
double dangling;
const double DAMP=0.85;
void solve(MBQ &a,fun x)
{
    int v=x.x;
    double s=dangling/n;
    for(int u:in[v])s+=r[u]/deg[u];
    ans[v]=(1-DAMP)/n+DAMP*s;
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
        in[v].push_back(u);
        deg[u]++;
    }
    int iters=read();
    r.assign(n+1,1.0/n);
    ans.assign(n+1,0);
    auto start=chrono::steady_clock::now();
    for(int it=0;it<iters;it++)
    {
        dangling=0;
        for(int i=1;i<=n;i++)if(deg[i]==0)dangling+=r[i];
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
        r.swap(ans);
    }
    auto end=chrono::steady_clock::now();
    double sum=0;
    for(int i=1;i<=n;i++)sum+=r[i];
    cout<<fixed<<setprecision(9)<<"rank_sum = "<<sum<<endl;
    for(int i=1;i<=min(n,10);i++)cout<<i<<" "<<r[i]<<endl;
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
