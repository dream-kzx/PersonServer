#pragma once
#ifndef PERSONSERVER_SQLKK_H
#define PERSONSERVER_SQLKK_H

#include <memory>
#include <string>

#include "sqlite3.h"

namespace lookupman {
class SQLKK {
 public:
  SQLKK(const std::string& database_name) : database_name_(database_name) {}

  int InitEngine();

  std::shared_ptr<sqlite3_stmt> Select(const std::string& sql_sentence);

  bool Update(const std::string& sql_sentence);

  bool Remove(const std::string& sql_sentence);

  bool Insert(const std::string& sql_sentence);

  ~SQLKK(){};

 private:
  std::shared_ptr<sqlite3> engine_;
  std::string database_name_;

  bool ExecuteSql(const std::string& sql_sentence);
};

}  // namespace lookupman
#endif
