#pragma once
#include <string>

class Uuid
{
public:
	/* 工厂方法 */
	static Uuid* Get();

	/* 获取主机唯一标识 */
	virtual std::string GetUUID() = 0;

	virtual ~Uuid();


protected:
	Uuid();
};

