#include "rule.h"

int WhichLine[ ALLplace ];
int WhichColumn[ ALLplace ];
int Side_OfChess[41];//棋子编号转化为某方，-1为不属于任何一方，0,1,2为某方
int ActSideOrder[3][4] = {0,0,1,2, 0,1,1,2, 0,1,2,2};//参数为优势方、回合除余4
int BaseCampPlace[3] = {0, 28, 35};//每方大本营的位置，当被其他方到达，则该方被全歼


//在程序启动前的函数，进行一些全局数据的计算
//将格子划分为列和行，第1行有1列，第二行有2列，以此类推
//棋子编号转化为某方，-1为不属于任何一方，0,1,2为某方
void BeforeAppRun() {
    for (int place = 0, theline = 0, thecolumn = 0; place < ALLplace; place ++) {
        if (thecolumn > theline) {
            theline ++;
            thecolumn = 0;
        }
        WhichLine[ place ] = theline;
        WhichColumn[ place ] = thecolumn;
        thecolumn++;
    }
    for (int point = 0; point < 41; point ++) {
        if (point >= 10 && point % 10 <= 6) {
            Side_OfChess[ point ] = point / 10 - 1;
        }else {
            Side_OfChess[ point ] = -1;
        }
    }
}


