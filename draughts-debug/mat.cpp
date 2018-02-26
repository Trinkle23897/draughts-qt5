#include "mat.h"
#include <QDebug>

const int dx[4]={-1,-1,1,1};
const int dy[4]={-1,1,-1,1};

void Mat::reset(int x)
{
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            m[i][j]=x;
}

void Mat::init(const QString&s)
{
    int cnt=0;
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            if((i+j)&1)
            {
                if(s[cnt]=='.')
                    m[i][j]=-1;
                else m[i][j]=s[cnt].digitValue();
                cnt++;
            }
            else
                m[i][j]=-1;
}

Mat Mat::revcol()
{
    Mat c;
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            if(m[i][j]==-1)
                c.m[i][j]=-1;
            else
                c.m[i][j]=m[i][j]^1;
    return c;
}

Mat Mat::symm180()
{
    Mat c;
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            c.m[i][j]=m[9-i][9-j];
    return c;
}

void Mat::print(QString s)
{
    qDebug()<<s;
    for(int i=0;i<10;++i)
        qDebug()<<m[i][0]<<m[i][1]<<m[i][2]<<m[i][3]<<m[i][4]
                <<m[i][5]<<m[i][6]<<m[i][7]<<m[i][8]<<m[i][9];
}

void Mat::s421()
{
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            if(m[i][j]>=4)
                m[i][j]=-1;
}
