#include "AuthorizationHandler.h"
#include <iostream>
#include <string.h>

AuthorizationHandler::AuthorizationHandler () {
   std::cout << "Creating AuthorizationHandler" << std::endl;
}
AuthorizationHandler::AuthorizationHandler (char * username, char * password) {
   _username = username;
   _password = password;
   std::cout << "Creating AuthorizationHandler with username and passsword" << std::endl;
}
AuthorizationHandler::~AuthorizationHandler () {
   std::cout << "Deleting AuthorizationHandler" << std::endl;
}

bool AuthorizationHandler::getUserName(char * buf, size_t bufSz) {
	snprintf(buf, bufSz, _username);
	return true;
}

bool AuthorizationHandler::getPassword(char * buf, size_t bufSz) {
	snprintf(buf, bufSz, _password);
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
