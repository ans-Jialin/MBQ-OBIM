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
vector<vector<int>>S;
vector<int>cover,gain,chosen;
void solve(OBIM &a,fun x,int tid)
{
    int i=x.x,cnt=0;
    if(chosen[i])return;
    for(int v:S[i])cnt+=!cover[v];
    gain[i]=cnt;
}
int main()
{
    read();read();read();read();
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
        OBIM a;
        for(int i=1;i<=m;i++)if(!chosen[i])a.push({i,-(int)S[i].size()},0);
    
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
