//++Base64.h

#pragma once
#include <string>


class CBase64
{
public:
public:
	CBase64();
	~CBase64();

	/*编码
	DataByte
	[in]输入的数据长度,以字节为单位
	*/
	std::string Encode(const char* Data, int DataByte);

	/*解码
	DataByte
	[in]输入的数据长度,以字节为单位
	OutByte
	[out]输出的数据长度,以字节为单位,请不要通过返回值计算
	输出数据的长度
	*/
	std::string Decode(const char* Data, int DataByte, int& OutByte);

};