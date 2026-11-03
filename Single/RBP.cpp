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
int n,m,limit,cnt;
double J,h,eps;
vector<vector<int>>g;
vector<int>src,dst,rev,ver;
vector<double>msg,sum;
struct fun
{
	double p;
	int id,v;
	bool operator<(fun a)const
	{
		return p<a.p;
	}
};
priority_queue<fun>q;
void add(int x,int y)
{
	int a=src.size();
	src.push_back(x),dst.push_back(y),rev.push_back(a+1);
	src.push_back(y),dst.push_back(x),rev.push_back(a);
	g[x].push_back(a),g[y].push_back(a+1);
}
double calc(int i)
{
	double x=msg[rev[i]];
	if(x>0.999999999999)x=0.999999999999;
	if(x<-0.999999999999)x=-0.999999999999;
	return tanh(J)*tanh(sum[src[i]]-atanh(x));
}
void rbp()
{
	for(int i=0;i<(int)src.size();i++)
	{
		double p=fabs(calc(i));
		if(p>eps)q.push({p,i,ver[i]});
	}
	while(!q.empty()&&cnt<limit)
	{
		fun a=q.top();
		q.pop();
		int i=a.id;
		if(a.v!=ver[i])continue;
		double x=calc(i),old=msg[i];
		if(fabs(x-old)<=eps)continue;
		int y=dst[i];
		double p=x,z=old;
		if(p>0.999999999999)p=0.999999999999;
		if(p<-0.999999999999)p=-0.999999999999;
		if(z>0.999999999999)z=0.999999999999;
		if(z<-0.999999999999)z=-0.999999999999;
		sum[y]+=atanh(p)-atanh(z);
		msg[i]=x;
		cnt++;
		for(int j=0;j<(int)g[y].size();j++)
		{
			int id=g[y][j];
			if(id==rev[i])continue;
			ver[id]++;
			double p=fabs(calc(id)-msg[id]);
			if(p>eps)q.push({p,id,ver[id]});
		}
	}
}
int main ()
{
	n=read(),m=read();
	scanf("%lf%lf",&J,&h);
	limit=read();
	scanf("%lf",&eps);
	if(n<=0||m<=0||limit<0||eps<=0)return 0;
	g.resize(n*m),sum.assign(n*m,h);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			int x=i*m+j;
			if(j+1<m)add(x,x+1);
			if(i+1<n)add(x,x+m);
		}
	}
	msg.assign(src.size(),0),ver.assign(src.size(),0);
	rbp();
	double ans=0;
	for(int i=0;i<n*m;i++)ans+=fabs(tanh(sum[i]));
	printf("%d %.10f\n",cnt,ans/(n*m));
	return 0;
}
