#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QPointF>
#include <QMessageBox>
#include<QPushButton>

#include "rule.h"
#include "clickedlabel.h"
//#include "status.h"
#include "player.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    int Takeup_place;
    int ViewChess_OfPlace[ ALLplace ] = { 0 };//棋盘上可见各位置的棋子

    void Refurbish();//刷新棋盘显示
    void RefurPoint(int);//刷新点数显示,参数为显示的棋子

    QPushButton Button_SwichControl_N;
    Clickedlabel Label_RndPoint;//随机数显示
    Clickedlabel Label_Chessimg[ ALLplace ];//棋子位置框
    QPixmap pixmap_Chessimg[ 37 ];//棋子图片，只用其中一部分

signals:
    void signals_LabelClick( int place );

public slots:
    void Do_LabelClick();//棋盘位置点击

protected:
    void paintEvent(QPaintEvent *) override;


private:
    QPointF PointF_ChessBoard[ ALLplace ][ 6 ];//6边形的点



};
#endif // MAINWIDGET_H
