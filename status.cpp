#include "status.h"

Status::Status(QObject *parent) : QObject(parent)
{

    //信号槽
    connect(&theMainWidget, SIGNAL(signals_LabelClick( int )), this, SLOT(Do_GetLabelClick( int )));
    connect(&theMainWidget.Button_SwichControl_N, &QPushButton::pressed, this, &Status::Do_SwichControl_N);
    connect( &theMainWidget.Label_RndPoint, SIGNAL( clicked() ), this, SLOT( Do_ProduceRandom() ) );

    //布局
    LayoutOfPlace( Chess_OfPlace );
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//随机数初始化

    //更新黑方棋盘
    UpdateBoard(0);

    //完成预先操作，等待响应
    theMainWidget.show();
}

void Status::Do_GetLabelClick(int place) {
    switch (Layout_Status) {
    case -2://交换棋子阶段
        if(Chess_OfPlace[place]){//如果该棋位不为0，即有棋子
            Layout_Status = -1;//转为提起状态
            Takeup_Place = place;//记下提起位置
            Takeup_Chess = Chess_OfPlace[place];//记下提起棋子
            UpdateBoard(ActSide);//刷新显示
        }
        break;
    case -1://交换棋子提起
        if(Chess_OfPlace[place]){//如果该棋位不为0，即有棋子
            Layout_Status = -2;//转为未提状态
            Chess_OfPlace[Takeup_Place] = Chess_OfPlace[place];//交换棋子位置
            Chess_OfPlace[place] = Takeup_Chess;//交换棋子位置
            Takeup_Chess = -1;//变回未提状态
            Takeup_Place = -1;//变回未提状态
            UpdateBoard(ActSide);//刷新显示
        }
        break;
    case 0://行棋阶段
        /*
        if (Chess_OfPlace[place]) {//如果已游戏结束，则不能移动# && IsGameover()
            QMessageBox::information(NULL, "提子不合法", "对局已结束");
        }
        else
        */
        if(Chess_OfPlace[place]){//如果该棋位不为0，即有棋子,且须提子合法# && IsLegalTakeup(Chess_OfPlace[place]) == 1
            Layout_Status = 1;//转为提起状态
            Takeup_Place = place;//记下提起位置
            Takeup_Chess = Chess_OfPlace[place];//记下提起棋子
            UpdateBoard(ActSide);//刷新显示
        }
        break;
    case 1://棋子提起
        if(place !=Takeup_Place){//如果不是放回原位，则进行记录# && IsLegalMove(Chess_OfPlace[Takeup_Place], place) == 1
            //%如果不是放回原位，则进行记录
            ChessMoveTo(place);
            /*
            if (GameModel == 2 && (ControlSide == 1 || ControlSide == 0) && !IsGameover()) {//试玩模式中帮程序掷骰子走子一气呵成
                Do_ProduceRandom();//掷骰子
                Do_AIconrolMove();//走子
            }
            */
        }else{//如果是放回原位，不需要推移回合(落子不合法也放回原位)
            Layout_Status = 0;//转为未提状态
            Takeup_Chess = -1;//变回未提状态
            Takeup_Place = -1;//变回未提状态
            UpdateBoard(ActSide);//刷新显示
        }
        break;
    default:
        break;
    }
}

//移动已提起的棋子
void Status::ChessMoveTo(int place) {
    Layout_Status = 0;//转为未提状态
    Chess_OfPlace[Takeup_Place] = 0;//从原位置离开
    if( Side_OfChess[ Chess_OfPlace[ place ] ] != -1) {
         SurvivalChessNumb[ Side_OfChess[ Chess_OfPlace[ place ] ] ] --;//被吃则该方存活棋子数-1
    }
    Chess_OfPlace[place] = Takeup_Chess;//到新的棋子位置,同时也替换掉了原棋子，相当于“吃”
    for (int side = 0; side < 3; side ++) {
        //如果某方棋子的大本营被到达，则该方被全歼
        if( side !=  Side_OfChess[ Chess_OfPlace[ BaseCampPlace[ side ] ] ] && Chess_OfPlace[ BaseCampPlace[ side ] ] != 0 ) {
            for(int place = 0; place < ALLplace; place ++) {
                //消灭所有该方棋子
                if( Side_OfChess[ Chess_OfPlace[ place ] ] == side) {
                    Chess_OfPlace[ place ] = 0;
                }
            }
            SurvivalChessNumb[ side ] = 0;//棋子数归零
        }
    }

    Takeup_Chess = -1;//变回未提状态
    Takeup_Place = -1;//变回未提状态
    NextRound();//到下一回合
    UpdateBoard( ActSide );
}

//回合推移
void Status::NextRound() {
    Round++;
    //每轮更新优势方,优势方为棋子最多的一方，在棋子数多个相同最多的情况下为上轮优势方顺数
    if( Round % 4 == 0 ) {
        int side = AdvantageSide;
        int theadvantageside = AdvantageSide;
        do {
            if (SurvivalChessNumb[ side ] > SurvivalChessNumb[ theadvantageside ] ) {
                theadvantageside = side;
            }
            side ++;
            if (side > 2) {
                side = 0;//从优势方循环起的顺数
            }
        } while ( side != AdvantageSide);
        AdvantageSide = theadvantageside;//更新优势方
    }
    ActSide = ActSideOrder[ AdvantageSide ][ Round % 4 ];//更新行动方
    Do_ProduceRandom();//#自动更新
    if ( SurvivalChessNumb[ ActSide ] == 0) {
        NextRound();//如果某方被全歼，则直接推进一回合
    }
}

//刷新局面,只提供某方的编号信息，当数值不为任何一方（如-1），则显示为无编号
void Status::UpdateBoard(int side) {
    for(int place = 0; place < ALLplace; place ++) {
        if( Side_OfChess[ Chess_OfPlace[ place ] ] == side) {
            theMainWidget.ViewChess_OfPlace[ place ] = Chess_OfPlace[ place ];//该方棋子对该方可见
        }
        else if( Chess_OfPlace[ place ] != 0 ) {
            theMainWidget.ViewChess_OfPlace[ place ] = ( Side_OfChess[ Chess_OfPlace[ place ] ] + 1 ) * 10;//不为该方的棋子显示为无编号棋子
        }else {
            theMainWidget.ViewChess_OfPlace[ place ] = 0;//其他情况显示为空白
        }
    }
    theMainWidget.Takeup_place = Takeup_Place;
    theMainWidget.Refurbish();
}

void Status::Do_ProduceRandom(){
    Rnd_Point = qrand() % 6 + 1;//设置随机数点数
    theMainWidget.RefurPoint( ( ActSide + 1 ) * 10 + Rnd_Point );//刷新随机数颜色显示
}


void Status::LayoutOfPlace(int chess_ofplace[] ) {
    chess_ofplace[ 28 ] = 21, chess_ofplace[ 29 ] = 22, chess_ofplace[ 30 ] = 24, chess_ofplace[ 33 ] = 36, chess_ofplace[ 34 ] = 33, chess_ofplace[ 35 ] = 31;

                chess_ofplace[ 21 ] = 23, chess_ofplace[ 22 ] = 25,                         chess_ofplace[ 26 ] = 35, chess_ofplace[ 27 ] = 32;

                            chess_ofplace[ 15 ] = 26,                                                   chess_ofplace[ 20 ] = 34;

                                        chess_ofplace[ 3  ] = 14, chess_ofplace[ 4  ] = 15, chess_ofplace[ 5  ] = 16;

                                                    chess_ofplace[ 1  ] = 12, chess_ofplace[ 2  ] = 13;

                                                                chess_ofplace[ 0  ] = 11;
}

void Status::Do_SwichControl_N()
{
    Layout_Status = 0;
    theMainWidget.Button_SwichControl_N.hide();
    Do_ProduceRandom();//#自动更新
    //EndLayout();//结束布局，对局正式开始前的设置
}


