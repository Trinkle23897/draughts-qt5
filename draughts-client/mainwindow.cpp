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
    col=0;state=-1;
    ip="127.0.0.1";port=2333;
    //return;
    //a.init("....00.0...0.0000.100.1.001...11110.11.11..1.1....");
    //a.init("00000..0..0...00.0.00..0001.1.1111..111111...1..11");
    //a.init("000..0......3...2..................11........11...");
    //a.init("...00........00..................3...2......1..111");
    //a.init("..1.....0010...0..............0......0.....0.....3");
}

void MainWindow::calc()
{
    if(state==-1)return;
    if(!sol.aval(sol.sx,sol.sy))
    {
        if(sol.get_sel()==0&&(state==0||state==1))
        {
            qDebug()<<"state"<<state;
            state+=2;
        }
        sol.get_nxt();
    }
    else
    {
        sol.b.reset(0);
        sol.b.m[sol.sx][sol.sy]=1;
        sol.get_nxt();
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    qDebug()<<"col"<<col<<"state"<<state;
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    int h=ui->centralWidget->height(),w=ui->centralWidget->width(),d=qMin((w-20)/10.,(h-20)/11.5);
    p.setBrush(Qt::gray);
    p.drawRect(0,0,w,h);
    int sep_x=(w-d*10)/2,sep_y=(h-d*11.5)/2;
    p.translate(sep_x,sep_y+d*1.3);

//    if(rev)
//        show_a=sol.a.revcol();
//    else
//        show_a=sol.a;
//    show_b=sol.b;//show_b.print("show b");
//    show_c=sol.c;//show_c.print("show c");
    //calc show table
    if(state==1||state==3)
    {
        if(col==0)
        {
            show_a=sol.a;
        }
        else
        {
            show_a=sol.a.revcol();
        }
        show_b=sol.b;
        show_c=sol.c;
    }
    else if(state==0||state==2)
    {
        if(col==0)
        {
            show_a=sol.a.revcol().symm180();
        }
        else
        {
            show_a=sol.a.symm180();
        }
        show_b=sol.b.symm180();
        show_c=sol.c.symm180();
    }
    //draw top bar
    int cnt[4]={0};
    for(int i=0;i<10;++i)
        for(int j=0;j<10;++j)
            if(show_a.m[i][j]!=-1)
                cnt[show_a.m[i][j]]++;
    QPixmap px[6];
    if(state!=-1)
    {
        px[0].load(":/fig/bgw");//white
        px[1].load(":/fig/bgb");//black
        px[2].load(":/fig/bgwk");//white king
        px[3].load(":/fig/bgbk");//black king
        for(int i=0;i<cnt[0^col];++i)
            p.drawPixmap(i*d*.5,-d*1.3,d*.5,d*.5,px[0^col]);
        for(int i=0;i<cnt[1^col];++i)
            p.drawPixmap(i*d*.5,-d*0.8,d*.5,d*.5,px[1^col]);
        for(int i=0;i<cnt[2^col];++i)
            p.drawPixmap(10*d-i*d*.5-d*.5,-d*1.3,d*.5,d*.5,px[2^col]);
        for(int i=0;i<cnt[3^col];++i)
            p.drawPixmap(10*d-i*d*.5-d*.5,-d*0.8,d*.5,d*.5,px[3^col]);
    }
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
    if(state==-1)
    {
        for(int i=0;i<10;++i)
            for(int j=0;j<10;++j)
                p.drawPixmap(j*d,i*d,d,d,(i+j)&1?*pd:*pl);
    }
    else
    {
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
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(state==1)
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
                        calc();
                        if(c==2)
                        {
                            sol.a=sol.a.revcol().symm180();
                            state^=1;
                            calc();
                            less--;
                        }
                        send(tr("%1").arg(i*10+j));
                    }
    }
    update();
    if(state>1||less<0)
    {
        //qDebug()<<state<<"state";
        QMessageBox::information(NULL,"Client",state==2&&less>=0?"You win!":"You lose.");
//        QMessageBox::information(NULL,"",tr("client Finish game. %1").arg(state));
        state=-1;
        if(less<0)
            send("rs");
    }
}

void MainWindow::send(QString s)
{
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(s);
    this->readWriteSocket->write(array->data());
}

void MainWindow::restart(int clear)
{
    if(clear)
        sol.reset();
    less=1<<30;
//    QMessageBox::StandardButton rb = \
//    QMessageBox::question(NULL, "Question server", "You go first?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//    if(rb!=QMessageBox::Yes)
//    {
//        col=1,state=0;//server white
//        send("setb");
//    }
//    else
//    {
//        col=0,state=1;//server black
//        send("setw");
//    }
//    sol.a.init("......0.11....0.0....1....0.....0...0.....1.......");
    update();
}

void MainWindow::setip(QString _, int __)
{
    qDebug()<<_<<__;
    ip=_,port=__;
    this->readWriteSocket = new QTcpSocket;
    this->readWriteSocket->connectToHost(QHostAddress(ip),port);
    connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
    ui->statusBar->showMessage(tr("Client connected to ")+ip+tr(":%1").arg(port)+tr("."));
    restart();
}

void MainWindow::on_actionInput_host_triggered()
{
    EnterHost eh(ip,port);
    connect(&eh,SIGNAL(acc(QString,int)),this,SLOT(setip(QString,int)));
//    connect(&eh,SIGNAL(none()),this,SLOT(stopserver()));
    eh.exec();
    eh.show();
}

void MainWindow::recvMessage()
{
    QString info;
    info=this->readWriteSocket->readAll();
    qDebug()<<"Recv"<<info;
    if(info=="setb")
    {
        col=0;state=1;
        calc();
        update();
        return;
    }
    else if(info=="setw")
    {
        col=1;state=0;
        calc();
        update();
        return;
    }
    else if(info=="rs")
    {
        QMessageBox::information(NULL,"Client","You win! Server gives up.");
        state=-1;
        return;
    }
    else if(info=="qh")
    {
        QMessageBox mes(QMessageBox::Question,"Client","Server wants to make a draw, do you agree?",QMessageBox::Yes|QMessageBox::No,NULL);
        if(mes.exec()==QMessageBox::Yes)
        {
            QMessageBox::information(NULL,"Client","You agree to make a draw.");
            state=-1;
            send("qh1");
        }
        else
        {
            if(less>40)less=40;
            send("qh0");
        }
        return;
    }
    else if(info=="qh1")
    {
        QMessageBox::information(NULL,"Client","Server agrees to make a draw.");
        state=-1;
        return;
    }
    else if(info=="qh0")
    {
        QMessageBox::information(NULL,"Client","Server refuses to make a draw.");
        return;
    }
    else if(info[0]=='s')
    {
        info.remove(0,1);
        sol.a.init(info);
        calc();
        //restart();
    }
    else
    {
        int x=info.toInt()/10;
        int y=info.toInt()%10;
        qDebug()<<"recv "<<x<<y;
        calc();
        int c;
        if(c=sol.clicked(x,y))
        {
            QSound *sound=new QSound(":/fig/click",this);
            sound->play();
            calc();
            if(c==2)
            {
                sol.a=sol.a.revcol().symm180();
                calc();
                state^=1;
                less--;
                qDebug()<<"next step";
            }
        }
        if(state>1||less<0)
        {
            //qDebug()<<state<<"state";
            QMessageBox::information(NULL,"Client",state==2&&less>=0?"You win!":"You lose.");
//            QMessageBox::information(NULL,"Client",tr("client Finish game. %1").arg(state));
            state=-1;
            if(less<0)
                send("rs");
        }
    }
    update();
}

void MainWindow::on_actionGive_up_triggered()
{
    if(state==-1)
        return;
    send("rs");
    QMessageBox::information(NULL,"Client","You lose.");
    state=-1;
}

void MainWindow::on_actionMake_a_draw_triggered()
{
    if(state==-1)
        return;
    send("qh");
    QMessageBox::information(NULL,"Client","Your request have been sent to server.");
}
