//Config.h
//此文件是整个系统的配置文件，包括配置数据库文件的位置，监听的端口，token超时的时间等
#ifndef PERSONSERVER_CONFIG_H
#define PERSONSERVER_CONFIG_H

#include <chrono>

#include "KServer.h"
#include "SQLKK.h"

const std::string database_url = "database.db";
const std::string address = "0.0.0.0";
int port = 1234;
auto token_timeout = std::chrono::minutes(30);

lookupman::SQLKK sqlEngine(database_url);

lookupman::KServer k_server(address, port, token_timeout);

void global_init();

#endif
