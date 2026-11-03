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
int n,m,left;
vector<vector<int>>a,g;
vector<int>cnt,vis,used,ans;
priority_queue<pair<int,int>>q;
bool cmp(int wjl,int jia)
{
	return wjl<jia;
}
void sc()
{
	while(left&&!q.empty())
	{
		int k=q.top().first,id=-q.top().second;
		q.pop();
		if(used[id]||cnt[id]!=k)continue;
		if(k==0)break;
		used[id]=1;
		ans.push_back(id);
		for(int j=0;j<(int)a[id].size();j++)
		{
			int x=a[id][j];
			if(vis[x])continue;
			vis[x]=1;
			left--;
			for(int z=0;z<(int)g[x].size();z++)
			{
				int y=g[x][z];
				if(used[y])continue;
				cnt[y]--;
				q.push(make_pair(cnt[y],-y));
			}
		}
	}
}
int main ()
{
	n=read(),m=read();
	if(n<0||m<0)return 0;
	left=n;
	a.resize(m),g.resize(n);
	cnt.assign(m,0),vis.assign(n,0),used.assign(m,0);
	for(int i=0;i<m;i++)
	{
		int k=read();
		a[i].resize(k);
		for(int j=0;j<k;j++)a[i][j]=read();
		sort(a[i].begin(),a[i].end(),cmp);
		a[i].erase(unique(a[i].begin(),a[i].end()),a[i].end());
		cnt[i]=a[i].size();
		for(int j=0;j<(int)a[i].size();j++)g[a[i][j]].push_back(i);
		q.push(make_pair(cnt[i],-i));
	}
	sc();
	if(left)
	{
		printf("-1\n");
		return 0;
	}
	printf("%d\n",(int)ans.size());
	for(int i=0;i<(int)ans.size();i++)printf("%d%c",ans[i],i+1==(int)ans.size()?'\n':' ');
	return 0;
}
