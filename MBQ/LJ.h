
#pragma once
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<random>
#include<climits>
using namespace std;

int n;
long long m;
vector<int>First,Next,ver,edge;

struct Reader
{
    FILE*f;
    char b[1<<16];
    int pos,len;
    Reader(FILE*x)
    {
        f=x;
        pos=len=0;
    }
    int get()
    {
        if(pos==len)
        {
            len=fread(b,1,sizeof(b),f);
            pos=0;
            if(!len)return EOF;
        }
        return (unsigned char)b[pos++];
    }
    bool read(int &x)
    {
        int c=get();
        while(c!=EOF)
        {
            if(c=='#')
            {
                while(c!=EOF&&c!='\n')c=get();
            }
            if(c>='0'&&c<='9')break;
            c=get();
        }
        if(c==EOF)return 0;
        x=0;
        while(c>='0'&&c<='9')
        {
            x=x*10+c-'0';
            c=get();
        }
        return 1;
    }
};

void add(int x,int y,int w)
{
    Next.push_back(First[x]);
    ver.push_back(y);
    if(w!=0)edge.push_back(w);
    First[x]=(int)ver.size()-1;
}

bool loadLJ(const char*path,int weighted)
{
    FILE*f=fopen(path,"rb");
    if(!f)
    {
        printf("Cannot open %s\n",path);
        return 0;
    }

    char s[1024];
    n=0;
    long long expected=-1;
    while(1)
    {
        long pos=ftell(f);
        if(!fgets(s,sizeof(s),f))break;
        if(s[0]!='#')
        {
            fseek(f,pos,SEEK_SET);
            break;
        }
        int nn;
        long long mm;
        if(sscanf(s,"# Nodes: %d Edges: %lld",&nn,&mm)==2)
        {
            n=nn;
            expected=mm;
        }
    }

    if(n<=0||expected<0||expected>INT_MAX)
    {
        printf("Invalid LJ header or too many edges\n");
        fclose(f);
        return 0;
    }

    First.assign(n,-1);
    Next.clear();
    ver.clear();
    edge.clear();
    Next.reserve((size_t)expected);
    ver.reserve((size_t)expected);
    if(weighted)edge.reserve((size_t)expected);

    mt19937 gen(20261104);
    uniform_int_distribution<int>wd(1,255);
    Reader r(f);
    int u,v;
    m=0;
    while(r.read(u))
    {
        if(!r.read(v)||u<0||v<0||u>=n||v>=n)
        {
            printf("Invalid edge at %lld\n",m+1);
            fclose(f);
            return 0;
        }
        int w=weighted?wd(gen):0;
        add(u,v,w);
        m++;
    }

    bool ok=!ferror(f);
    fclose(f);

    if(!ok||m!=expected)
    {
        printf("LJ edge count mismatch: %lld vs %lld\n",m,expected);
        return 0;
    }
    return 1;
}
