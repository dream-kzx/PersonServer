#pragma once
#ifndef PERSONSERVER_SQLKK_H
#define PERSONSERVER_SQLKK_H

#include <memory>
#include <string>

#include "sqlite3.h"

namespace lookupman {
//SQLKK sqlEngine("database.db");
//sqlEngine.InitEngine();
//
//auto result = sqlEngine.Select("select * from admin");
//
//while (sqlite3_step(result.get()) == SQLITE_ROW) {
//  int id = sqlite3_column_int(result.get(), 0);
//  const unsigned char* account_number = sqlite3_column_text(result.get(), 1);
//  printf("id: %d account_number: %s\n", id, account_number);
//}


class SQLKK {
 public:
  SQLKK(const std::string& database_name) : database_name_(database_name) {}

  int InitEngine();

  std::shared_ptr<sqlite3_stmt> Select(const std::string& sql_sentence);


  int Find(const std::string& sql_sentence);

  bool Update(const std::string& sql_sentence);

  bool Remove(const std::string& sql_sentence);

  bool Insert(const std::string& sql_sentence);

  ~SQLKK(){};

 private:
  std::shared_ptr<sqlite3> engine_;
  std::string database_name_;

  bool ExecuteSql(const std::string& sql_sentence);
};

const std::string getCurrentTime();

}  // namespace lookupman
#endif
