//
// Created by Greg Postnikoff on 11.12.17.
//

#ifndef LANGUAGE_PRINTFUNCS_H
#define LANGUAGE_PRINTFUNCS_H



int FirstCompile(char * Programm, VarArr * ProgArr, FILE * file) {

    char str[StrLenth] = {};

    int VarNum = 0;
    int InFunPar = 0;

    while(!feof(file))
    {
        fscanf(file, "%s", str);


        if (strcmp(str, "}")  == 0)
            VarNum = 0;

        if (strchr(str, '('))
            InFunPar++;
        if (strchr(str, ')'))
            InFunPar--;


        if (strcmp(str, "var") == 0)
        {
            fscanf(file, "%s", str);
            strcpy(ProgArr->Var_RAM[ProgArr->Count].Name, str);
            ProgArr->Var_RAM[ProgArr->Count].RegIdex = VarNum;
            ProgArr->Count++;
            VarNum++;
            if (InFunPar)
                strcat(Programm, str);
        } else
        if (strcmp(str, "fun")  == 0)
        {
            LblCount++;
            fscanf(file, "%s", str);
            strcpy(ProgArr->Fun_Num[ProgArr->FunCount].Name, str);
            ProgArr->Fun_Num[ProgArr->FunCount].FunNumber = ProgArr->FunCount;
            if (strcmp(str, "main") == 0)
                strcat(Programm, "0");
            else {
                strcat(Programm, IntToStr(ProgArr->FunCount));
                ProgArr->FunCount++;
            }
        } else
            strcat(Programm, str);
    }

    return 0;
}

Tree * CleanTree(Tree * Node) {

    if (Node) {

        while(Node->Inside.Type == NotaDot)
            Node = Node->Left;

        if (Node->Left)
            while (Node->Left->Inside.Type == NotaDot)
                Node->Left = Node->Left->Left;
        if (Node->Right)
            while (Node->Right->Inside.Type == NotaDot)
                Node->Right = Node->Right->Left;

        if (Node->Right)
            CleanTree(Node->Right);
        if (Node->Left)
            CleanTree(Node->Left);
    }

    return Node;

}

int PrintTreeDot(const Tree const * Node) {


    if (Node) {
        if (Node->Left) {
            PrintTreeDot(Node->Left);
            printf("\"%d\\n %s\\n %d\\n %p\" -> \"%d\\n %s\\n %d\\n %p\"\n", Node->Inside.DataInt, Node->Inside.DataChar, Node->Inside.Type, Node,
                   Node->Left->Inside.DataInt, Node->Left->Inside.DataChar, Node->Left->Inside.Type, Node->Left);
        }
        if (Node->Right) {
            PrintTreeDot(Node->Right);
            printf("\"%d\\n %s\\n %d\\n %p\" -> \"%d\\n %s\\n %d\\n %p\"\n", Node->Inside.DataInt, Node->Inside.DataChar, Node->Inside.Type, Node,
                   Node->Right->Inside.DataInt, Node->Right->Inside.DataChar, Node->Right->Inside.Type, Node->Right);
        }
        if (!(Node->Right && Node->Left))
            printf("\"%d\\n %s\\n %d\\n %p\"\n", Node->Inside.DataInt, Node->Inside.DataChar, Node->Inside.Type, Node);
    }

    return 0;

}

int PrintAllCode (Tree * Prog, VarArr * ProgArr) {

    fprintf(fileOut, "JMP :0\n");
    fprintf(fileOut, "PUSH -4\n");
    fprintf(fileOut, "POP ax\n");

    PrintProg(Prog, ProgArr);

    return 0;
}

int PrintProg(Tree * Prog, VarArr * ProgArr) {

    PrintFUNCS(Prog->Left, ProgArr);
    if (Prog->Right)

        PrintProg(Prog->Right, ProgArr);


    return 0;

}

int PrintFUNCS(Tree * Prog, VarArr * ProgArr) {

    if (Prog) {

        fprintf(fileOut, ":%d\n", Prog->Inside.DataInt);

        fprintf(fileOut, "PUSH ax\n");
        fprintf(fileOut, "PUSH 4\n");
        fprintf(fileOut, "SUB\n");
        fprintf(fileOut, "POP ax\n");

        PrintParams(Prog->Right, ProgArr);

        PrintFunc(Prog->Left, ProgArr);

        if (Prog->Inside.DataInt == 0)
            fprintf(fileOut, "HOLD\n");
        else {
            fprintf(fileOut, "###FUN\n");

            fprintf(fileOut, "PUSH ax\n");
            fprintf(fileOut, "PUSH -4\n");
            fprintf(fileOut, "SUB\n");
            fprintf(fileOut, "POP ax\n");

            fprintf(fileOut, "###FUN\n\n");



            fprintf(fileOut, "RET\n");
        }
    }
    return 0;
}

int PrintFunc(Tree * Prog, VarArr * ProgArr) {

    if (Prog)
    {
        if (Prog->Left)
            PrintOp(Prog->Left, ProgArr);
        if (Prog->Right)
            if (Prog->Right->Inside.Type == aDotOp)
                PrintFunc(Prog->Right, ProgArr);
            else
                PrintOp(Prog->Right, ProgArr);
    }
    return 0;
}

int PrintOp(Tree * Prog, VarArr * ProgArr) {

    switch (Prog->Inside.Type) {
        case CONST_cond:
            PrintCond(Prog, ProgArr);
            break;
        case CONST_equal:
            PrintAssn(Prog, ProgArr);
            break;
        case CONST_loop:
            PrintLoop(Prog, ProgArr);
            break;
        case CONST_call:
            PrintCall(Prog, ProgArr);
    }

    if (Prog->Inside.Type == CONST_cond)

        if (Prog->Inside.Type == CONST_equal)
            PrintAssn(Prog, ProgArr);


    return 0;
}

int PrintCond(Tree * Prog, VarArr * ProgArr) {

    PrintNode(Prog->Left, ProgArr);
    if (Prog->Left->Inside.Type == CONST_more)
        fprintf(fileOut, "JB :%d\n", LblCount);
    else
        fprintf(fileOut, "JA :%d\n", LblCount);

    int CurLbl = LblCount;

    LblCount++;

    PrintOp(Prog->Right, ProgArr);

    fprintf(fileOut, ":%d\n", CurLbl);

    return 0;
}

int PrintNode(Tree * Prog, VarArr * ProgArr) {

    if (Prog) {
        PrintNode(Prog->Right, ProgArr);
        PrintNode(Prog->Left, ProgArr);


        switch (Prog->Inside.Type) {

            case CONST_var:
                for (int i = 0; i < ProgArr->Count; i++)
                    if (strcmp(Prog->Inside.DataChar, ProgArr->Var_RAM[i].Name) == 0) {
                        fprintf(fileOut, "###NODE\n");
                        fprintf(fileOut, "PUSH ax\n");
                        fprintf(fileOut, "PUSH %d\n", ProgArr->Var_RAM[i].RegIdex);
                        fprintf(fileOut, "SUB\n");
                        fprintf(fileOut, "POP cx\n");
                        fprintf(fileOut, "PUSH [cx]\n");
                        fprintf(fileOut, "###NODE\n\n");
                    }
                break;
            case CONST_num:
                fprintf(fileOut, "PUSH %d\n", Prog->Inside.DataInt);
                break;
            case CONST_minus:
                fprintf(fileOut, "ADD\n");
                break;
            case CONST_plus:
                fprintf(fileOut, "SUB\n");
                break;
            case CONST_mul:
                fprintf(fileOut, "MUL\n");
                break;
            case CONST_dev:
                fprintf(fileOut, "DEV\n");
                break;

        }
    }

    return 0;
}

int PrintAssn(Tree * Prog, VarArr * ProgArr) {

    PrintNode(Prog->Right, ProgArr);

    printf("bbb # %s\n", Prog->Left->Inside.DataChar);

    for (int i = 0; i < ProgArr->Count; i++) {
        printf("aaa # %s\n", ProgArr->Var_RAM[i].Name);
        if (strcmp(Prog->Left->Inside.DataChar, ProgArr->Var_RAM[i].Name) == 0) {
           fprintf(fileOut, "###ASSN\n");

            fprintf(fileOut, "PUSH ax\n");
            fprintf(fileOut, "PUSH %d\n", ProgArr->Var_RAM[i].RegIdex);
            fprintf(fileOut, "SUB\n");
            fprintf(fileOut, "POP cx\n");
            fprintf(fileOut, "POP [cx]\n");
            fprintf(fileOut, "###ASSN\n\n");

        }
    }
    return 0;
}

int PrintLoop(Tree * Prog, VarArr * ProgArr) {

    int CurLbl = LblCount;

    fprintf(fileOut, ":%d\n", CurLbl);

    PrintOp(Prog->Right, ProgArr);

    PrintNode(Prog->Left, ProgArr);
    if (Prog->Left->Inside.Type == CONST_more)
        fprintf(fileOut, "JA :%d\n", LblCount);
    else
        fprintf(fileOut, "JB :%d\n", LblCount);

    LblCount++;

    return 0;
}


Tree * GetFun() {

    Tree * GenFun = (Tree *) calloc(1, sizeof(Tree));
    GenFun->Inside.Type = aDotHlpr;

    Tree * Dot = (Tree *) calloc(1, sizeof(Tree));
    Dot->Inside.Type = CONST_fun;
    Dot->Inside.DataInt = GetN()->Inside.DataInt;

    assert(str[p] == '(');
    p++;

    while (str[p] != ')') {


        AddParam(Dot);

        assert(str[p] == ',');
        p++;
    }
    p++;

    printf("HGC\n");

    assert(str[p] == '{');
    p++;

    Dot->Left = GetS();

    assert(str[p] == '}');
    p++;

    GenFun->Left = Dot;

    printf("%s\n", &str[p]);
    if (str[p] != 0)
        GenFun->Right = GetFun();

    return GenFun;

}

char * IntToStr(int a) {

    char * str = (char *) calloc(10, sizeof(char));
    int i = 0;

    while (a != 0)
    {
        str[i++] = a % 10 + '0';
        a /= 10;
    }

    str[i] = 0;

    return str;

}

int PrintCall(Tree * Prog, VarArr * ProgArr) {

    for (int i = 0; i < ProgArr->FunCount; i++ )
    {
        printf("###3%s\n", Prog->Left->Inside.DataChar);
        if (strcmp(Prog->Inside.DataChar, ProgArr->Fun_Num[i].Name) == 0) {
            printf("###%s\n", ProgArr->Fun_Num[i].Name);
            PrintPARAMS(Prog->Right, ProgArr);
            fprintf(fileOut, "CALL :%d\n", ProgArr->Fun_Num[i].FunNumber + 1);
        }
    }
}

int AddParam(Tree * Node) {

    if (Node->Right)
        AddParam(Node->Right);
    else
        Node->Right = GetID();

    return 0;
}


int PrintParams(Tree * Prog, VarArr * ProgArr) {

    if(Prog) {
        fprintf(fileOut, "###PARAMS\n");

        fprintf(fileOut, "POP [ax]\n");
//        fprintf(fileOut, "PUSH 1\n");
//        fprintf(fileOut, "PUSH ax\n");
//        fprintf(fileOut, "SUB\n");
//        fprintf(fileOut, "POP cx\n");

        fprintf(fileOut, "###PARAMS\n\n");


        PrintParams(Prog->Right, ProgArr);
    }
    return 0;
}

int PrintPARAMS(Tree * Prog, VarArr * ProgArr) {

    if (Prog) {
        for (int i = 0; i < ProgArr->Count; i++)
            if (strcmp(Prog->Inside.DataChar, ProgArr->Var_RAM[i].Name) == 0)
            {
                fprintf(fileOut, "###CALLPAR\n");

                fprintf(fileOut, "PUSH ax\n");
                fprintf(fileOut, "PUSH %d\n", ProgArr->Var_RAM[i].RegIdex);
                fprintf(fileOut, "SUB\n");
                fprintf(fileOut, "POP cx\n");
                fprintf(fileOut, "PUSH [cx]\n");
                fprintf(fileOut, "###CALLPAR\n\n");

            }

    PrintPARAMS(Prog->Right, ProgArr);

    }
}


#endif //LANGUAGE_PRINTFUNCS_H
