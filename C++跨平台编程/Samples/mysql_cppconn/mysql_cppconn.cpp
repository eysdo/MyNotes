#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

/// Mysql 配置
// ----------------------
#define MYSQL_URL    "tcp://127.0.0.1"
#define MYSQL_USER   "root"
#define MYSQL_PASS   "root"
#define MYSQL_DBNAME "mt_customer_service_system"
// ----------------------

// MySQL
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
using namespace sql;
#pragma comment(lib, "mysqlcppconn.lib")
// C++ 智能指针
#include <memory>

int main(int argc, char **argv)
{
	try
	{
		// 获取mysql驱动
		Driver* driver = ::get_driver_instance();

		unique_ptr<Connection> conn(driver->connect(MYSQL_URL, MYSQL_USER, MYSQL_PASS)); // 建立连接
		unique_ptr<Statement> state(conn->createStatement());
		unique_ptr<PreparedStatement> prep_stmt;

		//state->execute("use DBName");
		conn->setSchema(MYSQL_DBNAME); // 选择数据库

		// 普通执行
		//res = state->executeQuery("select * from user");

		// 数据绑定 - 查询
		prep_stmt.reset(conn->prepareStatement("select `name` from `user` where `id`=?"));
		prep_stmt->setInt(1, 1);
		unique_ptr<ResultSet> res;
		res.reset(prep_stmt->executeQuery());

		cout << "rowsCount(): " << res->rowsCount() << endl;

		while (res->next())
		{
			cout << "name:" << res->getString("name") << endl;
			cout << "name:" << res->getString(1) << endl;
		}

		// 关闭Mysql自动提交
		conn->setAutoCommit(0);

		// 数据绑定 - 增删改执行
		prep_stmt.reset(conn->prepareStatement("insert into `chat`(`data`,`data_type`) values(?, ?);"));
		prep_stmt->setString(1, "Hello!");
		prep_stmt->setInt(2, 1);
		int update_count = prep_stmt->executeUpdate();

		// 设置保存点
		unique_ptr<Savepoint> savept;
		savept.reset(conn->setSavepoint("savept1"));
		prep_stmt->setString(1, "World!");
		prep_stmt->setInt(2, 1);
		update_count = prep_stmt->executeUpdate();
		conn->rollback(savept.get());
		conn->releaseSavepoint(savept.get());
		conn->commit();


		// 关闭连接
		conn->close();
		// mysql连接数查看
		// > show status like 'Threads%';
		// > show variables like '%timeout%';

	}
	catch (SQLException &e)
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
	


	getchar();
	return 0;
}