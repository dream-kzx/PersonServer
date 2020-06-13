#include "Tools.h"

#include <Windows.h>
#include <rapidjson/writer.h>

#include "MyCrypto.h"

namespace lookupman {

std::string GBKToUTF8(const char* strGBK) {
  int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
  wchar_t* wstr = new wchar_t[len + 1];

  memset(wstr, 0, len + 1);
  MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
  len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);

  char* str = new char[len + 1];
  memset(str, 0, len + 1);
  WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
  std::string strTemp = str;

  if (wstr) delete[] wstr;
  if (str) delete[] str;

  return strTemp;
}

std::string UTF8ToGBK(const char* strUTF8) {
  int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
  wchar_t* wszGBK = new wchar_t[len + 1];

  memset(wszGBK, 0, len * 2 + 2);
  MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
  len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);

  char* szGBK = new char[len + 1];
  memset(szGBK, 0, len + 1);
  WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
  std::string strTemp(szGBK);

  if (wszGBK) delete[] wszGBK;
  if (szGBK) delete[] szGBK;

  return strTemp;
}

std::string KGetCurrentTime() {
  auto current_time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct tm ptm;
  localtime_s(&ptm, &current_time);

  char date[60] = {0};
  sprintf_s(date, 60, "%d-%02d-%02d %02d:%02d:%02d", (int)ptm.tm_year + 1900,
            (int)ptm.tm_mon + 1, (int)ptm.tm_mday, (int)ptm.tm_hour,
            (int)ptm.tm_min, (int)ptm.tm_sec);

  return std::string(date);
}

std::chrono::nanoseconds KGetCurrentTimestamp() {
  std::chrono::nanoseconds duration =
      std::chrono::system_clock::now().time_since_epoch();

  return duration;
}

// Token生成格式，用户名+时间戳+FixedString， 然后用sha1生成hash值
std::string GenerateToken(const std::string& key_message) {
  std::string temp = key_message + std::to_string(KGetCurrentTimestamp().count())+ FixedString;

  return MyCrypto::Hash(temp);
}

std::string encode_json(int status) {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  writer.StartObject();

  writer.Key("status");
  writer.Int(status);

  writer.EndObject();

  return buffer.GetString();
}

std::string encode_json(int status, int status_code,
                        const std::string& message) {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  writer.StartObject();

  writer.Key("status");
  writer.Int(status);

  writer.Key("status_code");
  writer.Int(status_code);

  writer.Key("message");
  // writer.StartObject();
  writer.String(message.c_str());
  // writer.EndObject();

  writer.EndObject();

  return buffer.GetString();
}

}  // namespace lookupman
