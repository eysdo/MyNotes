#pragma once
#include <string>

class Uuid
{
public:
	/* �������� */
	static Uuid* Get();

	/* ��ȡ����Ψһ��ʶ */
	virtual std::string GetUUID() = 0;

	virtual ~Uuid();


protected:
	Uuid();
};

