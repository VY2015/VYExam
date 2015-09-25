#include <string>

#pragma once
class __declspec(dllexport) CEncryptyFile
{
public:
	CEncryptyFile();
	~CEncryptyFile();

	//strOutFilePath-输出文件（为空则不生成文件），bEncrypt true-加密，false-解密
	std::string		AesEncryptorDecryptFile(std::string strFilePath, std::string key,
		std::string strOutFilePath = "", bool bEncrypt = false);	
	
private:
	std::string		AesEncrypt(const std::string& source, const std::string& key);
	std::string		AesDecrypt(const std::string& ciphertext, const std::string& key);	
};

