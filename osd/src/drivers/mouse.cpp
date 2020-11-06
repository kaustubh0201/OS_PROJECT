#include <drivers/mouse.h>

using namespace osd::common;
using namespace osd::drivers;
using namespace osd::hardwarecommunication;

MouseEventHandler :: MouseEventHandler()
{

}

void MouseEventHandler :: OnActivate()
{

}
void MouseEventHandler :: OnMouseDown(uint8_t button)
{

}
void MouseEventHandler :: OnMouseUp(uint8_t button)
{

}
void MouseEventHandler :: OnMouseMove(int x, int y)
{

}




MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
: InterruptHandler(0x2C, manager),
dataport(0x60), // initializing the dataport and the commandport
commandport(0x64)
{
    this -> handler = handler;
}

MouseDriver :: ~MouseDriver() // destructor
{}

void MouseDriver :: Activate() // the mouse driver is activated
{
    // initializing the variables
    offset = 0;
    buttons = 0;

    // we are writing and reading in the dataports and the command ports communicate
    commandport.Write(0xA8); //activate interrupts
    commandport.Write(0x20); //activate interrupts
    uint8_t status = (dataport.Read() | 2);
    commandport.Write(0x60);
    dataport.Write(status);

    commandport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read();
}

void printf(char *);

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandport.Read();
    if(!(status & 0x20)) // communicating with the stack pointer
        return esp;

    static int8_t x = 40, y = 12; // basically the cursor
    // this will be the positioning of the mouse cursor we are going to use

    buffer[offset] = dataport.Read();
    
    if(handler == 0)
        return esp;

    offset = (offset + 1) % 3;

    if(offset == 0)
    {
        // this is for changing the position of the mosue 
        if(buffer[1] != 0 || buffer[2] != 0)
            handler -> OnMouseMove((int8_t)buffer[1], - ((int8_t)buffer[2])); 

        // this code if so that when the cursor go over text its color changes
        for(uint8_t i = 0; i < 3; i++)
        {
            if((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i)))
            {
                // the below is for inverting the color
                if(buttons & (0x1 << i))
                    handler -> OnMouseUp(i+1);
                else
                {
                    handler -> OnMouseDown(i+1);
                }
                
            }
        }
        buttons = buffer[0];
        
    }
    
    return esp; // we are returning the stack pointer
}