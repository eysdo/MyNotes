#include "Linklist.h"

// ��ʼ������
void* init()
{
	// ͷ�ڵ�Ŀռ�
	Linklist *list = (Linklist*)malloc(sizeof(Linklist));
	if (NULL == list)
	{
		return NULL;
	}
	list->head.next = NULL;
	list->size = 0;
	return list;
}

// ָ��λ�ò���
void insert(void *_list, int _pos, void *_data)
{
	if (NULL == _list || NULL == _data)
	{
		return;
	}
	Linklist *list = (Linklist*)_list;
	Node *data = (Node*)_data;
	if (_pos < 0 || _pos > list->size)
	{
		_pos = list->size;
	}

	// �ҵ�posλ�ýڵ��ǰһ���ڵ�
	Node *pCur = &(list->head);
	for (int i = 0; i < _pos; i++)
	{
		pCur = pCur->next;
	}

	// ���½ڵ�������
	data->next = pCur->next;
	pCur->next = data;

	// ���½ڵ���
	list->size++;
}

// ����
void each(void *_list, void(*_each)(void*))
{
	if (NULL == _list || NULL == _each)
	{
		return;
	}
	Linklist *list = (Linklist*)_list;
	Node *pCur = list->head.next;
	while (NULL != pCur)
	{
		_each(pCur);
		pCur = pCur->next;
	}
}

// ����λ�û�ȡֵ
void* get_node(void *_list, int _pos)
{
	if (NULL == _list)
	{
		return NULL;
	}
	Linklist *list = (Linklist*)_list;
	if (_pos < 0 || _pos > list->size - 1)
	{
		return NULL;
	}

	Node *pCur = &(list->head);
	for (int i = 0; i < _pos; i++)
	{
		pCur = pCur->next;
	}
	return pCur->next;
}

// ��ȡԪ�ظ���
int get_size(void *_list)
{
	if (NULL == _list)
	{
		return -1;
	}
	Linklist *list = (Linklist*)_list;
	return list->size;
}

// ����λ��ɾ��
void remove_by_pos(void *_list, int _pos)
{
	if (NULL == _list)
	{
		return;
	}
	Linklist *list = (Linklist*)_list;
	if (list->size == 0)
	{
		return;
	}
	if (_pos<0 || _pos>list->size - 1)
	{
		return;
	}

	Node *pCur = &(list->head);
	for (int i = 0; i < _pos; i++)
	{
		pCur = pCur->next;
	}

	// �����ɾ���Ľڵ�
	Node *Del = pCur->next;
	// ���½���ǰ������ϵ
	pCur->next = Del->next;

	list->size--;
}

// ����
void destroy(void *_list)
{
	if (NULL != _list)
	{
		free(_list);
		_list = NULL;
	}
}