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

// ��ȡ���ĸ߶�(���)
int TreeHeight(TreeNode *root)
{
	if (NULL == root)
	{
		return -1;
	}
	// ���������ĸ߶�
	int Lheight = TreeHeight(root->Lchild);
	// ���������ĸ߶�
	int Rheight = TreeHeight(root->Rchild);
	// ��������, �ĸ��߾�ȡ�Ǹ�, Ȼ���ټ�1
	int height = Lheight > Rheight ? Lheight + 1 : Rheight + 1;

	return height;
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

	int height = TreeHeight(&node1);
	printf("height:%d\n", height);

	system("pause");
	return 0;
}