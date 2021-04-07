#include <iostream>
#include <stdio.h>
#include "Base64_1.h"
#include <winsock2.h>  
#include "json1.hpp"
#include <windows.h>
#include "sql.h"
#include "sqltypes.h"
#include "sqlext.h"
#include"sqlucode.h"
#include"odbcinst.h" 
//mysql所需的头文件和库文件  
#include "winsock.h" 
#include "mysql.h" 
#pragma comment(lib,"ws2_32.lib")  
using json = nlohmann::json;
using namespace std;
char revData[3888888];
MYSQL mydata;

bool WritePhotoFile(std::basic_string<TCHAR> strFileName, std::string& strData)
{
	HANDLE hFile;
	hFile = CreateFile(strFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	CBase64 base64;

	int datalen(0);
	DWORD dwritelen(0);
	std::string strdcode = base64.Decode(strData.data(), strData.size(), datalen);
	if (!WriteFile(hFile, strdcode.data(), datalen, &dwritelen, NULL))
	{
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);
	return true;
}


int connectToDB(string host_,string user_,string password_,string dbs_,  unsigned int port_)
{
	const char* host = host_.c_str();
	const char* user = user_.c_str();
	const char* upwd = password_.c_str();
	const char* dbs = dbs_.c_str();
	unsigned int port = port_;
	

	if (mysql_library_init(0, NULL, NULL) == 0) {
		cout << "mysql_library_init() successfully!" << endl;
	}
	else {
		cout << "mysql_library_init() failed!!" << endl;
		return -1;
	}

	//初始化数据结构
	if (mysql_init(&mydata) != NULL) {
		cout << "init data structure successfully!" << endl;
	}
	else {
		cout << "init data structure failed" << endl;
		return -1;
	}

	//设置数据库编码类型
	if (mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "utf8") == 0) {
		cout << "设置数据库编码类型成功！" << endl;
	}
	else {
		cout << "设置数据库编码类型失败！" << endl;
		return -1;
	}

	//连接数据库
	if (NULL != mysql_real_connect(&mydata, host, user, upwd, dbs, port, NULL, 0)) {
		cout << "connect to db successfully！" << endl;
	}
	else {
		cout << "connect to db failed！" << endl;
		return -1;
	}


}



int main(int argc, char* argv[])
{
	connectToDB("127.0.0.1", "root", "1230.0", "software", 3306);
	//初始化WSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字  
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	if (::bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听  
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据  
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);


	while (true)
	{
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//接收数据  
		int ret = recv(sClient, revData, 3888888, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			json o = json::parse(revData);
			if (o.size() == 0)
			{
				cout << "NULL json!" << endl;
				continue;
			}
			json::iterator it = o.begin();
			if (it.key() == "key")
			{

			}

		}


		closesocket(sClient);
	}


	closesocket(slisten);
	WSACleanup();
	mysql_close(&mydata);
	mysql_library_end();
	return 0;
}


int tmp() {



	string sqlstr_select = " select * from mytable;";

	MYSQL_RES* result = NULL;
	if (mysql_query(&mydata, sqlstr_select.c_str()) == 0) {
		cout << "query successfully" << endl;

		//一次性取得数据集
		result = mysql_store_result(&mydata);

		//取得并打印行数
		my_ulonglong rowcount = mysql_num_rows(result);
		cout << "row count: " << rowcount << endl;

		//取得并打印各字段的名称
		unsigned int fieldcount = mysql_num_fields(result);
		MYSQL_FIELD* field = NULL;

		for (unsigned int i = 0; i < fieldcount; i++) {
			field = mysql_fetch_field_direct(result, i);
			cout << field->name << "\t\t";
		}
		cout << endl;

	}
	else {
		cout << "mysql_query() select data failed" << endl;
		mysql_close(&mydata);
		return -1;
	}

	return 0;
}