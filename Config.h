#ifndef PERSONSERVER_CONFIG_H
#define PERSONSERVER_CONFIG_H

#include <chrono>

#include "KServer.h"
#include "SQLKK.h"

lookupman::SQLKK sqlEngine("database.db");

lookupman::KServer k_server("0.0.0.0", 1234, std::chrono::minutes(30));

void global_init();

#endif
