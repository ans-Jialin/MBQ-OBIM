#include "OBIM.h"
#define INF 0x3f3f3f3f
#define KMAX 100
vector<vector<double>>x,c,sum;
vector<int>belong,cnt;
int n,d,k;
void solve(OBIM &a,fun p,int tid)
{
    int u=p.x,id=0;
    double best=1e100;
    for(int j=0;j<k;j++)
    {
        double s=0;
        for(int t=0;t<d;t++)s+=(x[u][t]-c[j][t])*(x[u][t]-c[j][t]);
        if(s<best)best=s,id=j;
    }
    belong[u]=id;
}
int main()
{
    read();read();read();read();
    n=read();d=read();k=read();
    int iters=read();
    if(n<=0||d<=0||k<=0 || k>n)return 1;
    x.assign(n,vector<double>(d));
    c.assign(k,vector<double>(d));
    belong.assign(n,0);
    cnt.assign(k,0);
    sum.assign(k,vector<double>(d));
    for(int i=0;i<n;i++)for(int j=0;j<d;j++)scanf("%lf",&x[i][j]);
    for(int j=0;j<k;j++)c[j]=x[j];
    auto start=chrono::steady_clock::now();
    for(int it=0;it<iters;it++)
    {
        OBIM a;
        for(int i=0;i<n;i++)a.push({i,0},0);
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
        for(int j=0;j<k;j++)
        {
            cnt[j]=0;
            fill(sum[j].begin(),sum[j].end(),0);
        }
        for(int i=0;i<n;i++)
        {
            int j=belong[i];
            cnt[j]++;
            for(int t=0;t<d;t++)sum[j][t]+=x[i][t];
        }
        for(int j=0;j<k;j++)
        {
            if(cnt[j])for(int t=0;t<d;t++)c[j][t]=sum[j][t]/cnt[j];
        }
    }
    auto end=chrono::steady_clock::now();
    double sse=0;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<d;j++)sse+=(x[i][j]-c[belong[i]][j])*(x[i][j]-c[belong[i]][j]);
    }
    cout<<fixed<<setprecision(6)<<"SSE = "<<sse<<endl;
    for(int j=0;j<k;j++)
    {
        for(double z:c[j])cout<<z<<" ";
        cout<<endl;
    }
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
