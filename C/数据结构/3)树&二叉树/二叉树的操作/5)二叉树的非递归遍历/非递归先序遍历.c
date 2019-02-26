#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"SeqStack.h"
#include<stdbool.h>//引入头文件

//二叉树的节点
typedef struct BiNode{
	char ch;

	struct BiNode *Lchild;
	struct BiNode *Rchild;


}BiNode;

//有标志的数据
typedef struct Info{
	BiNode *node;
	bool flag;
}Info;

Info *CreateInfo(BiNode *node, bool flag)
{
	Info *info = (Info*)malloc(sizeof(Info));

	info->node = node;
	info->flag = flag;

	return info;
}

void Freeforeach(BiNode *root)
{
	//初始化栈
	void *stack = Init_SeqStack();
	//1.先把根节点入栈
	Push_SeqStack(stack, CreateInfo(root,false));

	while (Size_SeqStack(stack)>0)
	{
		//2.获取栈顶元素,因为压栈时，进入的是Info数据类型
		Info *info = (Info*)Top_SeqStack(stack);
		//3.弹出栈顶元素
		Pop_SeqStack(stack);
		//4.判断弹出的元素的标志是否为true,是true就输出
		if (info->flag)
		{
			printf("%c ", info->node->ch);

			free(info);

			//输出不用再往下走代码
			continue;
		}
		

		//如果弹出的元素标志不为true,取出该节点的左右孩子
		//右孩子入栈
		if (info->node->Rchild != NULL)
		{
			Push_SeqStack(stack, CreateInfo(info->node->Rchild, false));
		}

		


		//左孩子入栈
		if (info->node->Lchild != NULL)
		{
			Push_SeqStack(stack, CreateInfo(info->node->Lchild, false));
		}

		//根节点入栈
		info->flag = true;
		Push_SeqStack(stack, info);


		//根节点在最后是先序遍历，在中间是中序遍历，在前面是后序遍历

		
	}


}

int main()
{
	BiNode node1 = { 'A', NULL, NULL };
	BiNode node2 = { 'B', NULL, NULL };
	BiNode node3 = { 'C', NULL, NULL };
	BiNode node4 = { 'D', NULL, NULL };
	BiNode node5 = { 'E', NULL, NULL };
	BiNode node6 = { 'F', NULL, NULL };
	BiNode node7 = { 'G', NULL, NULL };
	BiNode node8 = { 'H', NULL, NULL };

	//建立二叉树
	node1.Lchild = &node2;
	node1.Rchild = &node6;

	node2.Rchild = &node3;

	node3.Lchild = &node4;
	node3.Rchild = &node5;

	node6.Rchild = &node7;

	node7.Lchild = &node8;


	//非递归先序遍历
	Freeforeach(&node1);


	system("pause");
	return EXIT_SUCCESS;
}

