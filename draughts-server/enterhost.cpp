#include "enterhost.h"
#include "ui_enterhost.h"
#include <QtNetwork>

EnterHost::EnterHost(QString _,int __,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterHost),
    host(_),port(__)
{
    ui->setupUi(this);
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, AddressList){
        if(address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::Null){//&&address != QHostAddress::LocalHost
            qDebug()<<address.toString();
            ui->comboBox->addItem(address.toString());
        }
    }
    for(int i = 0;i < ui->comboBox->count();++i)
        if(ui->comboBox->itemText(i) == host)
            ui->comboBox->setCurrentIndex(i);
//    int a[4]={0},cnt=0;
//    int len=host.length();
//    for(int i=0;i<len;++i)
//        if(host[i]=='.')
//            cnt++;
//        else
//            a[cnt]=a[cnt]*10+host[i].digitValue();
//    ui->spinBox->setValue(a[0]);
//    ui->spinBox_2->setValue(a[1]);
//    ui->spinBox_3->setValue(a[2]);
//    ui->spinBox_4->setValue(a[3]);
    ui->spinBox_5->setValue(port);
}

EnterHost::~EnterHost()
{
    delete ui;
}

void EnterHost::on_buttonBox_accepted()
{
    host=ui->comboBox->currentText();
//    host=tr("%1.%2.%3.%4").arg(ui->spinBox->value()).arg(ui->spinBox_2->value()).arg(ui->spinBox_3->value()).arg(ui->spinBox_4->value());
    emit acc(host,ui->spinBox_5->value());
    close();
}

void EnterHost::on_buttonBox_rejected()
{
    emit none();
    close();
}
