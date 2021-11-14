#ifndef SAMPLE_SINKVIDEO_H_
#define SAMPLE_SINKVIDEO_H_

#ifdef WINDOWS
#include <Windows.h>
#endif

#include "MxPEG_SinkVideo.hpp"
#include "MxPEG_Image.hpp"
#include "MxPEG_Defines.hpp"

#include <string>

using namespace ie::MxPEG;
/*
 * Sample video sink, writes the decoded frames and thermal raw data using to the specified base name.
 */
class SinkVideo : public MxPEG_SinkVideo
{
public:
   /*
    * Constructor of the sample sink, takes the base name for the files generated as parameter.
    */
   SinkVideo(std::string outName, std::string x, std::string y);

   virtual ~SinkVideo();

protected:
   /*
    * Receives each video frame right after it was decoded. Either YUV or RGB is possible (see MxPEG_Image.hpp and MxPEG_SDK_API.hpp)
    */
   virtual MxPEG_ReturnCode doConsumeVideo(MxPEG_Image::unique_ptr_t buffer);

private:
   /*
    * Writes the image data of decoded to RGB (see main.cpp) to an output file. A new file is created for each frame.
    */
   bool writeRBG(MxPEG_Image &buffer);

   /*
    * Writes the thermal raw data that is included with each frame to an output file. Separate files are generated for each frame.
    * If multiple thermal sensors are connected to the camera, the output is written to separate files for each sensor.
    */
   void writeThermalData(MxPEG_Image &buffer);

   /*
    * Writes the thermal raw data as a binary blob to an output file.
    */
   bool writeThermalRaw(std::shared_ptr<MX_ThermalRawData> rawData);
   /*
    * Converts the thermal raw data to unsigned integer values for each pixel. Writes integer values to a csv file.
    */
   bool writeThermalRawIntCSV(std::shared_ptr<MX_ThermalRawData> rawData);
   /*
    * If supported (TR sensor in "Linear Mode"):
    * Converts the thermal raw data to degrees Celsius float values for each pixel. Writes integer values to a csv file.
    */
   bool writeThermalCelsiusCSV(std::shared_ptr<MX_ThermalRawData> rawData);

   std::string m_name;
   std::string pos_x;
   std::string pos_y;
   uint32_t m_count;
};

#endif /* SAMPLE_SINKVIDEO_H_ */
