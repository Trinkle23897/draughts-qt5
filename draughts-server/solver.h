#ifndef SOLVER_H
#define SOLVER_H

#include <QObject>
#include "mat.h"

class solver : public QObject
{
    Q_OBJECT
public:
    int sx,sy;//selected grid
    int max_step;//-1: move, 0: err, >0: max moving step
    Mat a,//-1: null, 0: white, 1: black, 2: white king, 3: black king, 4: eated
        b,//0: null, 1: selected
        c;//0: null, 1: next step
public:
    explicit solver(QObject *parent = 0);
    int aval(int i,int j){return i>=0&&i<10&&j>=0&&j<10;}
    void reset(){
        sx=sy=-1;
        a.init("00000000000000000000..........11111111111111111111");
        b.reset(0);
        c.reset(0);
    }
    int get_sel();
    void get_nxt();
    int clicked(int x,int y);
    int nomove(int x,int y);
    int nomove2(int x,int y);//king
    int nojump(int x,int y);
    int nojump2(int x,int y);//king
private:
    Mat tmp;
    int dfs(int x,int y,int step);
    int dfs2(int x,int y,int step);//king

signals:

public slots:
};

#endif // SOLVER_H
