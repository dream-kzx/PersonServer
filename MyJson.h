#ifndef PERSONSERVER_MYJSON_H
#define PERSONSERVER_MYJOSN_H
#include <string>

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace lookupman {
class MyJson {
 public:
  explicit MyJson(int status);
  MyJson(int status, int status_code);
  MyJson(int status, int status_code, const std::string& message);

  ~MyJson() = default;

  void StartSubObject(const std::string& key) {
    ++number_;
    writer_.Key(key.data());
    writer_.StartObject();
  }
  void EndSubObject() {
    --number_;
    writer_.EndObject();
  }

  void StartSubArray(const std::string& key) {
    ++number_;
    writer_.Key(key.data());
    writer_.StartArray();
  }
  void EndSubArray() {
    --number_;
    writer_.EndArray();
  }

  void AddValue(const std::string& key, int value);
  void AddValue(const std::string& key, const std::string& value);
  void AddValue(const std::string& key, double value);

  const std::string GetJsonString();

 private:
  rapidjson::StringBuffer buffer_;
  rapidjson::Writer<rapidjson::StringBuffer> writer_;
  int number_;

  void init();
};

}  // namespace lookupman

#endif
