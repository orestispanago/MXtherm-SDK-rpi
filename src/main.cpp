#include "MxPEG_SDK_API.hpp"

#include "SinkVideo.h"
#include "SinkAudio.h"
#include "AuthorizationHandler.h"
#include "NotificationListener.h"

#include <iostream>
#include <memory>

#define URL_SZ 256

using namespace ie::MxPEG;

int main(int argc, char **argv)
{
   std::cout << "EventStream client SDK thermal raw data example" << std::endl;

   if (argc < 2)
   {
      std::cout << "Missing parameter hostname/IP" << std::endl;
      std::cout << "Start with " << argv[0] << " [hostname|IP]" << std::endl;
      std::cout << "Press enter to exit" << std::endl;
      getchar();
      exit(1);
   }

   char url[URL_SZ] = "";
   snprintf(url, URL_SZ, "http://%s/control/eventstream.jpg", argv[1]);

#ifdef _MSC_VER
   WSADATA wsaData;
   int iResult;
   iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
   if (iResult != NO_ERROR)
   {
      std::cout << "WSAStartup failed: " << iResult << std::endl;
      return 1;
   }
#endif
   MxPEG_SinkVideo::shared_ptr_t sinkVideo = MxPEG_SinkVideo::shared_ptr_t(new SinkVideo("stream", argv[4], argv[5]));
   MxPEG_SinkAudio::shared_ptr_t sinkAudio = MxPEG_SinkAudio::shared_ptr_t(new SinkAudio());
   MxPEG_AuthorizationHandler::shared_ptr_t authHandler = MxPEG_AuthorizationHandler::shared_ptr_t(new AuthorizationHandler(argv[2], argv[3]));
   MxPEG_EventNotificationListener::shared_ptr_t notificationListener = MxPEG_EventNotificationListener::shared_ptr_t(new NotificationListener());
   MxPEG_SDK::shared_ptr_t client = MxPEG_SDK_Factory::create(sinkAudio, sinkVideo, MxPEG_ImageMode::im_RGB, authHandler, notificationListener);
   client->stream_setup(url);
   /*
    * Do not activate audio for this session (the audio sink in this example will drop all audio data anyway)
    */
   client->setAudioActive(false);
   client->setVideoActive(true);

   /*
    * activate raw thermal data export for this session
    *
    * -> To use this feature the camera must be equipped with at least one thermal sensor that is configured
    * for raw export. (Open the camera setup menu and navigate to: Setup->Thermal Sensor Settings, set the option
    * "Thermal Raw Data" to "enabled".
    *
    * See SinkVideo.[cpp|h] for examples how to access and process the thermal raw data
    */
   client->setThermalRawExportActive(true);

   /*
    * tell the camera to start sending live pictures (thermal raw data is sent as part of the video stream)
    */
   client->startLive();

   /*
    * The SDK library is single threaded, the function loop() acts as the main loop of the library. Call it repeatedly
    * from the applications main loop.
    */
   while (true)
   {
      client->loop();
   }
#ifdef _MSC_VER
   WSACleanup();
#endif
}
