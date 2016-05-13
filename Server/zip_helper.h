
#ifndef __ZIP_HELPER
#define __ZIP_HELPER

#include <vector>
#include <string>

//1.暂时不支持子目录
//注意: 因为使用了zlib库,使用时加上预编译宏 ZLIB_WINAPI
class ZipHelper
{
public:
	ZipHelper(){}
	~ZipHelper(){}

	//path: utf8 path
	ZipHelper& AddFile(const char* input_path);
	//output_path :utf8 path
	bool ToZip(const char* output_path);

private:
	std::vector<std::string> files_;
};

class UnZipHelper
{
private:
	std::string ZipFileName;
public:
	UnZipHelper(){}
	~UnZipHelper(){}
	UnZipHelper& OpenZip(const char* input_path);
	std::string UnZip();
};

#endif