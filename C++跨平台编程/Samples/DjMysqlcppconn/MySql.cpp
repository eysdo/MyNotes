#include "MySql.h"
#include "Config.h"
using namespace sql;
using namespace std;

namespace DjMysqlcppconn
{
	// ��ȡmysql����
	sql::Driver* MySql::mDriver = ::get_driver_instance();

	MySql::MySql()
	{
		// ��������
		mConn = mDriver->connect(MYSQL_URL, MYSQL_USER, MYSQL_PASS);
		mState = mConn->createStatement();
	}

	/* ��ȡmysql���� */
	Connection* MySql::Get()
	{
		return mConn;
	}

	/* �ռ��ͷ� */
	MySql::~MySql()
	{
		// �������
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

	/* �л����ݿ� */
	void MySql::use(const std::string& dbname)
	{
		mConn->setSchema(dbname);
	}

	/* Query ��ѯ - ���ؿռ����迼���ڴ��ͷ�! */
	sql::ResultSet* MySql::query(std::string state)
	{
		if (!state.empty())
			mRes = mState->executeQuery(state);
		else
			mRes = mPrep_stmt->executeQuery();
		return mRes;
	}

	/* State SQLִ����� */
	MySql* MySql::state(const std::string& prep_stmt)
	{
		if (prep_stmt.empty())
			throw std::runtime_error("MySql* MySql::state(std::string& prep_stmt)>> prep_stmt is NULL!!");
		mPrep_stmt = mConn->prepareStatement(prep_stmt);
		return this;
	}

	/* Bind ������ */
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

	/* Save - executeUpdate(String sql); ��ִ������ɾ���ģ�����ִ���ܵ�Ӱ������� */
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

	/* Exec - execute(String sql); ��ִ���κ�SQL��䣬����һ������ֵ����ʾ�Ƿ񷵻�ResultSet */
	bool MySql::exec(std::string state)
	{
		if (!state.empty())
			return mState->execute(state);
		return mPrep_stmt->execute();
	}

	/* ������� - �Զ��ύ���� */
	void MySql::setAutoCommit(bool autoCommit)
	{
		mConn->setAutoCommit(autoCommit);
	}

	/* ������� - ���ñ���� */
	void MySql::setSavepoint(const std::string& savepoint)
	{
		if (savepoint.empty())
			throw std::runtime_error("void MySql::setSavepoint(const std::string& savepoint) >> savepoint is NULL!!");
		mSavepointMap.insert(make_pair(savepoint, mConn->setSavepoint(savepoint)));
	}

	/* ������� - �ύ */
	void MySql::commit()
	{
		mConn->commit();
		for (map<std::string, sql::Savepoint*>::iterator it = mSavepointMap.begin();
			it != mSavepointMap.end(); it++)
		{
			mConn->releaseSavepoint(it->second);
		}
		mSavepointMap.clear(); // ��������
	}

	/* ������� - �ع� */
	void MySql::rollback(const std::string& savepoint)
	{
		map<std::string, sql::Savepoint*>::iterator it = mSavepointMap.find(savepoint);
		mConn->rollback(it->second);
		mConn->releaseSavepoint(it->second);
		mSavepointMap.erase(it); // ɾ����ǰ�洢�ڵ�
	}
}
