#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPaintEvent>
#include <QMouseEvent>
#include "solver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    QPushButton***button;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    solver sol;
    int state;//-1: wait, 0: server, 1: client, 2: end
    //int sel_x,sel_y;//selected grid
    int rev;//0: default, 1: change b and w
//    int max_step;//-1: move, 0: err, >0: max moving step
//    Mat a,//-1: null, 0: white, 1: black, 2: white king, 3: black king
//        b,//0: null, 1: selected
//        c;//0: null, 1: next step
private:
    Mat show_a,show_b,show_c;
//    int get_sel();
//    void get_nxt();
//    void clicked(int x,int y);
//    int nomove(int x,int y);
//    int nomove2(int x,int y);
//    int nojump(int x,int y);
//    int nojump2(int x,int y);//king
//    int dfs(int x,int y,int step);
//    int dfs2(int x,int y,int step);//king
//    int aval(int i,int j){return i>=0&&i<10&&j>=0&&j<10;}
public slots:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
};

#endif // MAINWINDOW_H
