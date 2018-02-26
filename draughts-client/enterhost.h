#ifndef ENTERHOST_H
#define ENTERHOST_H

#include <QDialog>
#include <QString>

namespace Ui {
class EnterHost;
}

class EnterHost : public QDialog
{
    Q_OBJECT
private:
    Ui::EnterHost *ui;
    QString host;int port;
public:
    explicit EnterHost(QString _,int __,QWidget *parent = 0);
    ~EnterHost();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void acc(QString,int);
    void none();
};

#endif // ENTERHOST_H
