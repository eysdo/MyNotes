#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//顺序桟
typedef struct SeqStack{
	void *data[1024];
	int size;

}SeqStack;

//初始化栈
void *Init_SeqStack();

//入栈（压栈)
void Push_SeqStack(void *stack, void *data);
//出栈
void Pop_SeqStack(void *stack);

//获取元素个数
int Size_SeqStack(void *stack);

//获取栈顶元素
int Top_SeqStack(void *stack);

//销毁栈
void Destroy_SeqStack(void *stack);

