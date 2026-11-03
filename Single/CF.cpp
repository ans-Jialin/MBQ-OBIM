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
int n,m,r,k;
struct fun
{
	int x,y;
	double z;
};
vector<fun>a;
vector<double>p,q;
const double lr=0.01,reg=0.02;
void cf()
{
	for(int i=0;i<r;i++)
	{
		long long x=(long long)a[i].x*k,y=(long long)a[i].y*k;
		double s=0;
		for(int j=0;j<k;j++)s+=p[x+j]*q[y+j];
		double err=a[i].z-s;
		for(int j=0;j<k;j++)
		{
			double t=p[x+j];
			p[x+j]+=lr*(err*q[y+j]-reg*p[x+j]);
			q[y+j]+=lr*(err*t-reg*q[y+j]);
		}
	}
}
int main ()
{
	n=read(),m=read(),r=read(),k=read();
	if(n<=0||m<=0||r<0||k<=0)return 0;
	a.resize(r);
	for(int i=0;i<r;i++)
	{
		a[i].x=read(),a[i].y=read();
		scanf("%lf",&a[i].z);
	}
	p.resize((long long)n*k),q.resize((long long)m*k);
	for(int i=0;i<n;i++)
		for(int j=0;j<k;j++)p[(long long)i*k+j]=0.001*((i*131LL+j*17)%101+1);
	for(int i=0;i<m;i++)
		for(int j=0;j<k;j++)q[(long long)i*k+j]=0.001*((i*73LL+j*29)%101+1);
	cf();
	double ans=0;
	for(int i=0;i<r;i++)
	{
		long long x=(long long)a[i].x*k,y=(long long)a[i].y*k;
		double s=0;
		for(int j=0;j<k;j++)s+=p[x+j]*q[y+j];
		ans+=(a[i].z-s)*(a[i].z-s);
	}
	printf("%.10f\n",r?sqrt(ans/r):0);
	return 0;
}
