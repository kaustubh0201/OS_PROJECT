#ifndef OSD__HARDWARECOMMUNICATION__INTERRUPTS_H
#define OSD__HARDWARECOMMUNICATION__INTERRUPTS_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <gdt.h>
#include <multitasking.h>

namespace osd
{
    namespace hardwarecommunication
    {

        class InterruptManager;

        class InterruptHandler
        {
        protected:
            osd::common::uint8_t interruptNumber;
            InterruptManager* interruptManager;

            InterruptHandler(osd::common::uint8_t interruptNumber, InterruptManager* interruptManager);
            ~InterruptHandler();
        public:
            virtual osd::common::uint32_t HandleInterrupt(osd::common::uint32_t esp);
        };

        class InterruptManager
        {

        friend class InterruptHandler;

        protected:

            static InterruptManager* ActiveInterruptManager;
            InterruptHandler* handlers[256];
            TaskManager *taskManager;

            struct GateDescriptor
            {
                osd::common::uint16_t handleAddressLowBits;
                osd::common::uint16_t gdt_codeSegmentSelector;
                osd::common::uint8_t reserved;
                osd::common::uint8_t access;
                osd::common::uint16_t handleAddressHighBits;
                
            }__attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer
            {
                osd::common::uint16_t size;
                osd::common::uint32_t base;
            }__attribute__((packed));
            
            
            static void SetInterruptDescriptorTableEntry(
                osd::common::uint8_t interruptNumber,
                osd::common::uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                osd::common::uint8_t DescriptorPrivilegeLevel,
                osd::common::uint8_t DescriptorType
            );

            osd::hardwarecommunication::Port8bitSlow picMasterCommand;
            osd::hardwarecommunication::Port8bitSlow picMasterData;
            osd::hardwarecommunication::Port8bitSlow picSlaveCommand;
            osd::hardwarecommunication::Port8bitSlow picSlaveData;

        public:

            InterruptManager(osd::common::uint16_t hardwareInterruptOffset, osd::GlobalDescriptionTable* gdt, osd::TaskManager *taskManager);
            ~InterruptManager();

            void Activate();
            void Deactivate();

            static osd::common::uint32_t handleInterrupt(osd::common::uint8_t interruptNumber, osd::common::uint32_t esp);
            osd::common::uint32_t DoHandleInterrupt(osd::common::uint8_t interruptNumber, osd::common::uint32_t esp);

            static void IgnoreInterruptRequest();
            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x0C();
        };
    }
}

#endif