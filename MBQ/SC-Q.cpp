#include "MBQ.h"
#define N 100010
vector<vector<int>>S;
vector<int>cover,gain,chosen;
void solve(MBQ &a,fun x)
{
    int i=x.x,cnt=0;
    if(chosen[i])return;
    for(int v:S[i])cnt+=!cover[v];
    gain[i]=cnt;
}
int main()
{
    GAP=read();
    RANGE=read();
    SIZE=read();
    BATCH=read();
    int n=read(),m=read();
    S.resize(m+1);
    cover.resize(n+1);
    gain.resize(m+1);
    chosen.resize(m+1);
    for(int i=1;i<=m;i++)
    {
        int k=read();
        for(int j=1;j<=k;j++)S[i].push_back(read());
        sort(S[i].begin(),S[i].end());
        S[i].erase(unique(S[i].begin(),S[i].end()),S[i].end());
    }
    auto start=chrono::steady_clock::now();
    vector<int>ans;
    int left=n;
    while(left)
    {
        MBQ a;
        for(int i=1;i<=m;i++)if(!chosen[i])a.push({i,-(int)S[i].size()});
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
        int id=0;
        for(int i=1;i<=m;i++)if(!chosen[i] && gain[i]>(id?gain[id]:0))id=i;
        if(id==0)break;
        chosen[id]=1;
        ans.push_back(id);
        for(int v:S[id])if(!cover[v])cover[v]=1,left--;
    }
    auto end=chrono::steady_clock::now();
    if(left)cout<<"IMPOSSIBLE"<<endl;
    else
    {
        cout<<ans.size()<<endl;
        for(int v:ans)cout<<v<<" ";
        cout<<endl;
    }
    cout<<"time = "<<chrono::duration<double,milli>(end-start).count()<<" ms"<<endl;
    return 0;
}
