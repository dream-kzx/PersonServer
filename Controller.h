#ifndef PERSONSERVER_CONTROLLER_H
#define PERSONSERVER_CONTROLLER_H

#include <cstdio>

#include "SQLKK.h"
#include "httplib.h"
#include "KServer.h"

extern lookupman::KServer k_server;

extern lookupman::SQLKK sqlEngine;

void StaffSignIn(const httplib::Request& request, httplib::Response& response);

void StaffLogin(const httplib::Request& request, httplib::Response& response);


#endif
