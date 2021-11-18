#include "SinkVideo.h"
#include "MxPEG_ThermalRawData.hpp"
#include "MxPEG_Image.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>

SinkVideo::SinkVideo(char *output_file)
    : output_file_path(output_file)
{
   std::cout << "Creating SinkVideo" << std::endl;
}

SinkVideo::~SinkVideo()
{
   std::cout << "Deleting SinkVideo" << std::endl;
}

void SinkVideo::writeThermalData(MxPEG_Image &buffer)
{
   std::shared_ptr<MX_ThermalRawData> rawData = buffer.fetchThermalRawData(MXT_Sensor::left);
   // request information about the shutter of the thermal sensor (8 bit FFC counter)
   // uint8_t ffcCount = buffer.thermalFFCCount(MXT_Sensor::right);
   if (rawData.get() != nullptr)
   {
      std::cout << "  Got thermal raw data from left sensor " << std::endl;
      writeThermalCelsiusCSV(rawData);
   }
   rawData = buffer.fetchThermalRawData(MXT_Sensor::right);
   if (rawData.get() != nullptr)
   {
      std::cout << "  Got thermal raw data from right sensor " << std::endl;
      writeThermalCelsiusCSV(rawData);
   }
}

bool SinkVideo::writeThermalCelsiusCSV(std::shared_ptr<MX_ThermalRawData> rawData)
{

   if (!rawData->advancedRadiometrySupport())
   {
      std::cout << "conversion to absolute temperatures is only supported for sensors with advanced radiometry support" << std::endl;
      return false;
   }
   FILE *fVideoOut = NULL;
   fVideoOut = fopen(output_file_path, "w");

   if (fVideoOut == nullptr)
   {
      std::cout << "failed to open output file" << std::endl;
      return false;
   }

   std::ostringstream header;
   header << "sensor;" << ((rawData->sensor() == MXT_Sensor::left) ? "left" : "right") << "\n";
   header << "bit depth;" << ((rawData->bitDepth() == MXT_BitDepth::depth14bit) ? "14 bit" : "unknown") << "\n";
   header << "width;" << (int)rawData->width() << "\n";
   header << "height;" << (int)rawData->height() << "\n";
   header << "resolution;" << ((rawData->resolution() == MXT_Resolution::high) ? "high" : "low") << "\n";
   header << "advanced radiometry support;" << ((rawData->advancedRadiometrySupport() == true) ? "yes" : "no") << "\n";
   header << "unit;degrees Celsius"
          << "\n";
   header << "\n";

   fwrite(header.str().c_str(), header.str().length(), 1, fVideoOut);

   for (uint32_t y = 0; y < rawData->height(); ++y)
   {
      std::ostringstream line;
      for (uint32_t x = 0; x < rawData->width(); ++x)
      {
         line << rawData->convertTo(x, y, MXT_Unit::celsius);
         if (x + 1 < rawData->width())
            line << ";";
      }
      line << "\n";
      fwrite(line.str().c_str(), line.str().length(), 1, fVideoOut);
   }
   std::cout << "  -> converted thermal raw data of sensor " << (int)rawData->sensor() << " to Celsius csv file " << std::endl;

   fclose(fVideoOut);
   return true;
}

MxPEG_ReturnCode SinkVideo::doConsumeVideo(MxPEG_Image::unique_ptr_t buffer)
{
   m_count++;
   /*
    * get the timestamp for each frame - in this example only used for debug output
    */
   uint64_t syncTime = buffer->creationTime().tv_sec;
   syncTime *= 1000000;
   syncTime += buffer->creationTime().tv_usec;

   std::cout << "received video frame #" << m_count
             << " type: " << ((buffer->mode() == MxPEG_ImageMode::im_YUV) ? "YUV" : "BGRA")
             << " ts: " << syncTime << std::endl;
   // writeRBG(*buffer);
   writeThermalData(*buffer);
   std::cout << "Exiting in a hacky way...\n";
   exit(0);
}
