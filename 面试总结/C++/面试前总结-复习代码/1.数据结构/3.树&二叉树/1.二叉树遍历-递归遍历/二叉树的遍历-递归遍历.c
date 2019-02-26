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
void print_1(TreeNode *root) 
{
	if (NULL == root)
	{
		return;
	}

	// ������� (������)
	printf("%c\t",root->ch);
	// �ٷ���������
	print_1(root->Lchild);
	// �ٷ���������
	print_1(root->Rchild);
}

// ��ӡ�� - ������� (�����)
void print_2(TreeNode *root)
{
	if (NULL == root)
	{
		return;
	}

	// �ٷ���������
	print_2(root->Lchild);
	// ������� (�����)
	printf("%c\t", root->ch);
	// �ٷ���������
	print_2(root->Rchild);
}

// ��ӡ�� - ������� (���Ҹ�)
void print_3(TreeNode *root)
{
	if (NULL == root)
	{
		return;
	}

	// �ٷ���������
	print_3(root->Lchild);
	// �ٷ���������
	print_3(root->Rchild);
	// ������� (���Ҹ�)
	printf("%c\t", root->ch);
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

	printf("���������� - ����\n");
	print_1(&node1);
	printf("\n���������� - ����\n");
	print_2(&node1);
	printf("\n���������� - ����\n");
	print_3(&node1);
	printf("\n\n");

	system("pause");
	return 0;
}