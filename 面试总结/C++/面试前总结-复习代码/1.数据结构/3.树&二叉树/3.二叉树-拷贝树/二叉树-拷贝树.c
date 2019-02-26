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

// ��ӡ�� - ������� (������)
void print_r(TreeNode *root)
{
	if (NULL == root)
	{
		return;
	}

	// ������� (������)
	printf("%c\t", root->ch);
	// �ٷ���������
	print_r(root->Lchild);
	// �ٷ���������
	print_r(root->Rchild);
}

// ������
TreeNode *copyTree(TreeNode *root)
{
	if (NULL == root)
	{
		return NULL;
	}

	// �ݹ鿽��������
	TreeNode *l = copyTree(root->Lchild);
	// �ݹ鿽��������
	TreeNode *r = copyTree(root->Rchild);
	// ����ռ�, װ����
	TreeNode *newnode = (TreeNode*)malloc(sizeof(TreeNode));
	// ����
	newnode->ch = root->ch;
	newnode->Lchild = l;
	newnode->Rchild = r;
	return newnode;
}

// �ͷŶѿռ�
void freeSpace(TreeNode *root)
{
	if (NULL == root)
	{
		return;
	}
	// �ͷ�������
	freeSpace(root->Lchild);
	// �ͷ�������
	freeSpace(root->Rchild);
	// �ͷŸ��ڵ�
	free(root);
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

	// ����������
	TreeNode *root = copyTree(&node1);
	// ��ӡԭ������
	print_r(&node1);
	printf("\n--------------------------------------\n");
	// ��ӡ��������
	print_r(root);

	// �ͷſ����Ķ������ռ�
	freeSpace(root);

	system("pause");
	return 0;
}