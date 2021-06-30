#include "MxPEG_SDK_API.hpp"

#include "SinkVideo.h"
#include "SinkAudio.h"
#include "AuthorizationHandler.h"
#include "NotificationListener.h"

#include <iostream>
#include <memory>

#define URL_SZ 256

using namespace ie::MxPEG;
#define CAM_IP "150.140.194.41"

int main(int argc, char** argv)
{
    std::cout << "EventStream client SDK thermal raw data example" << std::endl;

    // if (argc < 2)
    // {
    //    std::cout << "Missing parameter hostname/IP" << std::endl;
    //    std::cout << "Start with " << argv[0] << " [hostname|IP]" << std::endl;
    //    std::cout << "Press enter to exit" << std::endl;
    //    getchar();
    //    exit(1);
    // }

    char url[URL_SZ] = "";
    snprintf(url, URL_SZ, "http://%s/control/eventstream.jpg", CAM_IP);

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

    /*
     * Create a video sink object to process the decoded video frames
     */
    MxPEG_SinkVideo::shared_ptr_t sinkVideo = MxPEG_SinkVideo::shared_ptr_t(new SinkVideo("stream"));

    /*
     * Create an audio sink object to process the received audio packages
     */
    MxPEG_SinkAudio::shared_ptr_t sinkAudio = MxPEG_SinkAudio::shared_ptr_t(new SinkAudio());

    /*
     * Create an authorization handler that provides the credentials (if needed)
     */
    MxPEG_AuthorizationHandler::shared_ptr_t authHandler = MxPEG_AuthorizationHandler::shared_ptr_t(new AuthorizationHandler());

    /*
     * Create a notification listener to handle the json replies of the camera
     */
    MxPEG_EventNotificationListener::shared_ptr_t notificationListener = MxPEG_EventNotificationListener::shared_ptr_t(new NotificationListener());

    /*
     * Create a new eventstream client SDK object
     */
    MxPEG_SDK::shared_ptr_t client = MxPEG_SDK_Factory::create(sinkAudio, sinkVideo, MxPEG_ImageMode::im_RGB, authHandler, notificationListener);

    /*
     * configure the client to connect to camera with hostname "i25"
     */
    client->stream_setup(url);

    /*
     * Do not activate audio for this session (the audio sink in this example will drop all audio data anyway)
     */
    client->setAudioActive(false);

    /*
     * activate video for this session
     */
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