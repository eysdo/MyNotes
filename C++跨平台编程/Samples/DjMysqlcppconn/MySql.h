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

		/* �л����ݿ� */
		void use(const std::string& dbname);

		/* ��ȡmysql���Ӿ�� */
		sql::Connection* Get();

		/* Query - executeQuery(String sql); ִ��SQL��ѯ��������ResultSet ���� */
		sql::ResultSet* query(std::string state = "");

		/* Save - executeUpdate(String sql); ��ִ������ɾ���ģ�����ִ���ܵ�Ӱ������� */
		int save(std::string state = "");

		/* Exec - execute(String sql); ��ִ���κ�SQL��䣬����һ������ֵ����ʾ�Ƿ񷵻�ResultSet */
		bool exec(std::string state = "");

		/* State SQLִ����� */
		MySql* state(const std::string& state);

		/* Bind ������ */
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

																		 /* ������� - �Զ��ύ���� */
		void setAutoCommit(bool autoCommit = true);

		/* ������� - ���ñ���� */
		void setSavepoint(const std::string& savepoint);

		/* ������� - �ύ */
		void commit();

		/* ������� - �ع� */
		void rollback(const std::string& savepoint);


	private:
		// ��ȡmysql����
		static sql::Driver* mDriver;

	private:
		// ���ݿ����Ӿ��
		sql::Connection* mConn = NULL;
		sql::Statement* mState = NULL;
		sql::PreparedStatement* mPrep_stmt = NULL;
		sql::ResultSet* mRes = NULL;
		// ��ű����Map
		std::map<std::string, sql::Savepoint*> mSavepointMap;
		// insert �������ص���������ID
		uint64_t mPrimaryKey = 0;
	};

}



#endif // !MYSQL_H



