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
vector<int>First,Next,ver,deg,c,used;
struct fun
{
	int x,d;
};
vector<fun>a;
bool cmp(fun wjl,fun jia)
{
	if(wjl.d!=jia.d)return wjl.d>jia.d;
	return wjl.x<jia.x;
}
void add(int x,int y)
{
	ver.push_back(y);
	Next.push_back(First[x]);
	First[x]=ver.size()-1;
	deg[x]++;
}
void color()
{
	for(int i=0;i<n;i++)a[i]={i,deg[i]};
	sort(a.begin(),a.end(),cmp);
	int mx=0;
	for(int i=0;i<n;i++)
	{
		int x=a[i].x;
		for(int j=First[x];~j;j=Next[j])
		{
			int y=ver[j];
			if(c[y])used[c[y]]=i+1;
		}
		int z=1;
		while(used[z]==i+1)z++;
		c[x]=z;
		if(z>mx)mx=z;
	}
	printf("%d\n",mx);
}
int main ()
{
	n=read(),m=read();
	if(n<=0||m<0)return 0;
	First.assign(n,-1);
	deg.assign(n,0);
	for(int i=1;i<=m;i++)
	{
		int x=read(),y=read();
		if(x==y)return 1;
		add(x,y);
		add(y,x);
	}
	a.resize(n),c.assign(n,0),used.assign(n+1,0);
	color();
	for(int i=0;i<n;i++)printf("%d%c",c[i],i+1==n?'\n':' ');
	return 0;
}
