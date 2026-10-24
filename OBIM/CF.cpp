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
#define K 4
struct Rating
{
    int u,v;
    double w;
};
vector<Rating>E;
vector<vector<int>>gu,gv;
vector<vector<double>>U,V;
int phase;
const double LR=0.05,REG=0.01;
void solve(OBIM &a,fun x,int tid)
{
    int u=x.x;
    double grad[K]={0};
    if(phase==0)
    {
        if(gu[u].empty())return;
        for(int id:gu[u])
        {
            int v=E[id].v;
            double p=0;
            for(int j=0;j<K;j++)p+=U[u][j]*V[v][j];
            double err=E[id].w-p;
            for(int j=0;j<K;j++)grad[j]+=err*V[v][j];
        }
        for(int j=0;j<K;j++)
            U[u][j]+=LR*(grad[j]/gu[u].size()-REG*U[u][j]);
    }
    else
    {
        if(gv[u].empty())return;
        for(int id:gv[u])
        {
            int v=E[id].u;
            double p=0;
            for(int j=0;j<K;j++)p+=U[v][j]*V[u][j];
            double err=E[id].w-p;
            for(int j=0;j<K;j++)grad[j]+=err*U[v][j];
        }
        for(int j=0;j<K;j++)
            V[u][j]+=LR*(grad[j]/gv[u].size()-REG*V[u][j]);
    }
}
int main()
{
    read();read();read();read();
    int n=read(),m=read(),cnt=read();
    E.resize(cnt);
    gu.resize(n+1);
    gv.resize(m+1);
    U.assign(n+1,vector<double>(K));
    V.assign(m+1,vector<double>(K));
    for(int i=0;i<cnt;i++)
    {
        int u=read(),v=read(),w=read();
        E[i]={u,v,(double)w};
        gu[u].push_back(i);
        gv[v].push_back(i);
    }
    int iters=read();
    for(int i=1;i<=n;i++)for(int j=0;j<K;j++)U[i][j]=0.1+0.01*((i+j)%7);
    for(int i=1;i<=m;i++)for(int j=0;j<K;j++)V[i][j]=0.1+0.01*((i+2*j)%7);
    auto start=chrono::steady_clock::now();
    for(int it=0;it<iters;it++)
        for(phase=0;phase<2;phase++)
        {
            OBIM a;
            int lim=phase?m:n;
            for(int i=1;i<=lim;i++)
                a.push({i,-(int)(phase?gv[i].size():gu[i].size())},0);
        
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
        }
    auto end=chrono::steady_clock::now();
    double err=0;
    for(auto e:E)
    {
        double p=0;
        for(int j=0;j<K;j++)p+=U[e.u][j]*V[e.v][j];
        err+=(e.w-p)*(e.w-p);
    }
    cout<<fixed<<setprecision(6)<<"RMSE = "<<sqrt(err/max(cnt,1))<<endl;
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
