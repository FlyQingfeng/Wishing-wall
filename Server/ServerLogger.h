#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <stdarg.h>

using namespace std;

template<typename ... Args>
string formatLogMessage(const Args&... args) {
	ostringstream oss;
	((oss << args), ...);
	return oss.str();
}

class ServerLogger
{
public:
	
	template<typename ... Args>
	static void log(const Args&... args) {
		string message = formatLogMessage(args...);
		// ��ȡ��ǰʱ���
		auto now = chrono::system_clock::now();
		// ת��Ϊtime_t
		time_t now_time = chrono::system_clock::to_time_t(now);
		// ʹ��localtime_s
		tm time_info;
		localtime_s(&time_info, &now_time);
		// ����һ������ַ�����
		ostringstream oss;
		// ʹ��put_time��ʱ��ת��Ϊ�ַ���
		oss << put_time(&time_info, "%Y-%m-%d %H:%M:%S");
		// ��ȡ�ַ���
		string time_str = oss.str();
		message = "[" + time_str + "]:" + message;
		cout << message << endl;
		ofstream logt("log.txt",ios::app);
		if (logt.is_open()){
			logt << message << endl;
			logt.close();
		}
	}

public:
	ServerLogger();
	~ServerLogger();

};

