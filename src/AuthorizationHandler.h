#ifndef SAMPLE_AUTHORIZATIONHANDLER_H_
#define SAMPLE_AUTHORIZATIONHANDLER_H_

#if defined(_MSC_VER)
#include <Windows.h>
#endif
#include <MxPEG_AuthorizationHandler.hpp>

using namespace ie::MxPEG;

class AuthorizationHandler : public MxPEG_AuthorizationHandler
{
public:
   AuthorizationHandler();
   AuthorizationHandler(char *username, char *password);
   virtual ~AuthorizationHandler();

   virtual bool getUserName(char *buf, size_t bufSz);

   virtual bool getPassword(char *buf, size_t bufSz);

   virtual bool haveCredentials();

   virtual void authenticationFailed();

   virtual void connectionAborted();

   virtual unsigned int maxRetries();

   virtual bool reconnectOnAuthFail();

private:
   char *_username;
   char *_password;
};

#endif /* SAMPLE_AUTHORIZATIONHANDLER_H_ */
