#ifndef PERSONSERVER_CONTROLLER_H
#define PERSONSERVER_CONTROLLER_H

#include <cstdio>

#include "KServer.h"
#include "SQLKK.h"
#include "httplib.h"

extern lookupman::KServer k_server;

extern lookupman::SQLKK sqlEngine;

void ErrorRequest(httplib::Response& response);

void LoginExpire(httplib::Response& response);

void StaffSignIn(const httplib::Request& request, httplib::Response& response);

void StaffLogin(const httplib::Request& request, httplib::Response& response);

void StaffGetMessage(const httplib::Request& request,
                     httplib::Response& response);

void StaffUpdateMessage(const httplib::Request& request,
                        httplib::Response& response);

#endif
