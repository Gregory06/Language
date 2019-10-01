//
// Created by Greg Postnikoff on 10.12.17.
//

#ifndef LANGUAGE_GRAMMAR_H
#define LANGUAGE_GRAMMAR_H


Tree * GetN()
{
    int val = 0;
    while (str[p] >= '0' && str[p] <= '9')
    {
        val = val * 10 + str[p] - '0';
        p++;
    }

    Tree * Val = (Tree *) calloc(1, sizeof(Tree));

    Val->Right = Val->Left = NULL;

    Val->Inside.Type = CONST_num;
    Val->Inside.DataInt = val;

    return Val;

}

Tree * GetID() {

    char var[StrLenth] = {};
    int i = 0;
    while (isalpha(str[p]))
    {
        var[i] = str[p];
        p++;
        i++;
    }

    Tree * Val = (Tree *) calloc(1, sizeof(Tree));

    Val->Right = Val->Left = NULL;

    Val->Inside.Type = CONST_var;
    strcpy(Val->Inside.DataChar,var);

    return Val;
}

Tree * GetReg() {

    Tree * Val = (Tree *) calloc(1, sizeof(Tree));
    if (str[p] == '[')
    {
        p++;
        Val = GetN();
        Val->Inside.Type = CONST_reg;
        assert(str[p] == ']');
        p++;
        return Val;
    }

}

Tree * GetP()
{
    Tree * Val = (Tree *) calloc(1, sizeof(Tree));

    if (str[p] == '(')
    {
        p++;
        Val = GetE();
        assert(str[p] == ')');
        p++;
        return Val;
    }
    else
    {
        int check = p;
        Val = GetN();
        if (check == p)
        {
            Val = GetID();

            if (check == p)
                return GetReg();
            else
                return Val;
        }
        else
            return Val;

    }
}

Tree * GetT()
{
    Tree * Dot = (Tree *) calloc(1, sizeof(Tree));
    Dot->Inside.Type = NotaDot;
    Dot->Left = GetP();

    while (str[p] == '*' || str[p] == '/')
    {
        int Oper = str[p];
        p++;
        if (Dot->Right)
        {
            Tree * Dot1 = (Tree *) calloc(1, sizeof(Tree));
            Dot1->Left = Dot->Right;
            Dot1->Right = GetP();
            Dot->Right = Dot1;
            if (Oper == '*')
                Dot1->Inside.Type = CONST_mul;
            else
                Dot1->Inside.Type = CONST_dev;
        } else
        {
            Dot->Right = GetP();
            if (Oper == '*')
                Dot->Inside.Type = CONST_mul;
            else
                Dot->Inside.Type = CONST_dev;
        }
    }

    return Dot;

}

Tree * GetE()
{
    Tree * Dot = (Tree *) calloc(1, sizeof(Tree));
    Dot->Left = GetT();
    Dot->Inside.Type = NotaDot;

    while ((str[p] == '+' || str[p] == '-') ||
           (str[p] == '<' || str[p] == '>'))
    {
        int Oper = str[p];
        p++;
        if (Dot->Right)
        {
            Tree * Dot1 = (Tree *) calloc(1, sizeof(Tree));
            Dot1->Left = Dot->Right;
            Dot1->Right = GetT();
            Dot->Right = Dot1;
            if (Oper == '+')
                Dot1->Inside.Type = CONST_plus;
            else
                Dot1->Inside.Type = CONST_minus;
        } else
        {
            Dot->Right = GetT();
            if (Oper == '+')
                Dot->Inside.Type = CONST_plus;
            else
            if (Oper == '-')
                Dot->Inside.Type = CONST_minus;
            if (Oper == '>')
                Dot->Inside.Type = CONST_more;
            else
            if (Oper == '<')
                Dot->Inside.Type = CONST_less;
        }
    }

    return Dot;

}

Tree * GetAssn() {

    Tree * Dot = (Tree *) calloc(1, sizeof(Tree));

    Dot->Left = GetID();

    assert(str[p] == '=');

    p++;

    Dot->Right = GetE();

    Dot->Inside.Type = CONST_equal;

    return Dot;

}

Tree * GetLoop() {

    Tree * Dot = (Tree *) calloc(1, sizeof(Tree));

    Dot->Left = GetE();

    Dot->Right = GetOp();

    Dot->Inside.Type = CONST_loop;

    return Dot;

}

Tree * GetIf() {

    Tree * Dot = (Tree *) calloc(1, sizeof(Tree));

    Dot->Left = GetE();

    Dot->Right = GetOp();

    Dot->Inside.Type = CONST_cond;

    return Dot;

}

Tree * GetFunc() {

    Tree * Dot = (Tree *) calloc(1, sizeof(Tree));

    Dot->Left = GetID();

    assert(str[p] == '(');
    p++;

    while (str[p] != ')') {


        AddParam(Dot);

        assert(str[p] == ',');
        p++;
    }
    p++;

    Dot->Inside.Type = CONST_call;

    return Dot;

}

#define Oper(name, sym, numb) \
    switch (numb) { \
    case CONST_cond: \
        strcpy(IfName, #sym); \
        break; \
    case CONST_loop: \
        strcpy(WhileName, #sym); \
        break; \
    case CONST_call: \
        strcpy(CallName, #sym); \
        break; \
    }


Tree * GetOp() {

    char IfName[StrLenth] = {};
    char WhileName[StrLenth] = {};
    char CallName[StrLenth] = {};

#include "Oper.h"

#undef Oper

    Tree * Dot = (Tree *) calloc(1, sizeof(Tree));



    if (strstr(&str[p], IfName) == &str[p])
    {
        p += IfLen;
        Dot->Left = GetIf();
    }
    else
    if (strstr(&str[p], WhileName) == &str[p])
    {
        p += WhileLen;
        Dot->Left = GetLoop();
    }
    else
    if (strstr(&str[p], CallName) == &str[p])
    {
        p += CallLen;
        Dot->Left = GetFunc();
    }
    else
        Dot->Left = GetAssn();

    Dot->Inside.Type = NotaDot;

    return Dot;

}

Tree * GetS() {

    Tree * Dot = (Tree *) calloc(1, sizeof(Tree));
    Dot->Left = GetOp();
    Dot->Inside.Type = aDotOp;

    assert(str[p] == ';');
    p++;


    Tree * Hlpr = Dot;

    while (str[p] != '}')
    {
        if(Hlpr->Right)
        {
            Tree * Dot1 = (Tree *) calloc(1, sizeof(Tree));
            Dot1->Left = Hlpr->Right;
            Dot1->Right = GetOp();
            Dot1->Inside.Type = aDotOp;
            Hlpr->Right = Dot1;
            Hlpr = Dot1;

            assert(str[p] == ';');
            p++;
        }
        else
        {
            Dot->Right = GetOp();

            assert(str[p] == ';');
            p++;
        }
    }

    return Dot;

}



Tree * GetG0(const char * what)
{
    Tree * Dot = (Tree *) calloc(1, sizeof(Tree));
    Dot->Inside.Type = NotaDot;

    str = what;
    p = 0;
    Dot->Left = GetFun();

    assert(str[p] == 0);

    return Dot;

}


#endif //LANGUAGE_GRAMMAR_H
