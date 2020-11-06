#ifndef OSD__HARDWARECOMMUNICATION__PORT_H
#define OSD__HARDWARECOMMUNICATION__PORT_H

#include <common/types.h>

namespace osd
{
    namespace hardwarecommunication
    {

        class Port
        {
        protected:
            osd::common::uint16_t portnumber;
            Port(osd::common::uint16_t portnumber);
            ~Port();
        };

        class Port8bit : public Port
        {
        public:
            Port8bit(osd::common::uint16_t portnumber);
            ~Port8bit();
            virtual void Write(osd::common::uint8_t data);
            virtual osd::common::uint8_t Read();
        };


        class Port8bitSlow : public Port8bit
        {
        public:
            Port8bitSlow(osd::common::uint16_t portnumber);
            ~Port8bitSlow();
            virtual void Write(osd::common::uint8_t data);
        };


        class Port16bit : public Port
        {
        public:
            Port16bit(osd::common::uint16_t portnumber);
            ~Port16bit();
            virtual void Write(osd::common::uint16_t data);
            virtual osd::common::uint16_t Read();
        };

        class Port32bit : public Port
        {
        public:
            Port32bit(osd::common::uint32_t portnumber);
            ~Port32bit();
            virtual void Write(osd::common::uint32_t data);
            virtual osd::common::uint32_t Read();
        };
    }
}

#endif