#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSignalMapper>
#include <QDebug>
#include <QGridLayout>
#include <QPainter>
#include <QPixmap>
#include <QSound>
#include <QtAlgorithms>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/fig/icon"));
    rev=0;state=0;
    sol.reset();
    sol.a.init("......0.11....0.0....1....0.....0...0.....1.......");
    update();
    //return;
    //a.init("....00.0...0.0000.100.1.001...11110.11.11..1.1....");
    //a.init("00000..0..0...00.0.00..0001.1.1111..111111...1..11");
    //a.init("000..0......3...2..................11........11...");
    //a.init("...00........00..................3...2......1..111");
    //a.init("..1.....0010...0..............0......0.....0.....3");
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    int h=ui->centralWidget->height(),w=ui->centralWidget->width(),d=qMin((w-20)/10.,(h-20)/11.5);
    p.setBrush(Qt::gray);
    p.drawRect(0,0,w,h);
    int sep_x=(w-d*10)/2,sep_y=(h-d*11.5)/2;
    p.translate(sep_x,sep_y+d*1.3);
    //calc hint
    if(!sol.aval(sol.sx,sol.sy))
    {
        if(sol.get_sel()==0&&(state==0||state==1))
            state=2;
        sol.get_nxt();
    }
    else
    {
        sol.b.reset(0);
        sol.b.m[sol.sx][sol.sy]=1;
        sol.get_nxt();
    }
    if(rev)
        show_a=sol.a.revcol();
    else
        show_a=sol.a;
    show_b=sol.b;//show_b.print("show b");
    show_c=sol.c;//show_c.print("show c");

    //draw top bar
    int cnt[4]={0};
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            if(show_a.m[i][j]!=-1)
                cnt[show_a.m[i][j]^rev]++;
    QPixmap px[6];
    px[0].load(":/fig/bgw");
    px[1].load(":/fig/bgb");
    px[2].load(":/fig/bgwk");
    px[3].load(":/fig/bgbk");
    for(int i=0;i<cnt[0];++i)
        p.drawPixmap(i*d*.5,-d*1.3,d*.5,d*.5,px[0^rev]);
    for(int i=0;i<cnt[1];++i)
        p.drawPixmap(i*d*.5,-d*0.8,d*.5,d*.5,px[1^rev]);
    for(int i=0;i<cnt[2];++i)
        p.drawPixmap(10*d-i*d*.5-d*.5,-d*1.3,d*.5,d*.5,px[2^rev]);
    for(int i=0;i<cnt[3];++i)
        p.drawPixmap(10*d-i*d*.5-d*.5,-d*0.8,d*.5,d*.5,px[3^rev]);

    //draw boundary
    QPen mp;
    mp.setStyle(Qt::SolidLine);
    mp.setWidth(8);
    mp.setColor(Qt::black);
    p.setPen(mp);
    p.drawLine(-1,-1,10*d,-1);
    p.drawLine(-1,-1,-1,10*d);
    p.drawLine(-1,10*d,10*d,10*d);
    p.drawLine(10*d,-1,10*d,10*d);
    //draw chess
    QPixmap*pl=new QPixmap;
    QPixmap*pd=new QPixmap;
    pl->load(":/fig/lb");
    pd->load(":/fig/db");
    px[0].load(":/fig/cw");
    px[1].load(":/fig/cb");
    px[2].load(":/fig/cwk");
    px[3].load(":/fig/cbk");
    px[4].load(":/fig/cwg");
    px[5].load(":/fig/cbg");
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            if(show_a.m[i][j]==-1)
                p.drawPixmap(j*d,i*d,d,d,(i+j)&1?*pd:*pl);
            else
                p.drawPixmap(j*d,i*d,d,d,px[show_a.m[i][j]]);
    int r=2;
    p.setBrush(Qt::NoBrush);
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            if(show_c.m[i][j])
            {
                mp.setWidth(2);
                mp.setColor(Qt::blue);
                p.setPen(mp);
                p.drawRect(j*d+r,i*d+r,d-r-r,d-r-r);
            }
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            if(show_b.m[i][j])
            {
                mp.setWidth(2);
                mp.setColor(Qt::green);
                p.setPen(mp);
                p.drawRect(j*d+r,i*d+r,d-r-r,d-r-r);
            }
    if(state>1)
    {
        QMessageBox::information(NULL,"","Finish game.");
        state=-1;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(state==0||state==1)
    {
        int h=ui->centralWidget->height(),w=ui->centralWidget->width(),d=qMin((w-20)/10.,(h-20)/11.5);
        int sep_x=(w-d*10)/2,sep_y=(h-d*11.5)/2+d*1.3;
        int x=e->pos().x()-sep_x,y=e->pos().y()-sep_y;
        for(int c,i=0;i<10;++i)
            if(i*d<=y&&y<(i+1)*d)
                for(int j=0;j<10;++j)
                    if((i+j)&1&&j*d<=x&&x<(j+1)*d&&(c=sol.clicked(i,j)))
                    {
                        QSound *sound=new QSound(":/fig/click",this);
                        sound->play();
                        update();
                        if(c==2)
                        {
                            sol.a=sol.a.revcol();
                            sol.a=sol.a.symm180();
                            rev^=1;
                        }
                    }
    }
}
