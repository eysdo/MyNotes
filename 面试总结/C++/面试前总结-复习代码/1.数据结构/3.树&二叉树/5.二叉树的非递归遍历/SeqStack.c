#include "SeqStack.h"


//初始化栈
void *Init_SeqStack()
{
	SeqStack *stack = (SeqStack*)malloc(sizeof(SeqStack));
	if (stack == NULL)
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

//入栈（压栈)
void Push_SeqStack(void *stack, void *data)
{
	if (stack == NULL || data == NULL)
	{
		return;
	}
	SeqStack *mystack = (SeqStack*)stack;

	//满了就不入栈
	if (mystack->size == 1024)
	{
		return;
	}

	mystack->data[mystack->size] = data;

	mystack->size++;


}
//出栈
void Pop_SeqStack(void *stack)
{
	if (stack == NULL)
	{
		return;
	}

	SeqStack *mystack = (SeqStack*)stack;

	mystack->size--;
}

//获取元素个数
int Size_SeqStack(void *stack)
{
	if (stack == NULL)
	{
		return;
	}

	SeqStack *mystack = (SeqStack*)stack;
	return mystack->size;
}

//获取栈顶元素
int Top_SeqStack(void *stack)
{
	if (stack == NULL)
	{
		return;
	}
	SeqStack *mystack = (SeqStack*)stack;

	return mystack->data[mystack->size - 1];

}

//销毁栈
void Destroy_SeqStack(void *stack)
{
	if (stack != NULL)
	{
		free(stack);
		stack = NULL;
	}
}