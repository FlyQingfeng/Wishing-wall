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
		// 获取当前时间点
		auto now = chrono::system_clock::now();
		// 转换为time_t
		time_t now_time = chrono::system_clock::to_time_t(now);
		// 使用localtime_s
		tm time_info;
		localtime_s(&time_info, &now_time);
		// 创建一个输出字符串流
		ostringstream oss;
		// 使用put_time将时间转换为字符串
		oss << put_time(&time_info, "%Y-%m-%d %H:%M:%S");
		// 获取字符串
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

