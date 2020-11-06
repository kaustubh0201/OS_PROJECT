#include <hardwarecommunication/pci.h>

using namespace osd::common;
using namespace osd::hardwarecommunication;

PeripheralComponentInterconnectController :: PeripheralComponentInterconnectController()
: dataPort(0xCFC), // dataports and commandports are assigned
  commandPort(0xCF8)
{
    // these are the specific hex for the port numbers of pci


}

PeripheralComponentInterconnectController :: ~PeripheralComponentInterconnectController()
{}

uint32_t PeripheralComponentInterconnectController :: Read(osd::common::uint16_t bus, osd::common::uint16_t device, osd::common::uint16_t function, osd::common::uint32_t registeroffset)
{
    // here we are writing the different bits of this 32 bit number
    uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | ((registeroffset & 0xFC));
    commandPort.Write(id);
    uint32_t result = dataPort.Read();
    return result >> (8* (registeroffset % 4));
}
            
void PeripheralComponentInterconnectController :: Write(osd::common::uint16_t bus, osd::common::uint16_t device, osd::common::uint16_t function, osd::common::uint32_t registeroffset, osd::common::int32_t value){
    uint32_t id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | ((registeroffset & 0xFC));
    commandPort.Write(id);
    dataPort.Write(value); 

    // we have to ask the device if it has functions or not because if there is no function and we still find then it will consume time.
}

bool PeripheralComponentInterconnectController :: DeviceHasFunctions(osd::common::uint16_t bus, osd::common::uint16_t device){
    // the seventh bit tells us if the device has fucntions or not
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}

// we want to give the driver manager to the PCI controller and have the PCI talk to the driver
// manager and insert the drivers for the device that are connected to the PCI

void printf(char* str);
void printfHex(uint8_t);

void PeripheralComponentInterconnectController :: SelectDrivers(osd::drivers::DriverManager* driverManager, osd::hardwarecommunication::InterruptManager* interrupts)
{
    for(int bus = 0; bus < 8; bus++)
    {
        for(int device = 0; device < 32; device++)
        {
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for(int function = 0; function < numFunctions; function++)
            {
                PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);

                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;

                for(int barNum = 0; barNum < 6; barNum++)
                {
                    // In the GetBaseAddressRegister method, the "address" will be set to the higher bits of the BAR, which in case of the I/O BARs contain the port number.
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    if(bar.address && (bar.type == InputOutput))
                        dev.portBase = (uint32_t)bar.address;
                    
                    osd::drivers::Driver* driver = GetDriver(dev, interrupts);
                    if(driver != 0)
                        driverManager -> AddDriver(driver);
                }

                // these print statements will tell about the information about the different devices available

                printf("PCI BUS ");
                printfHex(bus & 0xFF);

                printf(", DEVICE ");
                printfHex(device & 0xFF);

                printf(", FUNCTION ");
                printfHex(function & 0xFF);

                printf(" = VENDOR ");
                printfHex((dev.vendor_id & 0xFF00) >> 8);
                printfHex(dev.vendor_id & 0xFF);
                printf(", DEVICE ");
                printfHex((dev.device_id & 0xFF00) >> 8);
                printfHex(dev.device_id & 0xFF);
                printf("\n");
            }
        }
    }
}

BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressRegister(osd::common::uint16_t bus, osd::common::uint16_t device, osd::common::uint16_t function, osd::common::uint16_t bar)
{
    BaseAddressRegister result;

    uint32_t headertype = Read(bus, device, function, 0x0E) & 0x07F;
    int maxBARs = 6 - (4*headertype);
    if(bar >= maxBARs)
        return result;

    uint32_t bar_value = Read(bus, device, function, 0x10 + 4*bar);
    result.type = (bar_value & 0x01) ? InputOutput : MemoryMapping;
    uint32_t temp;

    if(result.type == MemoryMapping)
    {
        switch ((bar_value >> 1) & 0x3)
        {
        case 0: // 32 bit
        case 1: // 20 bit
        case 2: // 64 bit
        
        default:
            break;
        }
    }
    else
    {
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = ((bar_value >> 3) & 0x01) == 0x1;
    }
    

    return result;
}

osd::drivers::Driver* PeripheralComponentInterconnectController::GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, osd::hardwarecommunication::InterruptManager* interrupts){
    
    osd::drivers::Driver *driver = 0;

    // we will be using it for the instantiation of the devices

    switch (dev.vendor_id)
    {
    case 0x1022: //AMD
        switch(dev.device_id)
        {
            case 0x2000: //am79c973
                printf("AMD am79c973 ");
                break;
        }
        break;   

    case 0x8086: //Intel
        break;
    }

    switch (dev.class_id)
    {
    case 0x03: // graphics
        switch(dev.subclass_id)
        {
            case 0x00: // VGA
                printf("VGA ");
                break;
        }
        break;
    }
    
    return 0;
}



PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor(/* args */)
{

}

PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor()
{

}

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController ::GetDeviceDescriptor(osd::common::uint16_t bus, osd::common::uint16_t device, osd::common::uint16_t function)
{
    PeripheralComponentInterconnectDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0b);
    result.subclass_id = Read(bus, device, function, 0x0a);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3c);

    return result;
}