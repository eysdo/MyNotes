#include "SeqStack.h"

// ��ʼ��ջ
void* init()
{
	SeqStack *stack = (SeqStack*)malloc(sizeof(SeqStack));
	if (NULL == stack)
	{
		return NULL;
	}

	stack->size = 0;
	for (int i = 0; i < 1024; i++)
	{
		stack->data[i] = 0;
	}
	return stack;
}

// ��ջ (ѹջ)
void push(void *_stack,void *_data)
{
	if (NULL == _stack || NULL == _data)
	{
		return;
	}
	SeqStack *stack = (SeqStack*)_stack;
	// ���˾Ͳ���ջ
	if (stack->size == 1024)
	{
		return;
	}
	stack->data[stack->size] = _data;
	stack->size++;
}

// ��ջ
void pop(void *_stack)
{
	if (NULL == _stack)
	{
		return;
	}
	SeqStack *stack = (SeqStack*)_stack;
	stack->size--;
}

// ��ȡԪ�ظ���
int get_size(void *_stack)
{
	if (NULL== _stack)
	{
		return -1;
	}
	SeqStack *stack = (SeqStack*)_stack;
	return stack->size;
}

// ��ȡջ��Ԫ��
void* get_stack_top(void *_stack)
{
	if (NULL == _stack)
	{
		return NULL;
	}
	SeqStack *stack = (SeqStack*)_stack;
	return stack->data[stack->size - 1];
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
