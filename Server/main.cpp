#include <iostream>
#include "HttpServer.h"


int main()
{
	HttpServer* mServer = new HttpServer();
	if (mServer->Init())
	{
		mServer->Run();
	}
	return 0;
}