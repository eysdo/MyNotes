#include "DynamicArray.h"

// ��ʼ����̬���麯��
void* init_array()
{
	DA *arr = (DA*)malloc(sizeof(DA));
	if ( NULL == arr )
	{
		printf("malloc error!");
		return -1;
	}
	arr->capacity = 10; // ��ʼ������
	arr->size = 0; // ��ʼ��Ԫ�ظ���
	arr->data = malloc(sizeof(void*) * arr->capacity); // ����ָ������
	return arr;
}

// ָ��λ�ò�������
int insert(void *_arr, int _pos, void *_data) 
{
	if ( NULL == _arr || NULL == _data )
	{
		return -1;
	}
	DA *arr = (DA*)_arr; // �ı䲽��

	// �ݴ�,���λ�ò���,���ò����λ�õ��� arr->size
	if ( 0 > _pos || _pos > arr->size )
	{
		_pos = arr->size;
	}
	
	// �ж������Ƿ��㹻
	if ( arr->size == arr->capacity )
	{
		// �����Ѿ�����
		// ���������ռ����
		int newcapacity = arr->size * 2;
		void **newspace = (void**)malloc(sizeof(void*) * newcapacity);
		// ���ɿռ�����ݿ������¿ռ�
		memcpy(newspace, arr->data, sizeof(void*) * arr->capacity);
		// �ͷžɿռ�
		free(arr->data);
		// ���º�������
		arr->data = newspace;
		arr->capacity = newcapacity;
	}

	// �ƶ�Ԫ��,�ճ�λ����������
	for (int i = arr->size - 1; i >= _pos; i--)
	{
		arr->data[i + 1] = arr->data[i];
	}

	// ��Ԫ�ز���
	arr->data[_pos] = _data;

	// ά��Ԫ�ظ���
	arr->size++;

	return 0;
}

// ͷ������
int push_front(void *_arr, void *_data)
{
	return insert(_arr, 0, _data);
}

// β������
int push_back(void *_arr, void *_data)
{
	DA *arr = (DA*)_arr;
	return insert(_arr, arr->size, _data);
}

// ָ��λ��ɾ��,�±���0��ʼ
int remove_by_pos(void *_arr, int _pos)
{
	if ( NULL == _arr )
	{
		return -1;
	}
	DA *arr = (DA*)_arr;
	if ( 0 == arr->size) // û��Ԫ��
	{
		return -2;
	}
	// �ж�λ���Ƿ�Ϸ�
	if ( 0 > _pos || _pos > arr->size -1 )
	{
		return -3;
	}
	// �ƶ�Ԫ����ɾ��Ԫ��
	for (int i = _pos; i < arr->size - 1; i++)
	{
		arr->data[i] = arr->data[i + 1];
	}
	arr->size--;
	return 0;
}

// ����ֵ��ɾ��,��������������Ҫ�û����뺯����ַ
int remove_by_val(void *_arr, void *_data, compare *_compare)
{
	if ( NULL == _arr || NULL == _data || NULL == _compare)
	{
		return -1;
	}
	DA *arr = (DA*)_arr;
	for (int i = 0; i < arr->size; i++)
	{
		if (_compare(arr->data[i], _data))
		{
			remove_by_pos(arr, i);
			break;
		}
	}
	return 0;
}

// ͷ��ɾ��
int pop_front(void *_arr)
{
	return remove_by_pos(_arr, 0);
}

// β��ɾ��
int pop_back(void *_arr)
{
	DA *arr = (DA*)_arr;
	return remove_by_pos(_arr, arr->size - 1);
}

// ��ȡ����Ĵ�С
int get_size(void *_arr)
{
	DA *arr = (DA*)_arr;
	return arr->size;
}

// ��ȡ����
int get_capacity(void *_arr)
{
	DA *arr = (DA*)_arr;
	return arr->capacity;
}

// ��ӡ���ݵĺ���
void each(void *_arr, void(*foreach)(void*))
{
	if ( NULL == _arr || NULL == foreach )
	{
		return;
	}
	DA *arr = (DA*)_arr;
	for (size_t i = 0; i < arr->size; i++)
	{
		foreach(arr->data[i]);
	}
}

// ��������
int destroy_array(void *_arr)
{
	if ( NULL == _arr)
	{
		return -1;
	}

	DA *arr = (DA*)_arr;
	if ( arr->data != NULL )
	{
		arr->capacity = 0;
		arr->size = 0;
		free(arr->data);
	}
	free(arr);
	arr = NULL;
	return 0;
}