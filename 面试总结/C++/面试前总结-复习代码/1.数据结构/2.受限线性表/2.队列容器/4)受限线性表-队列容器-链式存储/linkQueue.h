#pragma once
#include "Linklist.h" // ����֮ǰ��װ�õ� Linux�ں�����-��ʽ�洢�汾2

// ��ʼ��
void* init_link_queue();

// ���
void push_link_queue(void*, void*);

// ����
void pop_link_queue(void*);

// ��ȡ��ͷԪ��
void* get_link_queue_head(void*);

// ��ȡ��βԪ��
void* get_link_queue_tail(void*);

// ��ȡԪ�ظ���
int get_link_queue_size(void*);

// ���ٶ���
void destroy_link_queue(void*);