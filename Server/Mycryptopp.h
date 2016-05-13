#pragma once
#ifndef __MYCRYPTOPP_H_
#define __MYCRYPTOPP_H_

#include <aes.h>
#include <default.h>
#include <filters.h>
#include <files.h>
#include <osrng.h>
#include <rsa.h>
#include <string>
#include <iostream>

using std::string;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::InvertibleRSAFunction;
using CryptoPP::RSA;
using CryptoPP::AES;

class MyAES
{
public:
	byte key[AES::DEFAULT_KEYLENGTH];
	int key_length;
	byte iv[AES::BLOCKSIZE];
	

	MyAES();
	MyAES(byte * key, byte *iv, int length);
	~MyAES();

	//use the key to encrypt the plainText and return the cipher
	string Encrypt(const string &plainText);
	//use the same key to decrypt the cipher and return the recover
	string Decrypt(const string &cipher);
	//use the key to encrypt the file
	bool EncryptFile(const string & inFilename, const string & outFilename);
	//use the key to decyrpt the file
	bool DecryptFile(const string & DecFilename, const string & recoverFilename);
	byte * GenerateKey();
	void SetKey(byte * key, byte * iv, int length);
};

class MyRSA
{
public:
	MyRSA(unsigned int size = 1024);
	MyRSA::MyRSA(InvertibleRSAFunction _rsa_param);
	virtual ~MyRSA();

	//设置Key
	bool MyRSA::SetRSA(InvertibleRSAFunction _rsa_param1);
	//加密，使用public_key
	string encrypt(const string &plain);
	//解密，使用private_key
	string decrypt(const string &cipher);

	//获取public_key的e
	string getPublicExponent();
	//获取public_key的n
	string getModulus();

	string GetPrivateExponent();
private:
	/**随机种子*/
	AutoSeededRandomPool _rng;
	/**rsa参数，含public/private key等*/
	InvertibleRSAFunction _rsa_param;
};

class MyRSAPublic
{
public:
	MyRSAPublic();
	MyRSAPublic(string Modulus,string PublicExponent);
	~MyRSAPublic();
	void SetPublic(string Modulus, string PublicExponent);
	string encrypt(const string &plain);
private:
	AutoSeededRandomPool _rng;
	RSA::PublicKey public_key;
};
#endif