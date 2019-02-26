#include "Linklist.h"

// ��ʼ������
List init()
{
	// ͷ�ڵ�ռ�
	Linklist *list = (Linklist*)malloc(sizeof(Linklist));
	if (NULL == list)
	{
		return NULL;
	}
	list->size = 0;
	list->head.data = NULL;
	list->head.next = NULL;

	return list;
}

// ָ��λ�ò��� pos=1 ��ô����Ч�ڵ�ĵ�0���͵�1��֮�����
int insert(List _list, int _pos, void *_data)
{
	if (NULL == _list || NULL == _data)
	{
		return -1;
	}

	Linklist *list = (Linklist*)_list;

	if (0 > _pos || _pos > list->size)
	{
		_pos = list->size;
	}

	// ���帨��ָ��
	Node *pCur = &(list->head); // ��Ϊhead��ʵ��
	for (int i = 0; i < _pos; i++)
	{
		pCur = pCur->next;
	}

	Node *newnode = (Node*)malloc(sizeof(Node));
	newnode->data = _data;
	newnode->next = NULL;

	// �½ڵ�������
	newnode->next = pCur->next;
	pCur->next = newnode;

	// ���µ�ǰ�ڵ�����
	list->size++;

	return 0;
}

// ͷ��
int push_front(List _list, void *_data)
{
	return insert(_list, 0, _data);
}

// β��
int push_back(List _list, void *_data)
{
	Linklist *list = (Linklist*)_list;
	return insert(_list, list->size, _data);
}

// ָ��ֵǰ���������  ��ֵ�ͽڵ��ֵ���бȽ�  ͨ���û�����ĺ������бȽ�ֵ
int insert_by_val(List _list, void *_oldval, void *_newval, int(*_compare)(void*, void*))
{
	if (NULL == _list || NULL == _oldval || NULL == _newval || NULL == _compare)
	{
		return -1;
	}

	Linklist *list = (Linklist*)_list;

	// ������������ָ��
	Node *pPre = &(list->head); // ǰһ���ڵ�Ԫ��
	Node *pCur = pPre->next; // ��ǰԪ��

	while (NULL != pCur)
	{
		if (_compare(pCur->data, _oldval))
		{
			// �����½ڵ�
			Node *newnode = (Node*)malloc(sizeof(Node));
			newnode->data = _newval;
			newnode->next = NULL;

			// �½ڵ�������
			pPre->next = newnode;
			newnode->next = pCur;

			list->size++;
			break;
		}

		pPre = pCur;
		pCur = pCur->next;
	}

	return 0;
}

// ָ��λ��ɾ��
int remove_by_pos(List _list, int _pos)
{
	if (NULL == _list)
	{
		return -1;
	}
	Linklist *list = (Linklist*)_list;
	if (list->size == 0)
	{
		return -1;
	}
	if (_pos<0 || _pos>list->size - 1)
	{
		return -2;
	}

	// Ѱַλ��
	Node *pCur = &(list->head);

	for (int i = 0; i < _pos; i++)
	{
		pCur = pCur->next;
	}

	// ��ʱ, pCur��Ҫɾ���Ľڵ����һ���ڵ�
	Node *pDel = pCur->next;
	// ���½���ǰ������ϵ
	pCur->next = pDel->next; // �Ͽ���pDel����ڵ�
	// �ͷŽڵ�ռ�
	free(pDel);
	pDel = NULL;

	list->size--;

	return 0;
}

// ָ��ֵɾ��
int remove_by_val(List _list, void *_data, int(*_compare)(void*, void*))
{
	if (NULL == _list || NULL == _data || NULL == _compare)
	{
		return -1;
	}
	Linklist *list = (Linklist*)_list;

	Node *pPre = &(list->head);
	Node *pCur = pPre->next;

	while (NULL != pCur)
	{
		if (_compare(pCur->data, _data))
		{
			pPre->next = pCur->next;
			free(pCur);
			list->size--;
			break;
		}
		pPre = pCur;
		pCur = pCur->next;
	}
	return 0;
}

// ͷɾ
int pop_front(List _list)
{
	return remove_by_pos(_list, 0);
}

// βɾ
int pop_back(List _list)
{
	Linklist *list = (Linklist*)_list;
	return remove_by_pos(_list, list->size - 1);
}

// ����
void each(List _list, void(*_each)(void*))
{
	if (NULL == _list || NULL == _each)
	{
		return;
	}
	Linklist *list = (Linklist*)_list;
	// ���帨��ָ��
	Node *pCur = list->head.next; // ����Ч�ڵ㿪ʼ����
	while (NULL != pCur)
	{
		_each(pCur->data);
		pCur = pCur->next;
	}
}

// ����
void destroy(List _list)
{
	if (NULL == _list)
	{
		return;
	}
	Linklist *list = (Linklist*)_list;
	// ����Ч�ڵ㿪ʼɾ��
	Node *pCur = list->head.next; // ��һ����Ч�ڵ�
	while (NULL != pCur)
	{
		// �洢��һ���ڵ�ĵ�ַ
		Node *tmp = pCur->next;
		// �ͷŵ�ǰ�ڵ�
		free(pCur);
		pCur = tmp;
	}
	// �ͷ�ͷ�ڵ�
	free(list);
	list = NULL;
}