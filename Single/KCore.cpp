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
int n,m;
vector<int>First,Next,ver,deg,core,vis;
priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>q;
void add(int x,int y)
{
	ver.push_back(y);
	Next.push_back(First[x]);
	First[x]=ver.size()-1;
	deg[x]++;
}
void kcore()
{
	for(int i=0;i<n;i++)q.push(make_pair(deg[i],i));
	int mx=0;
	long long sum=0;
	while(!q.empty())
	{
		int d=q.top().first,x=q.top().second;
		q.pop();
		if(vis[x]||d!=deg[x])continue;
		vis[x]=1;
		mx=max(mx,d);
		core[x]=mx;
		sum+=mx;
		for(int i=First[x];~i;i=Next[i])
		{
			int y=ver[i];
			if(vis[y])continue;
			deg[y]--;
			q.push(make_pair(deg[y],y));
		}
	}
	printf("%d %lld\n",mx,sum);
}
int main ()
{
	n=read(),m=read();
	if(n<=0||m<0)return 0;
	First.assign(n,-1),deg.assign(n,0);
	core.assign(n,0),vis.assign(n,0);
	for(int i=1;i<=m;i++)
	{
		int x=read(),y=read();
		if(x==y)return 1;
		add(x,y);
		add(y,x);
	}
	kcore();
	for(int i=0;i<n;i++)printf("%d%c",core[i],i+1==n?'\n':' ');
	return 0;
}
