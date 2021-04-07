//++Base64.h

#pragma once
#include <string>


class CBase64
{
public:
public:
	CBase64();
	~CBase64();

	/*����
	DataByte
	[in]��������ݳ���,���ֽ�Ϊ��λ
	*/
	std::string Encode(const char* Data, int DataByte);

	/*����
	DataByte
	[in]��������ݳ���,���ֽ�Ϊ��λ
	OutByte
	[out]��������ݳ���,���ֽ�Ϊ��λ,�벻Ҫͨ������ֵ����
	������ݵĳ���
	*/
	std::string Decode(const char* Data, int DataByte, int& OutByte);

};