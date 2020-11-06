#ifndef __OSD__DRIVERS__VGA_H
#define __OSD__DRIVERS__VGA_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace osd
{
    namespace drivers
    {
        class VideoGraphicsArray
        {
            // below are all the ports that are needed for putting video grahics to our operating system
        protected:
            osd::hardwarecommunication::Port8bit miscPort; // miscellaneous port
            osd::hardwarecommunication::Port8bit crtcIndexPort; // this is the cathode ray tube index port
            osd::hardwarecommunication::Port8bit crtcDataPort; // cathode ray tube data port
            osd::hardwarecommunication::Port8bit sequenceIndexPort; // this is the sequencing port
            osd::hardwarecommunication::Port8bit sequenceDataPort; // this is the sequencing data port
            osd::hardwarecommunication::Port8bit graphicsControllerIndexPort; // graphics controller index port
            osd::hardwarecommunication::Port8bit graphiceControllerDataPort; // graphics controller data port
            osd::hardwarecommunication::Port8bit attributeControllerIndexPort; // attribute controller reader port
            osd::hardwarecommunication::Port8bit attributeControllerReadPort; // attribute controller read port
            osd::hardwarecommunication::Port8bit attributeControllerWritePort; // attribute controller writer port
            osd::hardwarecommunication::Port8bit attributeControllerResetPort; // attribute controller reset port

            void WriteRegisters(osd::common::uint8_t* registers);
            osd::common::uint8_t* GetFrameBufferSegment();

            virtual osd::common::uint8_t GetColorIndex(osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b);

        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            virtual bool SetMode(osd::common::uint32_t width, osd::common::uint32_t height, osd::common::uint32_t colordepth);
            virtual bool SupportsMode(osd::common::uint32_t width, osd::common::uint32_t height, osd::common::uint32_t colordepth);
            virtual void PutPixel(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b);    
            virtual void PutPixel(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t colorIndex);

            virtual void FillRectangle(osd::common::uint32_t x, osd::common::uint32_t w, osd::common::uint32_t h, osd::common::uint32_t y, osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b);

        };
    }
}

#endif