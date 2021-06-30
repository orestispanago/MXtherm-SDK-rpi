#include "SinkVideo.h"
#include "MxPEG_ThermalRawData.hpp"
#include "MxPEG_Image.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#if defined(_MSC_VER)
#include <fcntl.h>
#include <io.h>
#endif

SinkVideo::SinkVideo(std::string outName)
    : m_name(outName), m_count(0)
{
    std::cout << "Creating SinkVideo using " << outName << " as basename for frames and thermal raw data" << std::endl;
}

SinkVideo::~SinkVideo()
{
    std::cout << "Deleting SinkVideo" << std::endl;
}

void SinkVideo::writeThermalData(MxPEG_Image& buffer)
{

    std::shared_ptr<MX_ThermalRawData> rawData = buffer.fetchThermalRawData(MXT_Sensor::left);

    // request information about the shutter of the thermal sensor (8 bit FFC counter)
    //uint8_t ffcCount = buffer.thermalFFCCount(MXT_Sensor::right);

    if (rawData.get() != nullptr)
    {
        std::cout << "  Got thermal raw data from left sensor " << std::endl;
        // writeThermalRaw(rawData);
        // writeThermalRawIntCSV(rawData);
        writeThermalCelsiusCSV(rawData);
    }

    rawData = buffer.fetchThermalRawData(MXT_Sensor::right);
    if (rawData.get() != nullptr)
    {
        std::cout << "  Got thermal raw data from right sensor " << std::endl;
        // writeThermalRaw(rawData);
        // writeThermalRawIntCSV(rawData);
        writeThermalCelsiusCSV(rawData);
    }
}

bool SinkVideo::writeRBG(MxPEG_Image& buffer)
{

    char fname[1024];
    struct tm* timenow;

    time_t now = time(NULL);
    timenow = gmtime(&now);
    strftime(fname, sizeof(fname), "output/%Y%m%d_%H%M%S.rgb", timenow);
    FILE* fVideoOut = NULL;
    fVideoOut = fopen(fname, "w");

#if defined(_MSC_VER)
    //for windows set the out stream to binary mode
    int res = _setmode(_fileno(fVideoOut), _O_BINARY);
#endif

    if (fVideoOut != NULL)
    {
        //only write RGB data
        if (buffer.mode() == MxPEG_ImageMode::im_RGB)
        {
            /*
             * channel order is bgra, interleaved 4:4:4, 8 bit per channel
             */
            const uint8_t* rgbBuffer = buffer.imgBuffer();
            const size_t rgbBufferSize = buffer.imgBufferSize();
            fwrite(rgbBuffer, rgbBufferSize, 1, fVideoOut);
            std::cout << "  -> wrote raw rgb image: " << rgbBufferSize << " bytes, res: " << (int)buffer.width() << "x"
                << (int)buffer.height() << std::endl;
        }
        fclose(fVideoOut);
        return true;
    }

    return false;
}

bool SinkVideo::writeThermalRaw(std::shared_ptr<MX_ThermalRawData> rawData)
{

    char fname[1024];
    snprintf(fname, 1024, "output/%s_%06u_%s_%ux%u_%s.thermal.raw",
        m_name.c_str(),
        m_count,
        ((rawData->sensor() == MXT_Sensor::left) ? "left" : "right"),
        rawData->width(),
        rawData->height(),
        ((rawData->bitDepth() == MXT_BitDepth::depth14bit) ? "14bit" : "unknown"));
    FILE* fVideoOut = NULL;
    fVideoOut = fopen(fname, "w");

    if (fVideoOut == nullptr)
    {
        //failed to open output file
        return false;
    }

#if defined(_MSC_VER)
    //for windows set the out stream to binary mode
    int res = _setmode(_fileno(fVideoOut), _O_BINARY);
#endif

    /*
     * channel order is bgra, interleaved 4:4:4, 8 bit per channel
     */
    const uint8_t* rawBuffer = rawData->rawBuffer();
    size_t rawBufferSize = rawData->rawBufferSize();

    fwrite(rawBuffer, rawBufferSize, 1, fVideoOut);
    std::cout << "  -> wrote thermal raw data of sensor " << (int)rawData->sensor() << " " << rawBufferSize << " bytes, res: " << (int)rawData->width() << "x"
        << (int)rawData->height() << std::endl;
    fclose(fVideoOut);
    return true;
}

bool SinkVideo::writeThermalRawIntCSV(std::shared_ptr<MX_ThermalRawData> rawData)
{
    char fname[1024];
    snprintf(fname, 1024, "output/%s_%06u_%s_%ux%u_%s.thermal.uint.csv",
        m_name.c_str(),
        m_count,
        ((rawData->sensor() == MXT_Sensor::left) ? "left" : "right"),
        rawData->width(),
        rawData->height(),
        ((rawData->bitDepth() == MXT_BitDepth::depth14bit) ? "14bit" : "unknown"));
    FILE* fVideoOut = NULL;
    fVideoOut = fopen(fname, "w");

    if (fVideoOut == nullptr)
    {
        //failed to open output file
        return false;
    }

#if defined(_MSC_VER)
    //for windows set the out stream to binary mode
    int res = _setmode(_fileno(fVideoOut), _O_BINARY);
#endif

    std::ostringstream header;
    header << "sensor;" << ((rawData->sensor() == MXT_Sensor::left) ? "left" : "right") << "\n";
    header << "bit depth;" << ((rawData->bitDepth() == MXT_BitDepth::depth14bit) ? "14 bit" : "unknown") << "\n";
    header << "width;" << (int)rawData->width() << "\n";
    header << "height;" << (int)rawData->height() << "\n";
    header << "resolution;" << ((rawData->resolution() == MXT_Resolution::high) ? "high" : "low") << "\n";
    header << "advanced radiometry support;" << ((rawData->advancedRadiometrySupport() == true) ? "yes" : "no") << "\n";
    header << "unit;raw uint16"
        << "\n";
    header << "\n";

    fwrite(header.str().c_str(), header.str().length(), 1, fVideoOut);

    /*
     * raw layout of 14bit thermal raw:
     * format uses 2 bytes per pixel (little endian), with the 2 most significant bytes set to 0:
     * llll llll hhhh hhhh
     * xxxx xxxx 00xx xxxx
     */
    const uint8_t* rawBuffer = rawData->rawBuffer();
    size_t rawBufferSize = rawData->rawBufferSize();
    for (uint32_t y = 0; y < rawData->height(); ++y)
    {
        std::ostringstream line;
        for (uint32_t x = 0; x < rawData->width(); ++x)
        {
            uint32_t offset = y * (rawData->width() * 2) + (x * 2);
            //sanity check - should never happen
            if ((offset + 1) >= rawBufferSize)
                break;
            //make sure to clear 2 msb
            uint8_t highByte = (rawBuffer[offset + 1] & 0x3F);
            uint8_t lowByte = rawBuffer[offset];
            uint32_t value = (highByte << 8) + lowByte;
            line << value;
            if (x + 1 < rawData->width())
                line << ";";
        }
        line << "\n";
        fwrite(line.str().c_str(), line.str().length(), 1, fVideoOut);
    }
    std::cout << "  -> converted thermal raw data of sensor " << (int)rawData->sensor() << " to integer csv file " << std::endl;

    fclose(fVideoOut);
    return true;
}

bool SinkVideo::writeThermalCelsiusCSV(std::shared_ptr<MX_ThermalRawData> rawData)
{

    if (!rawData->advancedRadiometrySupport())
    {
        std::cout << "conversion to absolute temperatures is only supported for sensors with advanced radiometry support" << std::endl;
        return false;
    }
    char fname[1024];
    struct tm* timenow;

    time_t now = time(NULL);
    timenow = gmtime(&now);

    strftime(fname, sizeof(fname), "output/%Y%m%d_%H%M%S.csv", timenow);

    // snprintf(fname, 1024, "output/%s.%03d.thermal.celsius.csv"
    // m_name.c_str(),
    // m_count,
    // ((rawData->sensor() == MXT_Sensor::left) ? "left" : "right"),
    // rawData->width(),
    // rawData->height(),
    // ((rawData->bitDepth() == MXT_BitDepth::depth14bit) ? "14bit" : "unknown")
    // );
    FILE* fVideoOut = NULL;
    fVideoOut = fopen(fname, "w");

    if (fVideoOut == nullptr)
    {
        //failed to open output file
        return false;
    }

#if defined(_MSC_VER)
    //for windows set the out stream to binary mode
    int res = _setmode(_fileno(fVideoOut), _O_BINARY);
#endif

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

    //write the RGB image data
     writeRBG(*buffer);

    //write the thermal data
    writeThermalData(*buffer);
    std::cout << "Exiting in a hacky way...\n";
    exit(0);
    return er_Success;
}