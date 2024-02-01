#pragma once
#include <iostream>
#include <mysql.h>
#include <string>
#include <vector>
#include "ServerLogger.h"

using namespace std;

struct SqlDataRow
{
	vector<string> RowString;
};

class sqlconnection
{
public:
	sqlconnection();
	~sqlconnection();
	bool CreatConnect(string host, string user, string pass, string database, int port = 3306);
	bool UpdateQuery(string sql, string& errstring);
	bool SelectQuery(string sql, vector<SqlDataRow>& datarow, string& errstring);
	void Close();
protected:
	MYSQL* SQL;
	string mhost;
	string muser;
	string mpass;
	string mdatabase;
	int mport;
};

