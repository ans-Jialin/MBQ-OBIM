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
struct fun
{
	int x,y;
	long long z;
};
vector<fun>a,ans;
vector<int>f,sz;
bool cmp(fun wjl,fun jia)
{
	if(wjl.z!=jia.z)return wjl.z<jia.z;
	if(wjl.x!=jia.x)return wjl.x<jia.x;
	return wjl.y<jia.y;
}
int Find(int x)
{
	if(f[x]==x)return x;
	return f[x]=Find(f[x]);
}
void kruskal()
{
	sort(a.begin(),a.end(),cmp);
	for(int i=0;i<n;i++)f[i]=i;
	for(int i=0;i<m;i++)
	{
		int x=Find(a[i].x),y=Find(a[i].y);
		if(x==y)continue;
		if(sz[x]<sz[y])swap(x,y);
		f[y]=x;
		sz[x]+=sz[y];
		ans.push_back(a[i]);
	}
}
int main ()
{
	n=read(),m=read();
	if(n<=0||m<0)return 0;
	a.resize(m);
	f.resize(n),sz.assign(n,1);
	for(int i=0;i<m;i++)
	{
		a[i].x=read(),a[i].y=read();
		scanf("%lld",&a[i].z);
	}
	kruskal();
	long long sum=0;
	for(int i=0;i<(int)ans.size();i++)sum+=ans[i].z;
	printf("%d %d %lld\n",n-(int)ans.size(),(int)ans.size(),sum);
	for(int i=0;i<(int)ans.size();i++)printf("%d %d %lld\n",ans[i].x,ans[i].y,ans[i].z);
	return 0;
}
