#ifndef OSD__HARDWARECOMMUNICATION__PCI_H
#define OSD__HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/port.h>
#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>

#include <memorymanagement.h>

namespace osd
{
    namespace hardwarecommunication
    {

        enum BaseAddressRegisterType
        {
            // this is to tell if below two are present for device
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister
        {
            public:
                bool prefetchable;
                osd::common::uint8_t* address;
                osd::common::uint32_t size;
                BaseAddressRegisterType type;      
        };

        class PeripheralComponentInterconnectDeviceDescriptor
        {
            // below are the different information that helps us to differentiate between the devices
            public:
                osd::common::uint32_t portBase;
                osd::common::uint32_t interrupt;

                osd::common::uint16_t bus; // multible buses
                osd::common::uint16_t device; // multiple devices
                osd::common::uint16_t function; // multiple functions

                // these ids are used to identify the devices
                osd::common::uint16_t vendor_id; 
                osd::common::uint16_t device_id;
                
                osd::common::uint8_t class_id;
                osd::common::uint8_t subclass_id;
                osd::common::uint8_t interface_id;

                osd::common::uint8_t revision;

                PeripheralComponentInterconnectDeviceDescriptor(/* args */);
                ~PeripheralComponentInterconnectDeviceDescriptor();
        };

        class PeripheralComponentInterconnectController
        {
        private:
            // these are the common ports that are necessary 
            osd::hardwarecommunication::Port32bit dataPort;
            osd::hardwarecommunication::Port32bit commandPort;
        public:
            PeripheralComponentInterconnectController(/* args */);
            ~PeripheralComponentInterconnectController();
            
            // we also pass the offset that we want to read
            osd::common::uint32_t Read(osd::common::uint16_t bus, osd::common::uint16_t device, osd::common::uint16_t function, osd::common::uint32_t registeroffset);
            // extra here is the value that we are going to write
            void Write(osd::common::uint16_t bus, osd::common::uint16_t device, osd::common::uint16_t function, osd::common::uint32_t registeroffset, osd::common::int32_t value);
            
            // this boolean is to check if the device has some functions or not
            bool DeviceHasFunctions(osd::common::uint16_t bus, osd::common::uint16_t device);
            
            // this functions selects the drivers on the basis of class, sub_class, window, device_id
            void SelectDrivers(osd::drivers::DriverManager* driverManager, osd::hardwarecommunication::InterruptManager* interrupts);

            // this function is for getting the details of the device such as the functions it is using and all that
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(osd::common::uint16_t bus, osd::common::uint16_t device, osd::common::uint16_t function);
            
            // this gives us the address register of the devices
            BaseAddressRegister GetBaseAddressRegister(osd::common::uint16_t bus, osd::common::uint16_t device, osd::common::uint16_t function, osd::common::uint16_t bar);

            // this is to get the driver for that paricular device
            osd::drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, osd::hardwarecommunication::InterruptManager* interrupts);
        };
    }
}



#endif