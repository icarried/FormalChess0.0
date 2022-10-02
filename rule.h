#ifndef RULE_H
#define RULE_H

const int ALLplace = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8;
extern int WhichLine[ ALLplace ];
extern int WhichColumn[ ALLplace ];
extern int Side_OfChess[41];
extern int ActSideOrder[3][4];
extern int BaseCampPlace[3];

//顺序执行
extern void BeforeAppRun();

//数据转换
//extern int LineColumnToPlace( int line, int column);//还没用到

#endif // RULE_H
