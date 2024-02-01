#pragma once

#include <iostream>
#include "httplib.h"
#include <algorithm>//�㷨��
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
	bool Init();//��ʼ��������
	void Run();//���з�����
	bool UserIsNull(User user);//�û�Ϊ��
	User GetUeserFromSql(string idcode);//����û��������ݿ�
	vector<User> GetUsersFromSqlBycondition(string conditions="");//����������û��������ݿ�
	vector<User> GetRandomUsersFromSql(int num=0);//�������û������ݿ�
	bool Registered(User user);//ע���û�

	vector<Wjson::Wishing> GetUserWishings(string idcode);//�����ݿ����û�Ը��

	UserData GetUserData(string idcode);//����û�����Ϣ

	static Json::Value MakeRegisteredJson(bool isSuccessful, string mes, User user = {"","",""});//����ע�᷵��json��ʽ

	static Json::Value MakeLoginJson(bool isSuccessful,string key, string mes);//������¼����json��ʽ

	bool MakeWishing(string key,string wish);//��Ը

	bool LikeWish(string mkey, string idcode,string wishname);//����Ը��

	bool CommentWish(string mkey, string mcomment, string idcode, string wishname);//����Ը��

	bool UpdateUserWishings(string idcode, vector<Wjson::Wishing> wishings);//�����û�����Ը����

	bool DeleteUserWishing(string key, string wishname);

	static string NowTime(string sy= "%Y-%m-%d %H:%M:%S");//���ո�ʽ��ȡ��ǰʱ��

	//�����û�Ը������
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
				"Wishing" : "����"
		}
			]
	}*/
	Json::Value MakeUserWishingObject(User user);//�����û�Ը������json

	static string MakeSetWishingSql(string idcode, vector<Wjson::Wishing> wishings);//�������Ը��sql���



protected:
	Server mSvr;
	std::string mHost;
	int mPort;
	SqlInfo msqlinfo;

};

