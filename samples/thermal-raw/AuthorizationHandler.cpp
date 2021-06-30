#include "AuthorizationHandler.h"
#include <iostream>
#include <string.h>

AuthorizationHandler::AuthorizationHandler () {
   std::cout << "Creating AuthorizationHandler" << std::endl;
}

AuthorizationHandler::~AuthorizationHandler () {
   std::cout << "Deleting AuthorizationHandler" << std::endl;
}

bool AuthorizationHandler::getUserName(char * buf, size_t bufSz) {
	snprintf(buf, bufSz, "admin");
	return true;
}

bool AuthorizationHandler::getPassword(char * buf, size_t bufSz) {
	snprintf(buf, bufSz, "meisnm");
	return true;
}

bool AuthorizationHandler::haveCredentials() {
   return true;
}

void AuthorizationHandler::authenticationFailed() {
   std::cout << "Authentication failed" << std::endl;
}

void AuthorizationHandler::connectionAborted() {
   std::cout << "Connection aborted" << std::endl;
}

unsigned int AuthorizationHandler::maxRetries() {
   return 3;
}

bool AuthorizationHandler::reconnectOnAuthFail() {
   return false;
}
