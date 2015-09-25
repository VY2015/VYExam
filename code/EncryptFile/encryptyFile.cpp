#include "encryptyFile.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <openssl\objects.h>
#include <openssl\evp.h>

CEncryptyFile::CEncryptyFile()
{
}


CEncryptyFile::~CEncryptyFile()
{
}

//AES 加密算法
//注意：参数和返回值全部是二进制数据
std::string CEncryptyFile::AesEncrypt(const std::string& source, const std::string& key)
{
	//EVP_des_ecb  EVP_aes_128_ecb,AES/DES两种算法
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
    int ret = EVP_EncryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, (const unsigned char*)key.data(), NULL);
	if (1 != ret)
		return "";

	unsigned char* result = new unsigned char[source.length() + 64]; // 弄个足够大的空间
	int len1 = 0;
	ret = EVP_EncryptUpdate(&ctx, result, &len1, (const unsigned char*)source.data(), source.length());
	if (1 != ret)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		delete[] result;
		return "";
	}

	int len2 = 0;
	ret = EVP_EncryptFinal_ex(&ctx, result + len1, &len2);
	if (1 != ret)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		delete[] result;
		return "";
	}

	ret = EVP_CIPHER_CTX_cleanup(&ctx);
	if (1 != ret)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		delete[] result;
		return "";
	}

	std::string res((char*)result, len1 + len2);
	delete[] result;
	return res;
}

//解密算法
std::string CEncryptyFile::AesDecrypt(const std::string& ciphertext, const std::string& key)
{
	//EVP_des_ecb  EVP_aes_128_ecb,AES/DES两种算法
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
    int ret = EVP_DecryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, (const unsigned char*)key.data(), NULL);
	if (1 != ret)
		return "";

	unsigned char* result = new unsigned char[ciphertext.length() + 64]; // 弄个足够大的空间
	int len1 = 0;
	ret = EVP_DecryptUpdate(&ctx, result, &len1, (const unsigned char*)ciphertext.data(), ciphertext.length());
	if (1 != ret)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		delete[] result;
		return "";
	}

	int len2 = 0;
	ret = EVP_DecryptFinal_ex(&ctx, result + len1, &len2);
	if (1 != ret)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		delete[] result;
		return "";
	}

	ret = EVP_CIPHER_CTX_cleanup(&ctx);
	if (1 != ret)
	{
		delete[] result;
		return "";
	}
	std::string res((char*)result, len1 + len2);
	delete[] result;
	return res;
}


std::string CEncryptyFile::AesEncryptorDecryptFile(std::string strFilePath, std::string key, std::string strOutFilePath, bool bEncrypt)
{
	FILE* fp = NULL;
	if (0 != fopen_s(&fp, strFilePath.c_str(), "rb"))
	{
		return "";
	}

	fseek(fp, 0L, SEEK_END);
	const int length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* buf = new char[length + 64];

	int bytes = fread(buf, 1, length, fp);
	fclose(fp);
	fp = NULL;

	std::string source(buf, bytes); // 二进制数据

	// 加密
//	std::string keyData(key, key.length());
	std::string keyData = key;
	
	std::string enc;
	if (bEncrypt)
	{
		enc = AesEncrypt(source, keyData);
	}
	else
	{
		enc = AesDecrypt(source, keyData);
	}

	if (enc.empty())
	{
		return "";
	}

	// 输出到文件
	if (!strOutFilePath.empty())
	{
		if (0 == fopen_s(&fp, strOutFilePath.c_str(), "wb"))
		{
			fwrite(enc.data(), 1, enc.length(), fp);
			fclose(fp);
			fp = NULL;
		}
	}

	return enc;
}
