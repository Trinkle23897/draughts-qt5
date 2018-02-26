#include "entergame.h"
#include "ui_entergame.h"
#include <QString>
#include <QDebug>

EnterGame::EnterGame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterGame)
{
    ui->setupUi(this);
    ui->textEdit->setText("......0.11....0.0....1....0.....0...0.....1.......");
}

EnterGame::~EnterGame()
{
    delete ui;
}

void EnterGame::on_buttonBox_accepted()
{
    QString s;
    s=ui->textEdit->toPlainText();
    qDebug()<<"enter"<<s;
    if(s.length()>=50)
        emit acc(s);
}
