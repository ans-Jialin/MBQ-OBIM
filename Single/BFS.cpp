#include "LJ.h"
#include<chrono>
using namespace std;
vector<int>dis,q;
void bfs(int s)
{
    dis.assign(n,-1);
    q.resize(n);
    int head=0,tail=0;
    dis[s]=0;
    q[tail++]=s;
    while(head<tail)
    {
        int x=q[head++];
        for(int i=First[x];~i;i=Next[i])
        {
            int y=ver[i];
            if(dis[y]!=-1)continue;
            dis[y]=dis[x]+1;
            q[tail++]=y;
        }
    }
}
int main()
{
    if(!loadLJ("data/LJ.txt",0))return 1;
    auto st=chrono::steady_clock::now();
    bfs(0);
    auto ed=chrono::steady_clock::now();
    int mmax=0;
    long long cnt=0,sum=0;
    for(int i=0;i<n;i++)
    {
        if(dis[i]==-1)continue;
        mmax=max(mmax,dis[i]);
        cnt++;
        sum+=dis[i];
    }
    printf("%d\n",mmax);
    printf("Reachable = %lld\n",cnt);
    printf("Distance Sum = %lld\n",sum);
    double tm=chrono::duration<double>(ed-st).count();
    printf("Wall Time = %.3f s\n",tm);
    return 0;
}