#ifndef PERSONSERVER_CONTROLLER_H
#define PERSONSERVER_CONTROLLER_H

#include <cstdio>
#include "SQLKK.h"
#include "httplib.h"
#include "sqlite3.h"

extern lookupman::SQLKK sqlEngine;
void StaffSignIn(const httplib::Request& request, httplib::Response& response);

void StaffLogin(const httplib::Request& request, httplib::Response& response);

#endif
