#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ���û������ݰ�������ṹ��
typedef struct Node {
	struct Node *next;
}Node;

typedef struct Linklist {
	Node head;
	int size;
}Linklist;

// ��ʼ������
void* init();

// ָ��λ�ò���
void insert(void*, int, Node*);

// ����
void each(void*, void(*)(void*));

// ����λ�û�ȡֵ
void* get_node(void*, int);

// ��ȡԪ�ظ���
int get_size(void*);

// ����λ��ɾ��
void remove_by_pos(void*, int);

// ����
void destroy(void*);