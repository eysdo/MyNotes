#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	struct Node *next;
}Node;

typedef struct LinkStack {
	Node head;
	int size;
}LinkStack;

// ��ʼ��
void* init();

// ��ջ
void push(void*, void*);

// ��ջ
void pop(void*);

// ��ȡջ�Ķ�Ԫ��
void* get_stack_top(void*);

// ��ȡԪ�ظ���
int get_size(void*);

// ����ջ
void destroy(void*);