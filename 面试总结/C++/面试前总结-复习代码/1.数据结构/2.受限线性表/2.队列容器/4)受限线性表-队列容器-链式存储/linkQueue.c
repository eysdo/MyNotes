#include "linkQueue.h"

// ��ʼ��
void* init_link_queue()
{
	return init();
}

// ���
void push_link_queue(void *queue, void *data)
{
	insert(queue, 0, data);
}

// ����
void pop_link_queue(void *queue)
{
	remove_by_pos(queue, get_size(queue) - 1);
}

// ��ȡ��ͷԪ��
void* get_link_queue_head(void *queue)
{
	return get_node(queue, get_size(queue) - 1);
}

// ��ȡ��βԪ��
void* get_link_queue_tail(void *queue)
{
	return get_node(queue, 0);
}

// ��ȡԪ�ظ���
int get_link_queue_size(void *queue)
{
	return get_size(queue);
}

// ���ٶ���
void destroy_link_queue(void *queue)
{
	destroy(queue);
}