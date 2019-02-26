#include "SeqStack.h"

// �ж�������
int is_left(char ch)
{
	return ch == '(';
}

// �ж�������
int is_right(char ch)
{
	return ch == ')';
}

// ��ӡ����λ��
void printErr(const char *str,const char *err,const char *p) 
{
	printf("Error:%s\n",err);
	printf("%s\n",str);
	int len = p - str;
	for (int i = 0; i < len; i++)
	{
		printf(" ");
	}
	printf("A\n");
}

int main()
{
	const char *str = "5+5*(6)+9/3*1)-(1+3(";

	// ��ʼ��ջ
	void *stack = init();
	char *p = (char*)str;

	while (*p != '\0')
	{
		// �����������, ��ջ
		if (is_left(*p))
		{
			push(stack,p);
		}

		// �����������, �����������: 1.ջ��Ϊ��,ƥ��ɹ�,��ջ�е��ַ�����
		// 2.ջΪ��, ���������û��ƥ���������
		if (is_right(*p))
		{
			if (get_size(stack) > 0) 
			{
				pop(stack);
			}
			else 
			{
				// ��ӡ��������Ϣ
				printErr(str, "û��ƥ�䵽������",p);
			}
		}
		p++;
	}

	// ɨ������ַ���, ��ô�ж�ջ�Ƿ�Ϊ��
	while (get_size(stack) > 0)
	{
		char *pChar = get_stack_top(stack);
		// ����ջ��Ԫ��
		pop(stack);
		// ��ӡ������Ϣ
		printErr(str, "û��ƥ�䵽������",pChar);
	}

	system("pause");
	return 0;
}