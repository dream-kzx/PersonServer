#include "SQLKK.h"

#include <cstdio>

namespace lookupman {

//返回结果为0表示正常，为1表示初始化异常
int SQLKK::InitEngine() {
  sqlite3* engine = nullptr;

  int status =
      sqlite3_open_v2(database_name_.c_str(), &engine,
                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_SHAREDCACHE, nullptr);

  if (status != SQLITE_OK) {
    printf("打开数据库失败！\n");
    return 1;
  }

  sqlite3_stmt* stmt = nullptr;

  engine_.reset(engine,
                [engine](sqlite3* engine) { sqlite3_close_v2(engine); });
}

std::shared_ptr<sqlite3_stmt> SQLKK::Select(const std::string& sql_sentence) {
  sqlite3_stmt* stmt = nullptr;
  int status = sqlite3_prepare_v2(engine_.get(), sql_sentence.c_str(), -1,
                                  &stmt, nullptr);

  if (status != SQLITE_OK) {
    printf("sql语句有误！\n");
    return nullptr;
  }

  return std::shared_ptr<sqlite3_stmt>(
      stmt, [stmt](sqlite3_stmt* stmt) { sqlite3_finalize(stmt); });
}

bool SQLKK::Update(const std::string& sql_sentence) {
  return ExecuteSql(sql_sentence);
}

bool SQLKK::Remove(const std::string& sql_sentence) {
  return ExecuteSql(sql_sentence);
}

bool SQLKK::Insert(const std::string& sql_sentence) {
  return ExecuteSql(sql_sentence);
}

bool SQLKK::ExecuteSql(const std::string& sql_sentence) {
  sqlite3_stmt* stmt = nullptr;

  int status = sqlite3_prepare_v2(engine_.get(), sql_sentence.c_str(), -1,
                                  &stmt, nullptr);

  bool result;
  if (status == SQLITE_OK && sqlite3_step(stmt) == SQLITE_DONE) {
    result = true;
  } else {
    result = false;
  }

  sqlite3_finalize(stmt);

  return result;
}

}  // namespace lookupman
