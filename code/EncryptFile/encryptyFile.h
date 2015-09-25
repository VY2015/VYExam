#include <string>

#pragma once
class __declspec(dllexport) CEncryptyFile
{
public:
	CEncryptyFile();
	~CEncryptyFile();

	//strOutFilePath-����ļ���Ϊ���������ļ�����bEncrypt true-���ܣ�false-����
	std::string		AesEncryptorDecryptFile(std::string strFilePath, std::string key,
		std::string strOutFilePath = "", bool bEncrypt = false);	
	
private:
	std::string		AesEncrypt(const std::string& source, const std::string& key);
	std::string		AesDecrypt(const std::string& ciphertext, const std::string& key);	
};

