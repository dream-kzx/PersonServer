#include "Controller.h"



void StaffLogin(const httplib::Request& request, httplib::Response& response) {
  if (!request.has_param("username")) {
    response.set_content(request.body, "text/plain");
    return;
  }

  auto username = request.get_param_value("username");
  auto password = request.get_param_value("password");
  std::string sql_string = "select * from staff where account_number = '" +
                           username + "' and password = '" + password + "'";

  auto result = sqlEngine.Select(sql_string);

  while (sqlite3_step(result.get()) == SQLITE_ROW) {
    int id = sqlite3_column_int(result.get(), 0);

    printf("%d\n", id);
    response.set_content(std::to_string(id), "text/plain");
  }

  response.set_content(request.body, "text/plain");
  // while (sqlite3_step(result.get()) == SQLITE_ROW) {
  //  int id = sqlite3_column_int(result.get(), 0);
  //  const unsigned char* account_number = sqlite3_column_text(result.get(),
  //  1); printf("id: %d account_number: %s\n", id, account_number);
  //}
}