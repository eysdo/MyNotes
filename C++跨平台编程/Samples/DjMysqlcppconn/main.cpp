#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

// DjMysqlcppconn
#include "MySql.h"
using namespace DjMysqlcppconn;

// C++ 智能指针
#include <memory>

// 测试
void test()
{
	try
	{
		{
			unique_ptr<MySql> sql(new MySql);

			// 重新选择数据库,默认读配置
			sql->use("test");

			// 普通查询
			//ResultSet* res = sql->query("select * from user");

			// 数据绑定查询 (ResultSet* res 不需要考虑空间释放!内部已实现自动释放!)
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

			// 执行增删改操作, 返回受影响行数
			int ret = sql
				->state("insert into `chat`(data_type, data) values(?, ?);")
				->bind(1, 1)
				->bind(2, "test insert!")
				->save();
			if (0 < ret)
				cout << "insert success!" << endl;
			else
				cout << "insert failed!" << endl;

			// 事务操作 - 提交、回滚(MyISAM表引擎不支持)
			sql->setAutoCommit(false); // 关闭自动提交
			sql->setSavepoint("savept01");
			ret = sql->save("insert into `test`(msg) values('test rollback!');");
			sql->rollback("savept01");
			ret = sql->save("insert into `test`(msg) values('test commit!');");
			sql->commit();

			// 执行任何SQL语句，返回 BOOL(表示是否返回ResultSet)
			bool ret_2 = sql->exec("truncate table `test`;");
			cout << "清空test表成功!!" << endl; // truncate 无返回ResultSet

			ret_2 = sql
				->state("select `name` from `user` where `status`=? and `name`=?;")
				->bind(1, 1)
				->bind(2, "Dejan")
				->exec();
			if (ret_2)
				cout << "有返回ResultSet!!" << endl;


			//cout << "连接未关闭! 按下任意键关闭当前连接!!" << endl;
			//getchar();
		}
		cout << "Mysql连接已断开!!" << endl;

		// mysql连接数查看
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