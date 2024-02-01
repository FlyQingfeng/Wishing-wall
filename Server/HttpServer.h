#pragma once

#include <iostream>
#include "httplib.h"
#include <algorithm>//算法库
#include "sqlconnection.h"
#include "wishingjson.hpp"

#define MaxIdCodeLen 32
#define MinIdCodeLen 10
#define MaxWishLen 60

#define ConfigPath "./config.json"

struct SqlInfo
{
	string ip;
	string username;
	string pass;
	string database;
	int port;
};

//SqlConn.CreatConnect("127.0.0.1", "root", "123456", "wishingwall", 3306);



using namespace httplib;



struct User
{
	string idcode="";
	string name="";
	string pass = "123456";
};

struct UserData {
	User user;
	int WishNum = 0;
	int LikeNum = 0;
	int CommentNum = 0;
};

class HttpServer
{
public:
	HttpServer(std::string host,int port);
	HttpServer();
	~HttpServer()
	{
		
	}
	bool Init();//初始化服务器
	void Run();//运行服务器
	bool UserIsNull(User user);//用户为空
	User GetUeserFromSql(string idcode);//获得用户从算数据库
	vector<User> GetUsersFromSqlBycondition(string conditions="");//按条件获得用户从算数据库
	vector<User> GetRandomUsersFromSql(int num=0);//随机获得用户从数据库
	bool Registered(User user);//注册用户

	vector<Wjson::Wishing> GetUserWishings(string idcode);//从数据库获得用户愿望

	UserData GetUserData(string idcode);//获得用户的信息

	static Json::Value MakeRegisteredJson(bool isSuccessful, string mes, User user = {"","",""});//创建注册返回json格式

	static Json::Value MakeLoginJson(bool isSuccessful,string key, string mes);//创建登录返回json格式

	bool MakeWishing(string key,string wish);//许愿

	bool LikeWish(string mkey, string idcode,string wishname);//点赞愿望

	bool CommentWish(string mkey, string mcomment, string idcode, string wishname);//评论愿望

	bool UpdateUserWishings(string idcode, vector<Wjson::Wishing> wishings);//更新用户的许愿数据

	bool DeleteUserWishing(string key, string wishname);

	static string NowTime(string sy= "%Y-%m-%d %H:%M:%S");//按照格式获取当前时间

	//创建用户愿望对象
	/*{
		"idcode": "flyqingfeng",
			"name" : "qf",
			"wishings" : [
		{
			"Comments": [
			{
				"Comment": "666",
					"idcode" : "qf",
					"Time":"2024/1/8 12:00"
			}
			] ,
				"LikeNum": 20,
				"Wishing" : "发财"
		}
			]
	}*/
	Json::Value MakeUserWishingObject(User user);//创建用户愿望对象json

	static string MakeSetWishingSql(string idcode, vector<Wjson::Wishing> wishings);//创建添加愿望sql语句



protected:
	Server mSvr;
	std::string mHost;
	int mPort;
	SqlInfo msqlinfo;

};

