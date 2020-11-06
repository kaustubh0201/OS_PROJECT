#ifndef __OSD__GDT_H
#define __OSD__GDT_H

#include <common/types.h>

namespace osd
{

    class GlobalDescriptionTable
    {
        public:
            class SegmentDescriptor
            {
                private:
                    osd::common::uint16_t limit_lo;
                    osd::common::uint16_t base_lo;
                    osd::common::uint8_t base_hi;
                    osd::common::uint8_t type;
                    osd::common::uint8_t flags_limit_hi;
                    osd::common::uint8_t base_vhi;
                public:
                    SegmentDescriptor(osd::common::uint32_t base, osd::common::uint32_t limit, osd::common::uint8_t type);
                    osd::common::uint32_t Base();
                    osd::common::uint32_t Limit();

            } __attribute__((packed));

        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

        public :
            GlobalDescriptionTable();
            ~GlobalDescriptionTable();

            osd::common::uint16_t CodeSegmentSelector();
            osd::common::uint16_t DataSegmentSelector();

    };
}


#endif