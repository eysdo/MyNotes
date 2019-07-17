#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
/// Redis
//=========================
#include <hiredis/hiredis.h>
#define REDIS_PORT   6379
#define REDIS_HOST   "127.0.0.1"
#pragma comment(lib, "hiredis.lib")
#pragma comment(lib, "Ws2_32.lib")
//=========================


int main(int argc, char **argv)
{
	cout << "Redis���Ĺ��ڴ����¼���ʼ! - Dejan" << endl;
	cout << "---------------------------------------------" << endl;

	redisContext *redis = redisConnect(REDIS_HOST, REDIS_PORT);
	if (redis == NULL || redis->err) {
		if (redis) {
			printf("Error: %s\n", redis->errstr);
		}
		else {
			printf("Can't allocate redis context\n");
		}
	}

	// ��Ϣ���Ŀ�ʼ
	// �޸� redis.conf �ļ�, �� notify-keyspace-events Ex ��ע�ͣ�����Key����֪ͨ����
	redisReply* reply = (redisReply*)redisCommand(redis, "PSUBSCRIBE __keyevent@*__:expired");
	//redisReply* reply = (redisReply*)redisCommand(redis, "subscribe __keyevent@0__:expired");
	freeReplyObject(reply);

	// ��ȡ���Ľ�� redisGetReply()��������
	while (redisGetReply(redis, (void **)&reply) == REDIS_OK)
	{
		if (NULL == reply) return 0;

		if (reply->type == REDIS_REPLY_ARRAY)
		{
			for (int i = 0; i < reply->elements; i++)
			{
				printf("[%d] => %s \n", i, reply->element[i]->str);
			}
		}

	}

	// �ͷ��ڴ�
	freeReplyObject(reply); reply = NULL;
	redisFree(redis); redis = NULL;

	// redis-cli.exe ����
	// > set dejan:01:01 000 ex 1


	getchar();
	return 0;
}