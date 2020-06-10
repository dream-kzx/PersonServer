// ManagerServer.cpp: 定义应用程序的入口点。
//
#include <sqlite3.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>

//#include "admin.h"
//#include "sqlpp11/sqlite3/sqlite3.h"
//#include "sqlpp11/sqlpp11.h"
//#include "staff.h"

const std::string getCurrentTime();

int main() {
  sqlite3 *sql = nullptr;
  const char *path = "database.db";
  int result = sqlite3_open_v2(
      path, &sql, SQLITE_OPEN_READWRITE | SQLITE_OPEN_SHAREDCACHE, nullptr);

  if (result != SQLITE_OK) {
    std::clog << "打开数据库失败！";
  }

  const char *sqlSentence = "select * from admin";
  sqlite3_stmt *stmt = nullptr;

  result = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, nullptr);

  if (result != SQLITE_OK) {
    std::clog << "查询语句执行失败";
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    const unsigned char *account_number = sqlite3_column_text(stmt, 1);

    std::clog << "id: " << id << " account_number: " << account_number
              << std::endl;
  }

  sqlite3_reset(stmt);

  std::string sqlS = "update admin set name = ? where id = ?";
  result = sqlite3_prepare_v2(sql, sqlS.c_str(), -1, &stmt, nullptr);

  result = sqlite3_bind_text(stmt, 1, "ZZXZZZZ", -1, SQLITE_STATIC);
  result = sqlite3_bind_int(stmt, 2, 1);

  if (result == SQLITE_OK && sqlite3_step(stmt) == SQLITE_DONE) {
    std::cout << "更新语句成功" << std::endl;
  } else {
    std::cout << "更新语句失败" << std::endl;
  }

  sqlite3_finalize(stmt);

  if (sql) {
    sqlite3_close_v2(sql);
    sql = nullptr;
  }
}

// namespace sql = sqlpp::sqlite3;

// int main() {
// std::string current_time = getCurrentTime();
// std::cout << current_time << std::endl;
// sql::connection_config config;
// config.path_to_database = "database.db";
// config.flags = SQLITE_OPEN_READWRITE;
// config.debug = false;

// try {
//  sql::connection db(config);

//  PersonServer::Admin admin{};

//  auto current_time = getCurrentTime();

// db(insert_into(admin).set(
//    admin.accountNumber = "44444", admin.password = "123456",
//    admin.name = "kzx", admin.sex = "男", admin.phone = "123456789",
//    admin.headUrl = "https://lookupman.cn", admin.power = 0,
//    admin.createTime = current_time));

// auto multi_insert = insert_into(admin).columns(
//    admin.accountNumber, admin.password, admin.name, admin.sex, admin.phone,
//    admin.headUrl, admin.power, admin.createTime);
// multi_insert.values.add(admin.accountNumber = "2222",
//                        admin.password = "123456", admin.name = "kzx",
//                        admin.sex = "男", admin.phone = "123456789",
//                        admin.headUrl = "https://lookupman.cn",
//                        admin.power = 0, admin.createTime = current_time);

// multi_insert.values.add(admin.accountNumber = "333333",
//                        admin.password = "123456", admin.name = "kzx",
//                        admin.sex = "男", admin.phone = "123456789",
//                        admin.headUrl = "https://lookupman.cn",
//                        admin.power = 0, admin.createTime = current_time);

// db(multi_insert);

/*for (const auto& row :
     db(sqlpp::select(admin.id, admin.accountNumber, admin.password,
admin.name, admin.sex, admin.phone, admin.headUrl) .from(admin))) {
  std::cout << row.name << std::endl;
}*/
// auto result = db(select(all_of(admin)).from(admin).unconditionally());

// for (const auto& row :
//     db(select(all_of(admin)).from(admin).unconditionally())) {
//  std::cout << row.name << std::endl;
//}

// db(update(admin).set(admin.name = "kkkkkk").where(admin.id == 1));

//  db(remove_from(admin).where(admin.id == 3));

//} catch (std::exception& except) {
//  std::cout << except.what() << std::endl;
//}
//}

const std::string getCurrentTime() {
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

// using namespace httplib;

// Server server;
// server.Get("/hi", [](const Request& req, Response& res) {
//  res.set_content("Hello World!", "text/plain");
//});

// server.Get("/stop",
//           [&](const Request& req, Response& res) { server.stop(); });
// server.listen("localhost", 1234);