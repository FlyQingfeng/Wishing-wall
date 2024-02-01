#include "sqlconnection.h"

sqlconnection::sqlconnection()
{
}

sqlconnection::~sqlconnection()
{
	
}

bool sqlconnection::CreatConnect(string host, string user, string pass, string database, int port)
{
	SQL = mysql_init(NULL);
	//mysql_options(SQL, MYSQL_SET_CHARSET_NAME, "GBK");
	mysql_options(SQL, MYSQL_SET_CHARSET_NAME, "utf8mb4");

	if (!mysql_real_connect(SQL, host.c_str(), user.c_str(), pass.c_str(), database.c_str(), port, NULL, 0)) {
		ServerLogger::log("Err", mysql_error(SQL));
		return false;
	}
	mhost = host;
	muser = user;
	mpass = pass;
	mdatabase = database;
	mport = port;

	return true;
}


bool sqlconnection::UpdateQuery(string sql, string& errstring)
{
	if (SQL == NULL)
	{
		mysql_real_connect(SQL, mhost.c_str(), muser.c_str(), mpass.c_str(), mdatabase.c_str(), mport, NULL, 0);
	}

	if (mysql_query(SQL, sql.c_str()))
	{
		errstring = string(mysql_error(SQL));
		ServerLogger::log("Err:", mysql_error(SQL));
		return false;
	}
	return true;
}

bool sqlconnection::SelectQuery(string sql, vector<SqlDataRow>& datarow, string& errstring)
{
	if (SQL==NULL)
	{
		mysql_real_connect(SQL, mhost.c_str(), muser.c_str(), mpass.c_str(), mdatabase.c_str(), mport, NULL, 0);
	}

	if (mysql_query(SQL, sql.c_str()))
	{
		errstring = string(mysql_error(SQL));
		ServerLogger::log("Err:", mysql_error(SQL));
		return false;
	}
	MYSQL_RES* res = mysql_store_result(SQL);
	if (res == NULL)
	{
		ServerLogger::log("操作成功");
		return true;
	}
	MYSQL_ROW row;
	// 获取结果集中的列数
	int num_fields = mysql_num_fields(res);
	while ((row = mysql_fetch_row(res)))
	{
		SqlDataRow mrow;
		for (int i = 0; i < num_fields; i++)
		{
			mrow.RowString.push_back(row[i]);
			//cout << " " << row[i];
		}
		datarow.push_back(mrow);
	}
	return true;
}
void sqlconnection::Close()
{
	if (mysql_ping(SQL)) {
		// 连接无效，不能关闭
		std::cerr << "Cannot close the connection because it is not valid." << std::endl;
	}
	else {
		// 连接有效，可以关闭
		mysql_close(SQL);
	}
}


//
//bool sqlconnection::Query()
//{
//	string sql = "SHOW COLUMNS FROM userinfo";
//	if (mysql_query(SQL, sql.c_str()))
//	{
//		cout << "Err" << mysql_error(SQL) << endl;
//		return -1;
//	}
//	MYSQL_RES* res = mysql_store_result(SQL);
//	if (res == NULL)
//	{
//		cout << "操作成功" << endl;
//		return 0;
//	}
//	MYSQL_ROW row;
//	// 获取结果集中的列数
//	int num_fields = mysql_num_fields(res);
//	cout << "列数：" << num_fields << endl;
//	while ((row = mysql_fetch_row(res)))
//	{
//		cout << " name:" << row[0] << endl;
//	}
//	return false;
//}
