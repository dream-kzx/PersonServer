// ManagerServer.cpp: 定义应用程序的入口点。
//
#include <httplib.h>
#include <sqlite3.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>


#include "KServer.h"
#include "SQLKK.h"
#include "Controller.h"
//#include "Config.h"

extern lookupman::KServer k_server;
extern void global_init();

int main() {
  using namespace lookupman;
  using namespace httplib;

  global_init();

  k_server.Get("/login", StaffLogin);

  //k_server.Get("/", [](const Request& req, Response& res) {
  //  res.set_content("Hello World!", "text/plain");
  //});


  k_server.Listen();
}
