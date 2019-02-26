#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ����Ľڵ�
typedef struct Node {
	void *data;
	struct Node *next;
}Node;

// ����
typedef struct Linklist {
	Node head; // �����ͷ�ڵ�
	int size; // �ڵ����
}Linklist;

typedef void * List; // αװ��������,��ʵ�� void*

// ��ʼ������
List init();

// ָ��λ�ò��� pos=1 ��ô����Ч�ڵ�ĵ�0���͵�1��֮�����
int insert(List, int, void*);

// ͷ��
int push_front(List, void*);

// β��
int push_back(List, void*);

// ָ��ֵǰ���������  ��ֵ�ͽڵ��ֵ���бȽ�  ͨ���û�����ĺ������бȽ�ֵ
int insert_by_val(List, void*, void*, int(*)(void*, void*));

// ָ��λ��ɾ��
int remove_by_pos(List, int);

// ָ��ֵɾ��
int remove_by_val(List, void*, int(*)(void*, void*));

// ͷɾ
int pop_front(List);

// βɾ
int pop_back(List);

// ����
void each(List, void(*)(void*));

// ����
void destroy(List);