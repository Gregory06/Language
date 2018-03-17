#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define Oper(name, sym, numb) \
    CONST_##name = numb,

enum {

#include "Oper.h"

    StrLenth = 50,
    NotaDot = -666,
    IfLen = 2,
    WhileLen = 5,
    funLen = 3,
    CallLen = 4,
    ProgLen = 4096,
    RAMSize = 128,
    MAXfunCount = 20,
    aDotOp = 200,
    aDotFun =201,
    aDotHlpr

};

#undef Oper

const char * str = NULL;
int p = 0, LblCount = 0;
FILE * fileOut = NULL;

struct Content {

    int Type = 0;

    int DataInt = 0;

    char DataChar[StrLenth] = {};

};


struct Tree {

    Tree * Left = NULL;

    Tree * Right = NULL;

    Content Inside;

};

struct VarNames {

    char Name[StrLenth] = {};

    int RegIdex = 0;

};

struct FunNames {

    char Name[StrLenth] = {};

    int FunNumber = 0;

};

struct VarArr {

    VarNames * Var_RAM = NULL;

    FunNames * Fun_Num = NULL;

    int Count = 0;

    int FunCount = 1;

    int LblCount = 0;
};

#include "FuncNames.h"

int main() {

    FILE * file = NULL;
    file = fopen("/Users/MyMac/CLionProjects/TopProgramming/Language/1.txt", "r");

    char Programm[ProgLen] = {};

    VarArr ProgArr = {};

    ProgArr.Var_RAM = (VarNames *) calloc(RAMSize, sizeof(VarNames));
    ProgArr.Fun_Num = (FunNames *) calloc(MAXfunCount, sizeof(FunNames));

    FirstCompile(Programm, &ProgArr, file);

    for(int i =0; i < ProgArr.FunCount; i++)
        printf("fff%d %s\n",ProgArr.Fun_Num[i].FunNumber, ProgArr.Fun_Num[i].Name );

    for (int i  =0; i < ProgArr.Count; i++)
        printf("%d %s\n", ProgArr.Var_RAM[i].RegIdex, ProgArr.Var_RAM[i].Name);

    printf("%s\n", Programm);

    Tree * Prog = GetG0(Programm);
    
    Prog = CleanTree(Prog);

    fileOut = fopen("/Users/MyMac/CLionProjects/TopProgramming/ComileLite/1.txt", "w");

    PrintAllCode(Prog, &ProgArr);

    PrintTreeDot(Prog);

    return 0;
}



#include "GrammarFuncs.h"

#include "PrintFuncs.h"