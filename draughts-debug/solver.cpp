#include <QDebug>
#include "solver.h"

solver::solver(QObject *parent) : QObject(parent),a(-1),b(0),c(0)
{
    reset();
}

int solver::clicked(int x, int y)
{
    qDebug()<<sx<<sy;
    if(x==sx&&y==sy)return 0;
    if(tmp.m[x][y])
    {
        sx=x;
        sy=y;
        return 1;
    }
    else if(aval(sx,sy))
    {
        if(max_step>0&&c.m[x][y]&(1<<(max_step-1)))
        {
            max_step--;tmp.reset(0);
            int delta_x=sx<x?1:-1,delta_y=sy<y?1:-1;
            a.m[x][y]=a.m[sx][sy];
            a.m[sx][sy]=-1;
            for(;sx+=delta_x,sy+=delta_y,sx!=x||sy!=y;)
            {
                qDebug()<<sx<<sy<<x<<y;
                if(a.m[sx][sy]!=-1)
                    a.m[sx][sy]=4+a.m[sx][sy]%2;
            }
            qDebug()<<max_step;
            if(max_step==0)
            {
                if(sx==0&&a.m[sx][sy]<2)
                    a.m[sx][sy]+=2;
                sx=sy=-1;
                a.s421();
                return 2;
            }
            return 1;
        }
        else if(max_step==-1&&c.m[x][y]==1)
        {
            a.m[x][y]=a.m[sx][sy];
            if(x==0&&a.m[x][y]<2)
                a.m[x][y]+=2;
            a.m[sx][sy]=-1;
            sx=sy=-1;
            a.s421();
            return 2;
        }
    }
    return 0;
}

int solver::nomove(int x, int y)
{
    if(a.m[x][y]!=1)
        return 1;
    if((aval(x-1,y-1)&&a.m[x-1][y-1]==-1)||(aval(x-1,y+1)&&a.m[x-1][y+1]==-1))
        return 0;
    return 1;
}

int solver::nomove2(int x, int y)
{
    if(a.m[x][y]!=3)
        return 1;
    for(int k=0;k<4;++k)
        if(aval(x+dx[k],y+dy[k])&&a.m[x+dx[k]][y+dy[k]]==-1)
            return 0;
    return 1;
}

int solver::nojump(int x, int y)
{
    if(a.m[x][y]!=1)
        return 1;
    for(int k=0;k<4;++k)
        if(aval(x+dx[k]+dx[k],y+dy[k]+dy[k])
                &&a.m[x+dx[k]+dx[k]][y+dy[k]+dy[k]]==-1
                &&a.m[x+dx[k]][y+dy[k]]%2==0
                &&a.m[x+dx[k]][y+dy[k]]<4)
            return 0;
    return 1;
}

int solver::nojump2(int x, int y)
{
    if(a.m[x][y]!=3)
        return 1;
    for(int i,j,k=0;k<4;++k)
    {
        for(i=x+dx[k],j=y+dy[k];aval(i,j)&&a.m[i][j]==-1;i+=dx[k],j+=dy[k]);
        if(aval(i+dx[k],j+dy[k])&&a.m[i][j]%2==0&&a.m[i][j]<4&&a.m[i+dx[k]][j+dy[k]]==-1)
            return 0;
    }
    return 1;
}

int solver::get_sel()
{
    max_step=0;
    b.reset(0);
    c.reset(0);
    //check if satisfied rule-6
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
        {
            //qDebug()<<i<<j;
            if(a.m[i][j]==1)
                b.m[i][j]=dfs(i,j,0);
            else if(a.m[i][j]==3)
                b.m[i][j]=dfs2(i,j,0);
            if(b.m[i][j]>max_step)
                max_step=b.m[i][j];
        }
    qDebug()<<"max step"<<max_step;
    if(max_step>0)
    {
        for(int i=0;i<10;++i)
            for(int j=0;j<10;++j)
                b.m[i][j]=(b.m[i][j]==max_step);
    }
    else//rule-7
    {
        for(int i=0;i<10;++i)
            for(int j=0;j<10;++j)
                if(!nomove(i,j)||!nomove2(i,j))
                {
                    b.m[i][j]=1;
                    max_step=-1;
                }
        if(max_step==0)
            return 0;
    }
//    b.print("b");
    tmp=b;
    return max_step;
}

void solver::get_nxt()
{
    c.reset(0);
    if(!aval(sx,sy))
        return;
//    qDebug()<<max_step;
    int i=sx,j=sy;
    if(b.m[i][j])
    {
        if(a.m[i][j]==1)
        {
            if(max_step==-1)
            {
                if(aval(i-1,j-1)&&a.m[i-1][j-1]==-1)
                    c.m[i-1][j-1]|=1;
                if(aval(i-1,j+1)&&a.m[i-1][j+1]==-1)
                    c.m[i-1][j+1]|=1;
            }
            else
                dfs(i,j,max_step);
        }
        else if(a.m[i][j]==3)
        {
            if(max_step==-1)
            {
                for(int k=0;k<4;++k)
                    for(int x=i+dx[k],y=j+dy[k];aval(x,y)&&a.m[x][y]==-1;x+=dx[k],y+=dy[k])
                        c.m[x][y]|=1;
            }
            else
                dfs2(i,j,max_step);
        }
    }
//    c.print("c");
}

int solver::dfs(int x, int y, int step)
{
    if(nojump(x,y))
    {
        if(step==0)
            c.m[x][y]|=1;
        return 0;
    }
    int maxx=0;
    for(int k=0;k<4;++k)
        if(aval(x+dx[k]+dx[k],y+dy[k]+dy[k])
                &&a.m[x+dx[k]+dx[k]][y+dy[k]+dy[k]]==-1
                &&a.m[x+dx[k]][y+dy[k]]%2==0
                &&a.m[x+dx[k]][y+dy[k]]<4)
        {
            int i=x+dx[k],j=y+dy[k];
            int x2=i+dx[k],y2=j+dy[k];
            int las=a.m[i][j];
            a.m[i][j]=4+a.m[i][j]%2;
            a.m[x2][y2]=a.m[x][y];
            a.m[x][y]=-1;
            int tmp=dfs(x2,y2,step-1);
            if(maxx<tmp)
                maxx=tmp;
            if(tmp==step-1)
                c.m[x2][y2]|=1<<tmp;
            a.m[x][y]=a.m[x2][y2];
            a.m[i][j]=las;
            a.m[x2][y2]=-1;
        }
    return maxx+1;
}

int solver::dfs2(int x, int y, int step)
{
    if(nojump2(x,y))
    {
        if(step==0)
            c.m[x][y]|=1;
        return 0;
    }
    int maxx=0;
    for(int i,j,k=0,x2,y2;k<4;++k)
    {
        for(i=x+dx[k],j=y+dy[k];aval(i,j)&&a.m[i][j]==-1;i+=dx[k],j+=dy[k]);
        if(aval(i+dx[k],j+dy[k])&&a.m[i][j]%2==0&&a.m[i][j]<4)
        {
            int las=a.m[i][j];
            for(x2=i+dx[k],y2=j+dy[k];aval(x2,y2)&&a.m[x2][y2]==-1;x2+=dx[k],y2+=dy[k])
            {
                a.m[x2][y2]=a.m[x][y];
                a.m[x][y]=-1;
                a.m[i][j]=4+a.m[i][j]%2;
                int tmp=dfs2(x2,y2,step-1);
                if(maxx<tmp)
                    maxx=tmp;
                if(tmp==step-1)
                    c.m[x2][y2]|=1<<tmp;
                a.m[i][j]=las;
                a.m[x][y]=a.m[x2][y2];
                a.m[x2][y2]=-1;
            }
        }
    }
    return maxx+1;
}
