// ManagerServer.cpp: 定义应用程序的入口点。
//
//
#pragma execution_character_set("utf-8")
#include <httplib.h>
#include <sqlite3.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "KServer.h"
#include "SQLKK.h"
#include "Controller.h"
#include "MyCrypto.h"


extern lookupman::KServer k_server;
extern void global_init();

int main() {
  using namespace lookupman;
  using namespace httplib;

  //std::cout << MyCrypto::Hash("I'm kzx khhhhhh") << std::endl;


  global_init();

  k_server.Post("/SignIn", StaffSignIn);

  k_server.Post("/login", StaffLogin);

  k_server.Get("/", [](const Request& req, Response& res) {
    res.set_content("Hello World!", "text/plain");
  });


  k_server.Listen();
}
