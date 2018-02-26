#ifndef ENTERGAME_H
#define ENTERGAME_H

#include <QDialog>

namespace Ui {
class EnterGame;
}

class EnterGame : public QDialog
{
    Q_OBJECT
    QString s;
public:
    explicit EnterGame(QWidget *parent = 0);
    ~EnterGame();

private:
    Ui::EnterGame *ui;
signals:
    void acc(QString);
private slots:
    void on_buttonBox_accepted();
};

#endif // ENTERGAME_H
