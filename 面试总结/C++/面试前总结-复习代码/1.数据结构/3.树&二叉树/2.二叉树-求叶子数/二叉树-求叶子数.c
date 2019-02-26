#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// �������Ľڵ�
typedef struct TreeNode {
	char ch;

	struct TreeNode *Lchild;
	struct TreeNode *Rchild;
}TreeNode;

// ������Ҷ����
void leafNum(TreeNode *root, int *num)
{
	// �ݹ������˳�
	if (NULL == root)
	{
		return;
	}

	// ���ڵ�����Ӻ��Һ��Ӷ�Ϊ��ʱ, ����ڵ����Ҷ�ӽڵ�
	if (NULL == root->Lchild && NULL == root->Rchild)
	{
		(*num)++;
	}
	// ����������
	leafNum(root->Lchild, num);
	// �����Һ�����
	leafNum(root->Rchild, num);
}

int main()
{
	/*
	��:
				A
			   / \
			  B   F
			  \    \
			   C	G
			  /\	/
			 D  E  H

	�������(������): A B C D E F G H
	�������(�����): B D C E A F H G
	�������(���Ҹ�): D E C B H G F A
	*/

	// ģ����
	TreeNode node1 = { 'A',NULL,NULL };
	TreeNode node2 = { 'B',NULL,NULL };
	TreeNode node3 = { 'C',NULL,NULL };
	TreeNode node4 = { 'D',NULL,NULL };
	TreeNode node5 = { 'E',NULL,NULL };
	TreeNode node6 = { 'F',NULL,NULL };
	TreeNode node7 = { 'G',NULL,NULL };
	TreeNode node8 = { 'H',NULL,NULL };

	// ����������
	node1.Lchild = &node2; // B
	node1.Rchild = &node6; // F

	node2.Rchild = &node3; // C

	node3.Lchild = &node4; // D
	node3.Rchild = &node5; // E

	node6.Rchild = &node7; // G

	node7.Lchild = &node8; // H


	// ��Ҷ����
	int num = 0;
	leafNum(&node1, &num);
	printf("num=%d\n", num);

	system("pause");
	return 0;
}