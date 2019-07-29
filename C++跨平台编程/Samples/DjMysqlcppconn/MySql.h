#ifndef MYSQL_H
#define MYSQL_H
#include <map>
// MySQL
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

#ifdef _MSC_VER // Windows
	#pragma comment(lib, "mysqlcppconn.lib")
#endif

namespace DjMysqlcppconn 
{
	class MySql
	{
	public:
		MySql();
		~MySql();

		/* 切换数据库 */
		void use(const std::string& dbname);

		/* 获取mysql连接句柄 */
		sql::Connection* Get();

		/* Query - executeQuery(String sql); 执行SQL查询，并返回ResultSet 对象 */
		sql::ResultSet* query(std::string state = "");

		/* Save - executeUpdate(String sql); 可执行增，删，改，返回执行受到影响的行数 */
		int save(std::string state = "");

		/* Exec - execute(String sql); 可执行任何SQL语句，返回一个布尔值，表示是否返回ResultSet */
		bool exec(std::string state = "");

		/* State SQL执行语句 */
		MySql* state(const std::string& state);

		/* Bind 参数绑定 */
		MySql* bind(unsigned int parameterIndex, int32_t value); // setInt
		MySql* bind(unsigned int parameterIndex, const char* value); // setString
		MySql* bind(unsigned int parameterIndex, bool value); // setBoolean	
		MySql* bind(unsigned int parameterIndex, double value); // setDouble
		MySql* bind(unsigned int parameterIndex, std::istream * blob); // setBlob
		MySql* bind_bigint(unsigned int parameterIndex, const sql::SQLString& value); // setBigInt
		MySql* bind_datetime(unsigned int parameterIndex, const sql::SQLString& value); // setDateTime
		MySql* bind_null(unsigned int parameterIndex, int sqlType); // setNull
		MySql* bind_uint(unsigned int parameterIndex, uint32_t value); // setUInt
		MySql* bind_int64(unsigned int parameterIndex, int64_t value); // setInt64
		MySql* bind_uint64(unsigned int parameterIndex, uint64_t value); // setUInt64

																		 /* 事务操作 - 自动提交设置 */
		void setAutoCommit(bool autoCommit = true);

		/* 事务操作 - 设置保存点 */
		void setSavepoint(const std::string& savepoint);

		/* 事务操作 - 提交 */
		void commit();

		/* 事务操作 - 回滚 */
		void rollback(const std::string& savepoint);


	private:
		// 获取mysql驱动
		static sql::Driver* mDriver;

	private:
		// 数据库连接句柄
		sql::Connection* mConn = NULL;
		sql::Statement* mState = NULL;
		sql::PreparedStatement* mPrep_stmt = NULL;
		sql::ResultSet* mRes = NULL;
		// 存放保存点Map
		std::map<std::string, sql::Savepoint*> mSavepointMap;
		// insert 操作返回的自增主键ID
		uint64_t mPrimaryKey = 0;
	};

}



#endif // !MYSQL_H



