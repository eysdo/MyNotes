#include "LinkStack.h"

// ��ʼ��
void* init()
{
	LinkStack *stack = (LinkStack*)malloc(sizeof(LinkStack));
	if (NULL == stack)
	{
		return NULL;
	}
	stack->head.next = NULL;
	stack->size = 0;

	return stack;
}

// ��ջ
void push(void *_stack, void *_data)
{
	if (NULL == _stack || NULL == _data)
	{
		return;
	}
	LinkStack *stack = (LinkStack*)_stack;
	Node *node = (Node*)_data;

	// ��ջ
	node->next = stack->head.next;
	stack->head.next = _data;

	stack->size++;
}

// ��ջ
void pop(void *_stack)
{
	if (NULL == _stack)
	{
		return;
	}
	LinkStack *stack = (LinkStack*)_stack;
	if (stack->size == 0)
	{
		return;
	}
	Node *pDel = stack->head.next;
	stack->head.next = pDel->next;

	stack->size--;
}

// ��ȡջ�Ķ�Ԫ��
void* get_stack_top(void *_stack)
{
	if (NULL == _stack)
	{
		return NULL;
	}
	LinkStack *stack = (LinkStack*)_stack;
	return stack->head.next;
}

// ��ȡԪ�ظ���
int get_size(void *_stack)
{
	if (NULL == _stack)
	{
		return -1;
	}
	LinkStack *stack = (LinkStack*)_stack;
	return stack->size;
}

// ����ջ
void destroy(void *_stack)
{
	if (NULL != _stack)
	{
		free(_stack);
		_stack = NULL;
	}
}
