#include "MxPEG_SDK_API.hpp"

#include "SinkVideo.h"
#include "SinkAudio.h"
#include "AuthorizationHandler.h"
#include "NotificationListener.h"
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define URL_SZ 256

using namespace ie::MxPEG;

char *ip, *user, *pass, *output_file;

int arg_parser(int argc, char **argv)
{
   int c;

   while (1)
   {
      static struct option long_options[] =
          {
              {"ip", required_argument, 0, 'i'},
              {"user", required_argument, 0, 'u'},
              {"pass", required_argument, 0, 'p'},
              {"output_file", required_argument, 0, 'o'},
              {0, 0, 0, 0}};
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long(argc, argv, "i:u:p:o:",
                      long_options, &option_index);
      /* Detect the end of the options. */
      if (c == -1)
         break;

      switch (c)
      {
      case 'i':
         ip = optarg;
         break;

      case 'u':
         user = optarg;
         break;

      case 'p':
         pass = optarg;
         break;

      case 'o':
         output_file = optarg;
         break;

      case '?':
         /* getopt_long already printed an error message. */
         break;

      default:
         abort();
      }
   }

   // printf("ip: %s\n", ip);
   // printf("user: %s\n", user);
   // printf("pass: %s\n", pass);
   // printf("output: %s\n", output_file);

   /* Print any remaining command line arguments (not options). */
   if (optind < argc)
   {
      printf("non-option ARGV-elements: ");
      while (optind < argc)
         printf("%s ", argv[optind++]);
      putchar('\n');
   }

   return 0;
}

int main(int argc, char **argv)
{
   arg_parser(argc, argv);
   char url[URL_SZ] = "";
   snprintf(url, URL_SZ, "http://%s/control/eventstream.jpg", ip);
   MxPEG_SinkVideo::shared_ptr_t sinkVideo = MxPEG_SinkVideo::shared_ptr_t(new SinkVideo(output_file));
   MxPEG_SinkAudio::shared_ptr_t sinkAudio = MxPEG_SinkAudio::shared_ptr_t(new SinkAudio());
   MxPEG_AuthorizationHandler::shared_ptr_t authHandler = MxPEG_AuthorizationHandler::shared_ptr_t(new AuthorizationHandler(user, pass));
   MxPEG_EventNotificationListener::shared_ptr_t notificationListener = MxPEG_EventNotificationListener::shared_ptr_t(new NotificationListener());
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
