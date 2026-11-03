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
vector<int>First,Next,ver,deg,mark;
vector<pair<int,int>>a;
long long ans;
bool cmp(pair<int,int>wjl,pair<int,int>jia)
{
	if(wjl.first!=jia.first)return wjl.first<jia.first;
	return wjl.second<jia.second;
}
void add(int x,int y)
{
	ver.push_back(y);
	Next.push_back(First[x]);
	First[x]=ver.size()-1;
}
void tri()
{
	for(int x=0;x<n;x++)
	{
		for(int i=First[x];~i;i=Next[i])mark[ver[i]]=x+1;
		for(int i=First[x];~i;i=Next[i])
		{
			int y=ver[i];
			for(int j=First[y];~j;j=Next[j])
			{
				int z=ver[j];
				if(mark[z]==x+1)ans++;
			}
		}
	}
}
int main ()
{
	n=read(),m=read();
	if(n<=0||m<0)return 0;
	deg.assign(n,0),mark.assign(n,0),First.assign(n,-1);
	for(int i=1;i<=m;i++)
	{
		int x=read(),y=read();
		if(x==y)return 1;
		if(x>y)swap(x,y);
		a.push_back(make_pair(x,y));
	}
	sort(a.begin(),a.end(),cmp);
	a.erase(unique(a.begin(),a.end()),a.end());
	for(int i=0;i<(int)a.size();i++)
	{
		deg[a[i].first]++;
		deg[a[i].second]++;
	}
	for(int i=0;i<(int)a.size();i++)
	{
		int x=a[i].first,y=a[i].second;
		if(deg[x]>deg[y]||(deg[x]==deg[y]&&x>y))swap(x,y);
		add(x,y);
	}
	tri();
	printf("%lld\n",ans);
	return 0;
}
