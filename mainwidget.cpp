#include "mainwidget.h"

//像素单位坐标
int CoordinateX_Base = 400;//基础坐标
int CoordinateY_Base = 650;
int CoordinateX_OfPlace [ ALLplace ];//位置转化为x坐标
int CoordinateY_OfPlace [ ALLplace ];//位置转化为y坐标
int Length_CellX = 80;//格子长度
int Length_CellY = 72;
int Size_CellX[ 6 ] = { 0, -40, -40, 0, 40, 40 };//每个六边形组成点与中心点相对坐标
int Size_CellY[ 6 ] = { 48, 24, -24, -48, -24, 24 };
int Size_chessimg = 64;

//按钮像素尺寸位置坐标
const int ButtonXlength = 125;//按钮间隔横长
const int ButtonYlength = 80;//按钮间隔竖长
const int ButtonXline = 800;//按钮阵列起点横坐标
const int ButtonYline = 100;//按钮阵列起点纵坐标
const int ButtonXsize = 100;//按钮横尺寸
const int ButtonYsize = 60;//按钮竖尺寸

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    //计算各格子的中心坐标
    //赋予每个六边形格子组成点坐标
    for (int place = 0; place < ALLplace; place ++) {
        CoordinateX_OfPlace[ place ] = CoordinateX_Base - Length_CellX / 2 * WhichLine[ place ] + Length_CellX * WhichColumn[ place ];
        CoordinateY_OfPlace[ place ] = CoordinateY_Base - Length_CellY * WhichLine[ place ];
        for (int order = 0; order < 6; order ++) {
            PointF_ChessBoard[ place ][ order ].setX(CoordinateX_OfPlace[ place ] + Size_CellX[ order ]);
            PointF_ChessBoard[ place ][ order ].setY(CoordinateY_OfPlace[ place ] + Size_CellY[ order ]);
        }
    }

    //窗口设置
    resize( 1200, 900 );

    //按钮
    Button_SwichControl_N.setParent(this);
    Button_SwichControl_N.setText(" 结束布局");
    Button_SwichControl_N.move(ButtonXline + ButtonXlength * 0, ButtonYline + ButtonYlength * 1);
    Button_SwichControl_N.resize(ButtonXsize, ButtonYsize);


    //棋子图片
    pixmap_Chessimg[0].load(":/chessimg/img/0.png");

    pixmap_Chessimg[10].load(":/chessimg/img/10.png");
    pixmap_Chessimg[11].load(":/chessimg/img/11.png");
    pixmap_Chessimg[12].load(":/chessimg/img/12.png");
    pixmap_Chessimg[13].load(":/chessimg/img/13.png");
    pixmap_Chessimg[14].load(":/chessimg/img/14.png");
    pixmap_Chessimg[15].load(":/chessimg/img/15.png");
    pixmap_Chessimg[16].load(":/chessimg/img/16.png");

    pixmap_Chessimg[20].load(":/chessimg/img/20.png");
    pixmap_Chessimg[21].load(":/chessimg/img/21.png");
    pixmap_Chessimg[22].load(":/chessimg/img/22.png");
    pixmap_Chessimg[23].load(":/chessimg/img/23.png");
    pixmap_Chessimg[24].load(":/chessimg/img/24.png");
    pixmap_Chessimg[25].load(":/chessimg/img/25.png");
    pixmap_Chessimg[26].load(":/chessimg/img/26.png");

    pixmap_Chessimg[30].load(":/chessimg/img/30.png");
    pixmap_Chessimg[31].load(":/chessimg/img/31.png");
    pixmap_Chessimg[32].load(":/chessimg/img/32.png");
    pixmap_Chessimg[33].load(":/chessimg/img/33.png");
    pixmap_Chessimg[34].load(":/chessimg/img/34.png");
    pixmap_Chessimg[35].load(":/chessimg/img/35.png");
    pixmap_Chessimg[36].load(":/chessimg/img/36.png");

    //棋子位置框
    //把位置框放在中心点的左上角，偏移量是棋子图片的一半尺寸
    for (int place = 0; place < ALLplace; place ++) {
        Label_Chessimg[ place ].setParent( this );
        Label_Chessimg[ place ].move( CoordinateX_OfPlace[ place ] - Size_chessimg / 2, CoordinateY_OfPlace[ place ] - Size_chessimg / 2 );
    }

    //随机数框
    Label_RndPoint.setParent( this );
    Label_RndPoint.move(ButtonXline + ButtonXlength * 1 + ButtonXsize / 2 - Size_chessimg / 2, ButtonYline + ButtonYlength * 0 + ButtonYsize / 2 - Size_chessimg / 2);
    Label_RndPoint.setPixmap( pixmap_Chessimg[10] );

    //信号槽
    for(int place = 0; place < ALLplace; place++){//建立每个棋位的连接
        connect( &Label_Chessimg[ place ], SIGNAL( clicked() ), this, SLOT( Do_LabelClick() ) );
    }

}






void MainWidget::Do_LabelClick(){//检查是哪个棋位被点击了
    for (int place = 0; place < ALLplace; place++) {
        if((Clickedlabel*) sender() == &Label_Chessimg[place])
        {
            emit signals_LabelClick( place );
            break;
        }
    }
}

void MainWidget::Refurbish(){//刷新显示
    for(int place = 0; place < ALLplace; place ++){
        if(Takeup_place == place){//如果是提起棋子的位置，显示上做个标记
            Label_Chessimg[ place ].setPixmap( pixmap_Chessimg[ 0 ] );//用空白做提子标记
        }else{
            Label_Chessimg[ place ].setPixmap( pixmap_Chessimg[ ViewChess_OfPlace[ place ] ] );//正常显示棋子
        }
    }
}

void MainWidget::RefurPoint(int point) {
    Label_RndPoint.setPixmap( pixmap_Chessimg[point] );

}

//重写的画棋盘事件
void MainWidget::paintEvent(QPaintEvent *) {
    QPainter painter_chessboard( this );
    //抗锯齿
    painter_chessboard.setRenderHint( QPainter::Antialiasing, true );

    QBrush brush_chessboard( QColor( 200, 200, 255 ), Qt::SolidPattern );
    painter_chessboard.setBrush(brush_chessboard);

    for (int place = 0; place < ALLplace; place ++) {
        painter_chessboard.drawPolygon(PointF_ChessBoard[ place ], 6);
    }
}

MainWidget::~MainWidget()
{
}

