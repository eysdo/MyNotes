#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DynamicArray {
	void **data; // ���ݿռ���׵�ַ
	int capacity; // ����
	int size; // Ԫ�ظ���
}DA;

// ���庯������
typedef int(compare)(void*, void*);

// ��ʼ����̬���麯��
void* init_array();

// ָ��λ�ò�������
int insert(void*, int, void*);

// ͷ������
int push_front(void*, void*);

// β������
int push_back(void*, void*);

// ָ��λ��ɾ��
int remove_by_pos(void*, int);

// ����ֵ��ɾ��,��������������Ҫ�û����뺯����ַ
int remove_by_val(void*, void*, compare*);

// ͷ��ɾ��
int pop_front(void*);

// β��ɾ��
int pop_back(void*);

// ��ȡ����Ĵ�С
int get_size(void*);

// ��ȡ����
int get_capacity(void*);

// ��ӡ���ݵĺ���
void each(void*, void(*)(void*));

// ��������
int destroy_array(void*);