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
int n,d,k;
vector<double>a,c,sum;
vector<int>id,cnt;
void kmeans()
{
	for(int i=0;i<k;i++)
		for(int j=0;j<d;j++)c[(long long)i*d+j]=a[((long long)i*n/k)*d+j];
	for(int t=0;t<100;t++)
	{
		fill(sum.begin(),sum.end(),0);
		fill(cnt.begin(),cnt.end(),0);
		int change=0;
		for(int i=0;i<n;i++)
		{
			int best=0;
			double mn=1e300;
			for(int x=0;x<k;x++)
			{
				double ds=0;
				for(int j=0;j<d;j++)
				{
					double v=a[(long long)i*d+j]-c[(long long)x*d+j];
					ds+=v*v;
				}
				if(ds<mn)
				{
					mn=ds;
					best=x;
				}
			}
			if(id[i]!=best)change++;
			id[i]=best;
			cnt[best]++;
			for(int j=0;j<d;j++)sum[(long long)best*d+j]+=a[(long long)i*d+j];
		}
		for(int i=0;i<k;i++)
		{
			if(!cnt[i])continue;
			for(int j=0;j<d;j++)c[(long long)i*d+j]=sum[(long long)i*d+j]/cnt[i];
		}
		if(!change)break;
	}
}
int main ()
{
	n=read(),d=read();
	if(n<=0||d<=0)return 0;
	k=min(40,n);
	a.resize((long long)n*d),c.resize((long long)k*d),sum.resize((long long)k*d);
	id.assign(n,-1),cnt.assign(k,0);
	for(int i=0;i<n;i++)
		for(int j=0;j<d;j++)scanf("%lf",&a[(long long)i*d+j]);
	kmeans();
	double ans=0;
	for(int i=0;i<n;i++)
	{
		int x=id[i];
		for(int j=0;j<d;j++)
		{
			double v=a[(long long)i*d+j]-c[(long long)x*d+j];
			ans+=v*v;
		}
	}
	printf("%d %.10f\n",k,ans);
	return 0;
}
