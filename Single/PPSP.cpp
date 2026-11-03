#include<cstdio>
#include<iostream>
#include<cstring>
#include<climits>
#include<algorithm>
#include<cmath>
#include<queue>
#include<utility>
#include<vector>
using namespace std;
inline int read()
{
    int wjl=0,jia=1;
    char ch=getchar();
    while(ch<'0'||ch>'9')
    {
        if(ch=='-')jia*=-1;
        ch=getchar();
    }
    while(ch>='0'&&ch<='9')
    {
        wjl=(wjl<<1)+(wjl<<3)+(ch^48);
        ch=getchar();
    }
    return wjl*jia;
}
#define INF (LLONG_MAX/4)
int n,m,dir,s,t;
vector<int>First,Next,ver;
vector<long long>edge,dis;
struct fun
{
	long long d;
	int x;
	bool operator<(fun a)const
	{
		return d>a.d;
	}
};
void add(int x,int y,long long z)
{
	ver.push_back(y);
	Next.push_back(First[x]);
	edge.push_back(z);
	First[x]=ver.size()-1;
}
void dijkstra()
{
	dis.assign(n,INF);
	priority_queue<fun>q;
	dis[s]=0;
	q.push({0,s});
	while(!q.empty())
	{
		fun a=q.top();
		q.pop();
		int x=a.x;
		if(a.d!=dis[x])continue;
		if(x==t)return;
		for(int i=First[x];~i;i=Next[i])
		{
			int y=ver[i];
			long long z=edge[i];
			if(dis[x]+z>=dis[y])continue;
			dis[y]=dis[x]+z;
			q.push({dis[y],y});
		}
	}
}
int main ()
{
	n=read(),m=read(),dir=read();
	if(n<=0||m<0)return 0;
	First.assign(n,-1);
	for(int i=1;i<=m;i++)
	{
		int x=read(),y=read();
		long long z;
		scanf("%lld",&z);
		if(z<0)return 1;
		add(x,y,z);
		if(!dir)add(y,x,z);
	}
	s=read(),t=read();
	dijkstra();
	printf("%lld\n",dis[t]==INF?-1:dis[t]);
	return 0;
}
