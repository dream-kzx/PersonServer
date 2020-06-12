#ifndef PERSONSERVER_CONFIG_H
#define PERSONSERVER_CONFIG_H

#include "SQLKK.h"
#include "KServer.h"


lookupman::SQLKK sqlEngine("database.db");

lookupman::KServer k_server("0.0.0.0", 1234);

void global_init();





#endif
