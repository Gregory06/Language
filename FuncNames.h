//
// Created by Greg Postnikoff on 11.12.17.
//

#ifndef LANGUAGE_FUNCNAMES_H
#define LANGUAGE_FUNCNAMES_H


Tree * GetG0(const char * s);
Tree * GetFun();
Tree * GetS();
Tree * GetE();
Tree * GetT();
Tree * GetP();
Tree * GetN();
Tree * GetID();
Tree * GetReg();
Tree * GetAssn();
Tree * GetIf();
Tree * GetOp();


int FirstCompile(char * Programm, VarArr * ProgArr, FILE * file);
Tree * CleanTree(Tree * Node);
int PrintTreeDot(const Tree const * Node);
int PrintOp(Tree * Prog, VarArr * ProgArr);
int PrintCond(Tree * Prog, VarArr * ProgArr);
int PrintNode(Tree * Prog, VarArr * ProgArr);
int PrintAssn(Tree * Prog, VarArr * ProgArr);
int PrintProg(Tree * Prog, VarArr * ProgArr);
int PrintLoop(Tree * Prog, VarArr * ProgArr);
int PrintFunc(Tree * Prog, VarArr * ProgArr);
char * IntToStr(int a);
int PrintAllCode (Tree * Prog, VarArr * ProgArr);
int PrintCall(Tree * Prog, VarArr * ProgArr);
int AddParam(Tree * Node);
int PrintFUNCS(Tree * Prog, VarArr * ProgArr);
int PrintParams(Tree * Prog, VarArr * ProgArr);
int PrintPARAMS(Tree * Prog, VarArr * ProgArr);

#endif //LANGUAGE_FUNCNAMES_H
