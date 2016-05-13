#include "stdafx.h"
#include "Mycryptopp.h"
#include <stdlib.h>
#include <io.h>
#include <iostream>
#include <sstream>

using std::ostringstream;
using namespace CryptoPP;
using std::string;


/*
*AES
*/
MyAES::MyAES()
{
}
/*
*key iv应使用byte new 
*/
MyAES::MyAES(byte * key1, byte * iv1, int key_length1)
{
	SetKey(key1, iv1, key_length1);
}

MyAES::~MyAES()
{
}

/*
* Description: use key to encrypt 'plainText', return the cipher
* Input:
*  plainText: the string need to be encrypted
* OutPUt:
*  return the cipher
*/
string MyAES::Encrypt(const string & plainText)
{
	string cipher;
	CBC_Mode<AES>::Encryption aesEncryptor(key, key_length, iv);
	//  AESEncryption aesEncryptor; //加密器
	//  aesEncryptor.SetKey( key, key_length );  //设定加密密钥
	StringSource(plainText, true,new StreamTransformationFilter(aesEncryptor,new StringSink(cipher)));
	return cipher;
}

/*
* Description: use the same key to decrypt "cipher" and return the plainText
* Input:
*  cipher: the string to be decrypted
* Output:
*  return the recover
*/
string MyAES::Decrypt(const string & cipher)
{
	string recover;
	CBC_Mode<AES>::Decryption aesDecryptor(key, key_length, iv);
	//AESDecryption aesDecryptor; //解密器
	//aesDecryptor.SetKey( key, key_length );  //设定加密密钥
	StringSource(cipher, true,new StreamTransformationFilter(aesDecryptor,new StringSink(recover)));
	return recover;
}

/*
* Description: use the key to encrypt the 'inFilename' and store the cipher in 'outFilname'
* Input:
*  inFilename: the file need to be encrypted!
*  outFilename: the encrypted file
* OutPut:
*  if encrypt success, return true, or return false
* Others: the function should delete the file : 'inFilename', however I note it
*/
bool MyAES::EncryptFile(const string & inFilename, const string & outFilename)
{
	// check if the file 'inFilename' exists.
	if (access(inFilename.c_str(), 0) == -1)
	{
	//	cout << "The file " << inFilename << " is not exist! " << endl;
		return false;
	}
	// file exists, the encrypt the file
	CBC_Mode<AES>::Encryption aesEncryptor(key, key_length, iv);

	FileSource(inFilename.c_str(), true,new StreamTransformationFilter(aesEncryptor,new FileSink(outFilename.c_str())));
	// remove the file 'inFilename'
	// if(remove(inFilename) == 0) cout << "remove file succeed! " << endl;
	//      else cout << "fail to remove the file " << inFilname << endl;
	// use function remove(), you have to add #include <cstdio> in the .h file
	return true;
}

/*
* Description: use the same key to decrypt the 'decFilename' and create recoverFile
* Input:
*  decFilename: the encrypted file name
*  recoverFilename: the decrypted file name
* OutPut:
*  if decrypted the file successful, return true, else return false
* Others: we should also delete the file 'decFilename'
*/
bool MyAES::DecryptFile(const string & decFilename, const string & recoverFilename)
{
	// check if the file 'decFilename' exists!
	if (access(decFilename.c_str(), 0) == -1)
	{
	//	cout << "The file " << decFilename << " is not exist! " << endl;
		return false;
	}
	// exist , then decrypt the file
	CBC_Mode<AES>::Decryption aesDecryptor(key, key_length, iv);
	FileSource(decFilename.c_str(), true,new StreamTransformationFilter(aesDecryptor,new FileSink(recoverFilename.c_str())));
	return true;
}

byte* MyAES::GenerateKey()
{
	AutoSeededRandomPool rnd;
	byte key1[AES::DEFAULT_KEYLENGTH];
	rnd.GenerateBlock(key1, AES::DEFAULT_KEYLENGTH);

	// Generate a random IV
	byte iv1[AES::BLOCKSIZE]= "Transmission";

	SetKey(key1, iv1, 16);
	return key1;
}

void MyAES::SetKey(byte * key1, byte * iv1, int length1)
{
	memcpy(this->key ,key1, AES::DEFAULT_KEYLENGTH);
	memcpy(this->iv ,iv1, AES::BLOCKSIZE);
	this->key_length = length1;
}

/*
*RSA
*/
MyRSA::MyRSA(unsigned int size)
{
	//初始化rsa参数
	_rsa_param.GenerateRandomWithKeySize(_rng, size);
}

MyRSA::MyRSA(InvertibleRSAFunction _rsa_param1)
{
	this->_rsa_param = _rsa_param1;
}

MyRSA::~MyRSA()
{
}

bool MyRSA::SetRSA(InvertibleRSAFunction _rsa_param1)
{
	this->_rsa_param = _rsa_param1;
	return true;
}

string MyRSA::getPublicExponent()
{
	ostringstream os;
	os << _rsa_param.GetPublicExponent();

	string ret = os.str();
	ret.erase(ret.size() - 1);
	return ret;
}

string MyRSA::getModulus()
{
	ostringstream os;
	os << _rsa_param.GetModulus();

	string ret = os.str();
	ret.erase(ret.length() - 1);
	return ret;
}
string MyRSA::GetPrivateExponent()
{
	ostringstream os;
	os << _rsa_param.GetPrivateExponent();

	string ret = os.str();
	ret.erase(ret.length() - 1);
	return ret;
}
string MyRSA::encrypt(const string & plain)
{
	string cipher;
	RSA::PublicKey public_key(_rsa_param);
	RSAES_OAEP_SHA_Encryptor encryptor(public_key);
	StringSource(plain, true, new PK_EncryptorFilter(_rng, encryptor,new StringSink(cipher)));

	return cipher;
}

string MyRSA::decrypt(const string & cipher)
{
	string recovered;
	RSA::PrivateKey private_key(_rsa_param);
	RSAES_OAEP_SHA_Decryptor decryptor(private_key);
	StringSource(cipher, true, new PK_DecryptorFilter(_rng, decryptor,new StringSink(recovered)));

	return recovered;
}

MyRSAPublic::MyRSAPublic()
{
}

MyRSAPublic::MyRSAPublic(string Modulus, string PublicExponent)
{
	SetPublic(Modulus, PublicExponent);
}

MyRSAPublic::~MyRSAPublic()
{
}

void MyRSAPublic::SetPublic(string Modulus, string PublicExponent)
{
	Integer INTModulus(Modulus.c_str());
	Integer INTPublicExponent(PublicExponent.c_str());
	public_key.SetModulus(INTModulus);
	public_key.SetPublicExponent(INTPublicExponent);
}

string MyRSAPublic::encrypt(const string & plain)
{
	string cipher;
	RSAES_OAEP_SHA_Encryptor encryptor(public_key);
	StringSource(plain, true, new PK_EncryptorFilter(_rng, encryptor, new StringSink(cipher)));

	return cipher;
}
