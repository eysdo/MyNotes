#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ˳��ջ
typedef struct SeqStack {
	void *data[1024];
	int size;
}SeqStack;

// ��ʼ��ջ
void* init();

// ��ջ
void push(void*, void*);

// ��ջ
void pop(void*);

// ��ȡԪ�ظ���
int get_size(void*);

// ��ȡջ��Ԫ��
void* get_stack_top(void*);

// ����ջ
void destroy(void*);
