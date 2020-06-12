#pragma execution_character_set("utf-8")
#include "Controller.h"

#include "Tools.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"



void StaffLogin(const httplib::Request& request, httplib::Response& response) {
  //验证头部参数
  if (!request.has_file("username") || !request.has_file("password")) {
    std::string msg = lookupman::encode_json(404);
    response.set_content(msg, "text/plain");
    return;
  }

  auto username = request.get_file_value("username").content;
  auto password = request.get_file_value("password").content;

  std::string sql_string = "select * from staff where account_number = '" +
                           username + "' and password = '" + password + "'";

  //查询用户是否存在
  auto select_result = sqlEngine.Select(sql_string);

  if (select_result == nullptr) {
    std::string msg = lookupman::encode_json(404);
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  }

  //生成token并返回
  while (sqlite3_step(select_result.get()) == SQLITE_ROW) {
    std::string account_number =
        (const char*)sqlite3_column_text(select_result.get(), 1);

    auto token = lookupman::GenerateToken(account_number);
    k_server.PutToken(token, account_number);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.StartObject();

    writer.Key("token");
    writer.String(token.data());
   
    writer.EndObject();

    std::string msg = lookupman::encode_json(200, 1, buffer.GetString());
    response.set_content(msg, "application/json;charset=utf-8");
    return;
  }

  std::string msg = lookupman::encode_json(200, 0, "用户名或密码错误！");
  response.set_content(msg, "application/json; charset=utf-8");
  return;
}

void StaffSignIn(const httplib::Request& request, httplib::Response& response) {
  //验证头部参数
  if (!request.has_file("username") || !request.has_file("password") ||
      !request.has_file("name") || !request.has_file("sex") ||
      !request.has_file("phone")) {
    std::string msg = lookupman::encode_json(404);
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  }

  auto username = request.get_file_value("username").content;

  std::string sql_string =
      "select id from staff where account_number='" + username + "'";

  //查询用户是否存在
  int result = sqlEngine.Find(sql_string);

  if (result != -1 && result != -2) {
    std::string msg = lookupman::encode_json(200, 0, "该用户已存在");
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  } else if (result == -2) {
    std::string msg = lookupman::encode_json(500, 0, "server error");
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  }

  auto password = request.get_file_value("password").content;
  auto name = request.get_file_value("name").content;
  auto sex = request.get_file_value("sex").content;
  auto phone = request.get_file_value("phone").content;

  std::string head_url = "11111";
  if (request.has_file("head_url")) {
    head_url = request.get_file_value("head_url").content;
  }

  int level = 0;
  std::string create_time = lookupman::KGetCurrentTime();

  char sql_str[500];
  sprintf_s(sql_str, 500,
            "insert into staff (account_number, password, name, sex, phone, "
            "head_url, level, "
            "create_time)values('%s','%s','%s','%s','%s','%s',%d,'%s')",
            username.data(), password.data(), name.data(), sex.data(),
            phone.data(), head_url.data(), level, create_time.data());

  sql_string = sql_str;

  //将用户信息加入注册表
  bool insert_status = sqlEngine.Insert(sql_string);

  if (insert_status) {
    std::string msg = lookupman::encode_json(200, 1, "sign in success");
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  } else {
    std::string msg = lookupman::encode_json(500, 1, "server error");
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  }
}
