#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

// DjMysqlcppconn
#include "MySql.h"
using namespace DjMysqlcppconn;

// C++ ����ָ��
#include <memory>

// ����
void test()
{
	try
	{
		{
			unique_ptr<MySql> sql(new MySql);

			// ����ѡ�����ݿ�,Ĭ�϶�����
			sql->use("test");

			// ��ͨ��ѯ
			//ResultSet* res = sql->query("select * from user");

			// ���ݰ󶨲�ѯ (ResultSet* res ����Ҫ���ǿռ��ͷ�!�ڲ���ʵ���Զ��ͷ�!)
			sql::ResultSet* res = sql
				->state("select `name` from `user` where `status`=? and `name`=?;")
				->bind(1, 1)
				->bind(2, "Dejan")
				->query();

			cout << "rowsCount(): " << res->rowsCount() << endl;

			while (res->next())
			{
				cout << "name:" << res->getString("name") << endl;
				cout << "name:" << res->getString(1) << endl;
			}

			// ִ����ɾ�Ĳ���, ������Ӱ������
			int ret = sql
				->state("insert into `chat`(data_type, data) values(?, ?);")
				->bind(1, 1)
				->bind(2, "test insert!")
				->save();
			if (0 < ret)
				cout << "insert success!" << endl;
			else
				cout << "insert failed!" << endl;

			// ������� - �ύ���ع�(MyISAM�����治֧��)
			sql->setAutoCommit(false); // �ر��Զ��ύ
			sql->setSavepoint("savept01");
			ret = sql->save("insert into `test`(msg) values('test rollback!');");
			sql->rollback("savept01");
			ret = sql->save("insert into `test`(msg) values('test commit!');");
			sql->commit();

			// ִ���κ�SQL��䣬���� BOOL(��ʾ�Ƿ񷵻�ResultSet)
			bool ret_2 = sql->exec("truncate table `test`;");
			cout << "���test��ɹ�!!" << endl; // truncate �޷���ResultSet

			ret_2 = sql
				->state("select `name` from `user` where `status`=? and `name`=?;")
				->bind(1, 1)
				->bind(2, "Dejan")
				->exec();
			if (ret_2)
				cout << "�з���ResultSet!!" << endl;


			//cout << "����δ�ر�! ����������رյ�ǰ����!!" << endl;
			//getchar();
		}
		cout << "Mysql�����ѶϿ�!!" << endl;

		// mysql�������鿴
		// > show status like 'Threads%';

	}
	catch (sql::SQLException &e)
	{
		/*
		The JDBC API throws three different exceptions:
		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << "EXAMPLE_FUNCTION" << ") on line " << __LINE__ << endl;

		/* Use what() (derived from std::runtime_error) to fetch the error message */

		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	catch (const exception &e)
	{
		cerr << "Get exception : " << e.what() << endl;
	}
}

int main(int argc, char **argv)
{

	test();

	getchar();
	return 0;
}