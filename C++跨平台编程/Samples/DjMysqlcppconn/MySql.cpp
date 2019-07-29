#include "MySql.h"
#include "Config.h"
using namespace sql;
using namespace std;

namespace DjMysqlcppconn
{
	// 获取mysql驱动
	sql::Driver* MySql::mDriver = ::get_driver_instance();

	MySql::MySql()
	{
		// 建立连接
		mConn = mDriver->connect(MYSQL_URL, MYSQL_USER, MYSQL_PASS);
		mState = mConn->createStatement();
	}

	/* 获取mysql连接 */
	Connection* MySql::Get()
	{
		return mConn;
	}

	/* 空间释放 */
	MySql::~MySql()
	{
		// 清理保存点
		for (map<std::string, sql::Savepoint*>::iterator it = mSavepointMap.begin();
			it != mSavepointMap.end(); it++)
		{
			mConn->releaseSavepoint(it->second);
		}

		if (mConn)
		{
			mConn->close();
			delete mConn;
			mConn = NULL;
		}
		if (mState)
		{
			delete mState;
			mState = NULL;
		}
		if (mPrep_stmt)
		{
			delete mPrep_stmt;
			mPrep_stmt = NULL;
		}
		if (mRes)
		{
			delete mRes;
			mRes = NULL;
		}
	}

	/* 切换数据库 */
	void MySql::use(const std::string& dbname)
	{
		mConn->setSchema(dbname);
	}

	/* Query 查询 - 返回空间无需考虑内存释放! */
	sql::ResultSet* MySql::query(std::string state)
	{
		if (!state.empty())
			mRes = mState->executeQuery(state);
		else
			mRes = mPrep_stmt->executeQuery();
		return mRes;
	}

	/* State SQL执行语句 */
	MySql* MySql::state(const std::string& prep_stmt)
	{
		if (prep_stmt.empty())
			throw std::runtime_error("MySql* MySql::state(std::string& prep_stmt)>> prep_stmt is NULL!!");
		mPrep_stmt = mConn->prepareStatement(prep_stmt);
		return this;
	}

	/* Bind 参数绑定 */
	// setBigInt
	MySql* MySql::bind_bigint(unsigned int parameterIndex, const sql::SQLString& value)
	{
		mPrep_stmt->setBigInt(parameterIndex, value);
		return this;
	}
	// setBlob
	MySql* MySql::bind(unsigned int parameterIndex, std::istream * blob)
	{
		mPrep_stmt->setBlob(parameterIndex, blob);
		return this;
	}
	// setBoolean
	MySql* MySql::bind(unsigned int parameterIndex, bool value)
	{
		mPrep_stmt->setBoolean(parameterIndex, value);
		return this;
	}
	// setDateTime
	MySql* MySql::bind_datetime(unsigned int parameterIndex, const sql::SQLString& value)
	{
		mPrep_stmt->setDateTime(parameterIndex, value);
		return this;
	}
	// setDouble
	MySql* MySql::bind(unsigned int parameterIndex, double value)
	{
		mPrep_stmt->setDouble(parameterIndex, value);
		return this;
	}
	// setInt
	MySql* MySql::bind(unsigned int parameterIndex, int32_t value)
	{
		mPrep_stmt->setInt(parameterIndex, value);
		return this;
	}
	// setUInt
	MySql* MySql::bind_uint(unsigned int parameterIndex, uint32_t value)
	{
		mPrep_stmt->setUInt(parameterIndex, value);
		return this;
	}
	// setInt64
	MySql* MySql::bind_int64(unsigned int parameterIndex, int64_t value)
	{
		mPrep_stmt->setInt64(parameterIndex, value);
		return this;
	}
	// setUInt64
	MySql* MySql::bind_uint64(unsigned int parameterIndex, uint64_t value)
	{
		mPrep_stmt->setUInt64(parameterIndex, value);
		return this;
	}
	// setNull
	MySql* MySql::bind_null(unsigned int parameterIndex, int sqlType)
	{
		mPrep_stmt->setNull(parameterIndex, sqlType);
		return this;
	}
	// setString
	MySql* MySql::bind(unsigned int parameterIndex, const char* value)
	{
		mPrep_stmt->setString(parameterIndex, value);
		return this;
	}

	/* Save - executeUpdate(String sql); 可执行增，删，改，返回执行受到影响的行数 */
	int MySql::save(std::string state)
	{
		int rows = 0;
		if (!state.empty())
		{
			rows = mState->executeUpdate(state);
		}
		else
		{
			rows = mPrep_stmt->executeUpdate();
		}
		return rows;
	}

	/* Exec - execute(String sql); 可执行任何SQL语句，返回一个布尔值，表示是否返回ResultSet */
	bool MySql::exec(std::string state)
	{
		if (!state.empty())
			return mState->execute(state);
		return mPrep_stmt->execute();
	}

	/* 事务操作 - 自动提交设置 */
	void MySql::setAutoCommit(bool autoCommit)
	{
		mConn->setAutoCommit(autoCommit);
	}

	/* 事务操作 - 设置保存点 */
	void MySql::setSavepoint(const std::string& savepoint)
	{
		if (savepoint.empty())
			throw std::runtime_error("void MySql::setSavepoint(const std::string& savepoint) >> savepoint is NULL!!");
		mSavepointMap.insert(make_pair(savepoint, mConn->setSavepoint(savepoint)));
	}

	/* 事务操作 - 提交 */
	void MySql::commit()
	{
		mConn->commit();
		for (map<std::string, sql::Savepoint*>::iterator it = mSavepointMap.begin();
			it != mSavepointMap.end(); it++)
		{
			mConn->releaseSavepoint(it->second);
		}
		mSavepointMap.clear(); // 清理容器
	}

	/* 事务操作 - 回滚 */
	void MySql::rollback(const std::string& savepoint)
	{
		map<std::string, sql::Savepoint*>::iterator it = mSavepointMap.find(savepoint);
		mConn->rollback(it->second);
		mConn->releaseSavepoint(it->second);
		mSavepointMap.erase(it); // 删除当前存储节点
	}
}
