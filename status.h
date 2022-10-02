#ifndef STATUS_H
#define STATUS_H

#include <QObject>
#include<QtGlobal>
#include<QTime>

#include "rule.h"
#include "mainwidget.h"

class Status : public QObject
{
    Q_OBJECT
public:
    explicit Status(QObject *parent = nullptr);


signals:

private slots:
    void Do_GetLabelClick(int place);
    void Do_SwichControl_N();
    void Do_ProduceRandom();

protected:
    int Layout_Status = -2;//布局状态，-2为交换棋子阶段，-1为交换棋子提起，0为行棋阶段，1为提起棋子
    int Round = 0;//回合,4回合为1轮，每一轮优势方额外进行一回合。天时为摇点数的运气，地利为棋子的位置，人和为棋子良好结构
    int Rnd_color = 2;//随机数颜色，0为黑，1为灰，2为白
    int Rnd_Point = 0;//随机数点数，1~6
    int ActSide = 0;//行动方，0为黑方，1为灰方，2为白方
    int AdvantageSide = 0;//开局默认优势方为先手方
    int SurvivalChessNumb[3] = {6, 6, 6};//每方存活棋子数

private:
    int Chess_OfPlace[ ALLplace ] = { 0 };//棋盘上各位置的棋子，要保护的数据
    int Takeup_Place = -1;//拿起棋子的位置,-1代表没有提起，棋子数据也要保护
    int Takeup_Chess = -1;//拿起棋子的编号,-1代表没有提起


    MainWidget theMainWidget;
    Player player[ 3 ];//定义三个玩家

    void ChessMoveTo(int place);
    void NextRound();
    void UpdateBoard(int side);//刷新给某方的棋盘，参数为某方0,1,2
    void LayoutOfPlace(int chess_ofplace[] );
};

#endif // STATUS_H
