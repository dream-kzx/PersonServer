#pragma execution_character_set("utf-8")
#include "Controller.h"

#include "MyJson.h"
#include "Tools.h"

void ErrorRequest(httplib::Response& response) {
  lookupman::MyJson json_builder(404);
  std::string msg = json_builder.GetJsonString();
  response.set_content(msg, "application/json; charset=utf-8");
};

void LoginExpire(httplib::Response& response) {
  lookupman::MyJson json_builer(200, 0, "用户凭证已失效，请重新登录!");
  const auto msg = json_builer.GetJsonString();
  response.set_content(msg, "application/json; charset=utf-8");
}

void StaffLogin(const httplib::Request& request, httplib::Response& response) {
  //验证头部参数
  if (!request.has_file("username") || !request.has_file("password")) {
    ErrorRequest(response);
    return;
  }

  auto username = request.get_file_value("username").content;
  auto password = request.get_file_value("password").content;

  std::string sql_string = "select * from staff where account_number = '" +
                           username + "' and password = '" + password + "'";

  //查询用户是否存在
  auto select_result = sqlEngine.Select(sql_string);

  if (select_result == nullptr) {
    ErrorRequest(response);
    return;
  }

  //生成token并返回
  while (sqlite3_step(select_result.get()) == SQLITE_ROW) {
    std::string account_number =
        (const char*)sqlite3_column_text(select_result.get(), 1);

    auto token = lookupman::GenerateToken(account_number + "staff");
    k_server.PutTokenInStaff(token, account_number,
                             lookupman::KGetCurrentTimestamp());

    lookupman::MyJson json_builder(200, 1);
    json_builder.StartSubObject("message");
    json_builder.AddValue("token", token);
    json_builder.EndSubObject();

    std::string msg = json_builder.GetJsonString();
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
    ErrorRequest(response);
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

void StaffGetMessage(const httplib::Request& request,
                     httplib::Response& response) {
  if (!request.has_file("token")) {
    ErrorRequest(response);
    return;
  }

  std::string token = request.get_file_value("token").content;

  if (token.empty()) {
    ErrorRequest(response);
    return;
  }

  const auto account_number = k_server.GetUserInTokenInStaff(token);
  if (account_number.empty()) {
    LoginExpire(response);
    return;
  }

  std::string sql_string =
      "select name,sex,phone,head_url from staff where account_number = '" +
      account_number + "'";

  auto result = sqlEngine.Select(sql_string);

  if (sqlite3_step(result.get()) != SQLITE_ROW) {
    lookupman::MyJson json_builer(500, 0, "服务器内部错误！");
  }

  std::string name = (const char*)sqlite3_column_text(result.get(), 0);
  std::string sex = (const char*)sqlite3_column_text(result.get(), 1);
  std::string phone = (const char*)sqlite3_column_text(result.get(), 2);
  std::string head_url = (const char*)sqlite3_column_text(result.get(), 3);

  lookupman::MyJson json_builder(200, 1);
  json_builder.StartSubObject("message");
  json_builder.AddValue("name", name);
  json_builder.AddValue("sex", sex);
  json_builder.AddValue("phone", phone);
  json_builder.AddValue("head_url", head_url);
  json_builder.EndSubObject();
  const auto msg = json_builder.GetJsonString();

  response.set_content(msg, "application/json; charset=utf-8");
}

void StaffUpdateMessage(const httplib::Request& request,
                        httplib::Response& response) {
  if (!request.has_file("token")) {
    ErrorRequest(response);
    return;
  }

  auto token = request.get_file_value("token").content;
  auto account_number = k_server.GetUserInTokenInStaff(token);

  if (account_number.empty()) {
    LoginExpire(response);
    return;
  }

  std::string name;
  std::string sex;
  std::string phone;
  std::string head_url;
  if (request.has_file("name")) name = request.get_file_value("name").content;
  if (request.has_file("sex")) sex = request.get_file_value("sex").content;
  if (request.has_file("phone"))
    phone = request.get_file_value("phone").content;
  if (request.has_file("head_url"))
    head_url = request.get_file_value("head_url").content;

  if (name.empty() && sex.empty() && phone.empty() && head_url.empty()) {
    lookupman::MyJson json_builder(200, 1, "更新成功！");
    std::string msg = json_builder.GetJsonString();
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  }

  std::string sql_string = "update staff set ";
  int i = 0;
  if (!name.empty()) {
    sql_string += "name = '" + name + "'";
    ++i;
  }
  if (!sex.empty()) {
    if (i != 0) sql_string += ",";
    sql_string += "sex = '" + sex + "'";
    ++i;
  }
  if (!phone.empty()) {
    if (i != 0) sql_string += ",";
    sql_string += "phone = '" + phone + "'";
    ++i;
  }
  if (!head_url.empty()) {
    if (i != 0) sql_string += ",";
    sql_string += "head_url = '" + head_url + "'";
    ++i;
  }
  

  sql_string += " where account_number = '" + account_number + "'";

  if (sqlEngine.Update(sql_string)) {
    lookupman::MyJson json_builder(200, 1, "更新成功！");
    std::string msg = json_builder.GetJsonString();
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  } else {
    lookupman::MyJson json_builder(500, 0, "服务器内部错误！");
    std::string msg = json_builder.GetJsonString();
    response.set_content(msg, "application/json; charset=utf-8");
    return;
  }
}
