#include "MyJson.h"

namespace lookupman {
MyJson::MyJson(int status) : writer_(buffer_), number_(0) {
  writer_.StartObject();
  writer_.Key("status");
  writer_.Int(status);

}

MyJson::MyJson(int status, int status_code) : writer_(buffer_), number_(0) {
  writer_.StartObject();
  writer_.Key("status");
  writer_.Int(status);

  writer_.Key("status_code");
  writer_.Int(status_code);
}

MyJson::MyJson(int status, int status_code, const std::string& message)
    : writer_(buffer_), number_(0) {
  writer_.StartObject();
  writer_.Key("status");
  writer_.Int(status);

  writer_.Key("status_code");
  writer_.Int(status_code);

  writer_.Key("message");
  writer_.String(message.data());
}

void MyJson::AddValue(const std::string& key, int value) {
  writer_.Key(key.data());
  writer_.Int(value);
}

void MyJson::AddValue(const std::string& key, double value) {
  writer_.Key(key.data());
  writer_.Double(value);
}

void MyJson::AddValue(const std::string& key, const std::string& value) {
  writer_.Key(key.data());
  writer_.String(value.data());
}

const std::string MyJson::GetJsonString() {
  writer_.EndObject();
  if (number_ != 0) return "";

  return buffer_.GetString();
}

}  // namespace lookupman
