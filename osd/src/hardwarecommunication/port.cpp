#include <hardwarecommunication/port.h>

using namespace osd::common;
using namespace osd::hardwarecommunication;


// main port
Port::Port(uint16_t portnumber)
{
    this->portnumber = portnumber;
}

Port::~Port()
{
}

//8bit Port
Port8bit:: Port8bit(uint16_t portnumber) : Port(portnumber)
{}

Port8bit::~Port8bit()
{}

void Port8bit::Write(uint8_t data)
{
    __asm__ volatile("outb %0, %1": : "a" (data), "Nd" (portnumber));
}

uint8_t Port8bit::Read()
{
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}


//8bit Port Slow
Port8bitSlow::Port8bitSlow(uint16_t portnumber) : Port8bit(portnumber)
{
}

Port8bitSlow::~Port8bitSlow()
{
}

void Port8bitSlow::Write(uint8_t data)
{
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portnumber));
}

//16bit Port
Port16bit::Port16bit(uint16_t portnumber) : Port(portnumber)
{
}

Port16bit::~Port16bit()
{}

void Port16bit::Write(uint16_t data)
{
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint16_t Port16bit::Read()
{
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}

//32bit Port
Port32bit::Port32bit(uint32_t portnumber) : Port(portnumber)
{
}

Port32bit::~Port32bit()
{
}

void Port32bit::Write(uint32_t data)
{
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint32_t Port32bit::Read()
{
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}
