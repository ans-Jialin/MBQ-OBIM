#include "MBQ.h"
#define N 100010
struct Edge
{
    int u,v;
    double w;
};
vector<Edge>E;
vector<int>in[N];
vector<double>msg,ans,res;
double bias[N];
void solve(MBQ &a,fun x)
{
    int id=x.x,u=E[id].u,v=E[id].v;
    double s=bias[u];
    for(int j:in[u])if(E[j].u!=v)s+=msg[j];
    double z=tanh(E[id].w)*tanh(s);
    z=max(-0.999999999,min(0.999999999,z));
    ans[id]=atanh(z);
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
        double j=w/100.0;
        int id=E.size();
        E.push_back({u,v,j});
        E.push_back({v,u,j});
        in[v].push_back(id);
        in[u].push_back(id+1);
    }
    for(int i=1;i<=n;i++)bias[i]=read()/100.0;
    int iters=read();
    msg.assign(E.size(),0);
    ans.assign(E.size(),0);
    res.assign(E.size(),1);
    auto start=chrono::steady_clock::now();
    for(int it=0;it<iters;it++)
    {
        MBQ a;
        for(int i=0;i<(int)E.size();i++)a.push({i,-min(1000000,(int)(res[i]*1000000))});
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
        for(int i=0;i<(int)E.size();i++)res[i]=abs(ans[i]-msg[i]);
        msg.swap(ans);
    }
    auto end=chrono::steady_clock::now();
    cout<<fixed<<setprecision(9);
    for(int u=1;u<=n;u++)
    {
        double s=bias[u];
        for(int i:in[u])s+=msg[i];
        double p=1/(1+exp(-2*s));
        cout<<u<<" "<<p<<endl;
    }
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
