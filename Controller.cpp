#include "Controller.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

void StaffLogin(const httplib::Request& request, httplib::Response& response) {
  // if (!request.has_param("username")) {
  //  response.set_content(request.body, "text/plain");
  //  return;
  //}

  if (!request.has_file("username") || !request.has_file("password")) {
    response.set_content(request.body, "text/plain");
    return;
  }

  auto username = request.get_file_value("username").content;
  auto password = request.get_file_value("password").content;

  std::string sql_string = "select * from staff where account_number = '" +
                           username + "' and password = '" + password + "'";

  auto result = sqlEngine.Select(sql_string);

  while (sqlite3_step(result.get()) == SQLITE_ROW) {
    int id = sqlite3_column_int(result.get(), 0);

    printf("%d\n", id);
    response.set_content(std::to_string(id), "text/plain");
  }

  response.set_content(request.body, "text/plain");
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
  writer.String(message.c_str());

  writer.EndObject();

  return buffer.GetString();
}

void StaffSignIn(const httplib::Request& request, httplib::Response& response) {
  if (!request.has_file("username") || !request.has_file("password") ||
      !request.has_file("name") || !request.has_file("sex") ||
      !request.has_file("phone")) {

    std::string msg = encode_json(404);
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  }

  auto username = request.get_file_value("username").content;

  std::string sql_string =
      "select id from staff where account_number='" + username + "'";

  int result = sqlEngine.Find(sql_string);

  if (result != -1 && result != -2) {
    std::string msg = encode_json(200, 0, "The user already exists");
    response.set_content(msg, "application/json; charset=utf-8");
    return;

  } else if (result == -2) {
    std::string msg = encode_json(500, 0, "server error");
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  }

  auto password = request.get_file_value("password").content;
  auto name = request.get_file_value("name").content;
  //auto sex = request.get_file_value("sex").content;
  std::string sex = "man";

  auto phone = request.get_file_value("phone").content;

  std::string head_url = "11111";
  if (request.has_file("head_url")) {
    head_url = request.get_file_value("head_url").content;
  }

  int level = 0;
  std::string create_time = lookupman::getCurrentTime();

  char sql_str[500];
  sprintf_s(sql_str, 500,
            "insert into staff (account_number, password, name, sex, phone, head_url, level, create_time)values('%s','%s','%s','%s','%s','%s',%d,'%s')",
            username.data(), password.data(), name.data(), sex.data(),
            phone.data(), head_url.data(),
            level, create_time.data());

  sql_string = sql_str;

  bool insert_status = sqlEngine.Insert(sql_string);

  if (insert_status) {

    std::string msg = encode_json(200, 1, "sign in success");
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  } else {

    std::string msg = encode_json(500, 1, "server error");
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  }
}