#include<drivers/vga.h>

using namespace osd::drivers;
using namespace osd::common;

void VideoGraphicsArray::WriteRegisters(osd::common::uint8_t* registers)
{
    // for misc
    miscPort.Write(*(registers++));

    // for sequencer
    for(uint8_t i = 0; i < 5; i++)
    {
        sequenceIndexPort.Write(i);
        sequenceDataPort.Write(*(registers++));
    }

    // for cathode ray tube controller
    // this is for some security feature of cathode ray tube 
    crtcIndexPort.Write(0x03);
    crtcDataPort.Write(crtcDataPort.Read() | 0x80);
    crtcIndexPort.Write(0x11);
    crtcDataPort.Write(crtcDataPort.Read() & ~0x80);

    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;

    for(uint8_t i = 0; i < 25; i++)
    {
        crtcIndexPort.Write(i);
        crtcDataPort.Write(*(registers++));
    }

    // for graphics controller
    for(uint8_t i = 0; i < 9; i++)
    {
        graphicsControllerIndexPort.Write(i);
        graphiceControllerDataPort.Write(*(registers++));
    }

    // for attribute controller
    for (uint8_t i = 0; i < 21; i++)
    {
        attributeControllerResetPort.Read();
        attributeControllerIndexPort.Write(i);
        attributeControllerWritePort.Write(*(registers++));
    }

    attributeControllerResetPort.Read();
    attributeControllerIndexPort.Write(0x20);


}

uint8_t* VideoGraphicsArray::GetFrameBufferSegment()
{
    graphicsControllerIndexPort.Write(0x06);
    uint8_t segmentNumber = ((graphiceControllerDataPort.Read() >> 2) & 0x03);
    switch(segmentNumber)
    {
        default:
        case 0: return (uint8_t*)0x00000;
        case 1: return (uint8_t*)0xA0000;
        case 2: return (uint8_t*)0xB0000;
        case 3: return (uint8_t*)0xB8000;
    }
}

VideoGraphicsArray::VideoGraphicsArray() :
    miscPort(0x3c2),
    crtcIndexPort(0x3d4),
    crtcDataPort(0x3d5),
    sequenceIndexPort(0x3c4),
    sequenceDataPort(0x3c5),
    graphicsControllerIndexPort(0x3ce),
    graphiceControllerDataPort(0x3cf),
    attributeControllerIndexPort(0x3c0),
    attributeControllerReadPort(0x3c1),
    attributeControllerWritePort(0x3c0),
    attributeControllerResetPort(0x3da)
{

}

VideoGraphicsArray::~VideoGraphicsArray()
{

}

bool VideoGraphicsArray::SetMode(osd::common::uint32_t width, osd::common::uint32_t height, osd::common::uint32_t colordepth)
{
    if(!SupportsMode(width, height, colordepth))
        return false;
        
        // we have to write this data to their respective ports using WriteRegisters()
    unsigned char g_320x200x256[] =
        {
            // MISC MISCELLANEOUS
            0x63,
            // SEQ SEQUENCER
            0x03, 0x01, 0x0F, 0x00, 0x0E,
            // CRTC CATHODE RAY TUBE CONTROLLER
            0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
            0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0x89, 0xA3,
            0xFF,
            // GC GRAPHICS CONTROLLER
            0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
            0xFF,
            // AC ATTRIBUTE CONTROLLER
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x41, 0x00, 0x0F, 0x00, 0x00
        };

        WriteRegisters(g_320x200x256);
        return true;
}

bool VideoGraphicsArray::SupportsMode(osd::common::uint32_t width, osd::common::uint32_t height, osd::common::uint32_t colordepth){

    return width == 320 && height == 200 && colordepth == 8;

}
void VideoGraphicsArray::PutPixel(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b)
{
    PutPixel(x, y, GetColorIndex(r,g,b));
}    

osd::common::uint8_t VideoGraphicsArray::GetColorIndex(osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b)
{
    // this function is here to get the different color index 
    if(r == 0x00 && g == 0x00 && b == 0x00) return 0x00; // black
    if(r == 0x00 && g == 0x00 && b == 0xA8) return 0x01; // blue
    if(r == 0x00 && g == 0xA8 && b == 0x00) return 0x02; // green
    if(r == 0xA8 && g == 0x00 && b == 0x00) return 0x04; // red 
    if(r == 0xFF && g == 0xFF && b == 0xFF) return 0x3F; // white
    
    return 0x00;
}

// this putpixel method is for putting the pixels on the screen
void VideoGraphicsArray::PutPixel(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t colorIndex)
{
    if(x < 0 || 320 <= x || y < 0 || 200 <= y) return;
    uint8_t* pixelAddress = GetFrameBufferSegment() + 320*y + x;
    *pixelAddress = colorIndex;
}

void VideoGraphicsArray::FillRectangle(osd::common::uint32_t x, osd::common::uint32_t y, osd::common::uint32_t w, osd::common::uint32_t h, osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b)
{
    for(int32_t Y = y; Y < y+h; Y++)
        for(int32_t X = x; X < x+w; X++)
            PutPixel(X, Y, r, g, b);
}