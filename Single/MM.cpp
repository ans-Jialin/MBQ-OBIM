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
vector<int>vis;
vector<pair<int,int>>ans;
void mm()
{
	for(int i=1;i<=m;i++)
	{
		int x=read(),y=read();
		if(x==y)continue;
		if(vis[x]||vis[y])continue;
		vis[x]=vis[y]=1;
		ans.push_back(make_pair(x,y));
	}
}
int main ()
{
	n=read(),m=read();
	if(n<=0||m<0)return 0;
	vis.assign(n,0);
	mm();
	printf("%d\n",(int)ans.size());
	for(int i=0;i<(int)ans.size();i++)printf("%d %d\n",ans[i].first,ans[i].second);
	return 0;
}
