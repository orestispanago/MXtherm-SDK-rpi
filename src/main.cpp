#include "MxPEG_SDK_API.hpp"
#include "SinkVideo.h"
#include "SinkAudio.h"
#include "AuthorizationHandler.h"
#include "NotificationListener.h"
#include "params.h"

#define URL_SZ 256

using namespace ie::MxPEG;

int main(int argc, char **argv)
{
   parse_args(argc, argv);
   char url[URL_SZ] = "";
   snprintf(url, URL_SZ, "http://%s/control/eventstream.jpg", ip);
   MxPEG_SinkVideo::shared_ptr_t sinkVideo(new SinkVideo(output));
   MxPEG_SinkAudio::shared_ptr_t sinkAudio(new SinkAudio());
   MxPEG_AuthorizationHandler::shared_ptr_t authHandler(new AuthorizationHandler(user, pass));
   MxPEG_EventNotificationListener::shared_ptr_t notificationListener(new NotificationListener());
   MxPEG_SDK::shared_ptr_t client = MxPEG_SDK_Factory::create(sinkAudio, sinkVideo, MxPEG_ImageMode::im_RGB, authHandler, notificationListener);
   client->stream_setup(url);
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
   client->startLive();
   while (true)
   {
      client->loop();
   }
}
