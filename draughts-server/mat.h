#ifndef MAT_H
#define MAT_H

#include <QObject>
#include <QString>

extern const int dx[4];
extern const int dy[4];

class Mat
{
public:
    int m[10][10];
    Mat(int x=0){reset(x);}
    void reset(int x=0);
    void init(const QString&s);
    Mat revcol();
    Mat symm180();
    void print(QString s="");
    void s421();
    QString tostr();
/*    Mat get_all_select();
    Mat get_next_step(Mat&sel);
private:
    int bfs();*/
};

#endif // MAT_H
