#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ˳��洢
typedef struct SeqQueue {
	void *data[1024]; // ָ������
	int size;
}SeqQueue;

// ��ʼ��
void* init();

// ���
void push(void*, void*);

// ����
void pop(void*);

// ��ȡ��ͷԪ��
void* get_queue_head(void*);

// ��ȡ��βԪ��
void* get_queue_tail(void*);

// ��ȡԪ�ظ���
int get_size(void*);

// ���ٶ���
void destroy(void*);