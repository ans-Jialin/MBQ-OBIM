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
int n,m,dir,step;
vector<int>First,Next,ver,deg;
vector<double>p,q;
const double d=0.85,eps=1e-10;
void add(int x,int y)
{
	ver.push_back(y);
	Next.push_back(First[x]);
	First[x]=ver.size()-1;
	deg[x]++;
}
void pagerank()
{
	p.assign(n,1.0/n),q.resize(n);
	for(int t=0;t<1000;t++)
	{
		double lost=0;
		for(int i=0;i<n;i++)if(!deg[i])lost+=p[i];
		fill(q.begin(),q.end(),(1.0-d+d*lost)/n);
		for(int i=0;i<n;i++)
		{
			if(!deg[i])continue;
			double x=d*p[i]/deg[i];
			for(int j=First[i];~j;j=Next[j])q[ver[j]]+=x;
		}
		double diff=0;
		for(int i=0;i<n;i++)diff+=fabs(p[i]-q[i]);
		p.swap(q);
		step=t+1;
		if(diff<eps)break;
	}
}
int main ()
{
	n=read(),m=read(),dir=read();
	if(n<=0||m<0)return 0;
	First.assign(n,-1),deg.assign(n,0);
	for(int i=1;i<=m;i++)
	{
		int x=read(),y=read();
		add(x,y);
		if(!dir)add(y,x);
	}
	pagerank();
	int best=0;
	double ans=0;
	for(int i=0;i<n;i++)
	{
		ans+=p[i];
		if(p[i]>p[best])best=i;
	}
	printf("%d %d %.12f %.12f\n",step,best,p[best],ans);
	return 0;
}
