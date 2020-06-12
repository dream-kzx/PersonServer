//该类为工具类，包含了GBK和UTF-8编码互转函数、生成Token的函数、
//获取当前系统时间、获取当前系统时间戳函数
//
//

#ifndef PERSONSERVER_TOOL_H
#define PERSONSERVER_TOOL_H
#include <string>

namespace lookupman {

//用于生成Token的固定字符串
const std::string FixedString = "lookupman";

std::string GBKToUTF8(const char* strGBK);

std::string UTF8ToGBK(const char* strUTF8);

//获取当前的系统时间，格式为2020-6-11 14:43:11
std::string KGetCurrentTime();

//获取当前的系统时间戳
std::string KGetCurrentTimestamp();

std::string GenerateToken(const std::string& key_message);

std::string encode_json(int status);

std::string encode_json(int status, int status_code,
                        const std::string& message);
}  // namespace lookupman

#endif
