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
int n,m,dir,s,t;
vector<int>First,Next,ver,dis,q;
void add(int x,int y)
{
	ver.push_back(y);
	Next.push_back(First[x]);
	First[x]=ver.size()-1;
}
void bfs()
{
	dis.assign(n,-1);
	q.resize(n);
	int head=0,tail=0;
	dis[s]=0;
	q[tail++]=s;
	while(head<tail)
	{
		int x=q[head++];
		for(int i=First[x];~i;i=Next[i])
		{
			int y=ver[i];
			if(dis[y]!=-1)continue;
			dis[y]=dis[x]+1;
			q[tail++]=y;
		}
	}
}
int main ()
{
	n=read(),m=read(),dir=read();
	if(n<=0||m<0)return 0;
	First.assign(n,-1);
	for(int i=1;i<=m;i++)
	{
		int x=read(),y=read();
		add(x,y);
		if(!dir)add(y,x);
	}
	s=read(),t=read();
	bfs();
	printf("%d\n",dis[t]);
	return 0;
}
