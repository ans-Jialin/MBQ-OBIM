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
vector<int>First,Next,ver,vis,ans;
void add(int x,int y)
{
	ver.push_back(y);
	Next.push_back(First[x]);
	First[x]=ver.size()-1;
}
void mis()
{
	vis.assign(n,0);
	for(int x=0;x<n;x++)
	{
		if(vis[x])continue;
		ans.push_back(x);
		vis[x]=1;
		for(int i=First[x];~i;i=Next[i])vis[ver[i]]=1;
	}
}
int main ()
{
	n=read(),m=read();
	if(n<=0||m<0)return 0;
	First.assign(n,-1);
	for(int i=1;i<=m;i++)
	{
		int x=read(),y=read();
		if(x==y)return 1;
		add(x,y);
		add(y,x);
	}
	mis();
	printf("%d\n",(int)ans.size());
	for(int i=0;i<(int)ans.size();i++)printf("%d%c",ans[i],i+1==(int)ans.size()?'\n':' ');
	return 0;
}
