#ifndef __OSD__DRIVERS__MOUSE_H
#define __OSD__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace osd
{
    namespace drivers
    {
        class MouseEventHandler
        {
        public:
            MouseEventHandler(); // this is the mouse event handler

            virtual void OnActivate(); // activating the mouse driver
            virtual void OnMouseDown(osd::common:: uint8_t button); // when mouse button is clicked
            virtual void OnMouseUp(osd::common::uint8_t button); // when mouse button is relaeased
            virtual void OnMouseMove(int x, int y); // this is the co-ordinates of the mouse pointer on the screen
        };

        // this is the driver for the mouse 
        class MouseDriver : public osd::hardwarecommunication::InterruptHandler, public Driver
        {
            osd::hardwarecommunication::Port8bit dataport; // taking the interrupt to the port for the mouse
            osd::hardwarecommunication::Port8bit commandport; // commanding the processor what to do

            osd::common::uint8_t buffer[3]; // transmission of mouse is through these buffers only
            osd::common::uint8_t offset; 
            osd::common::uint8_t buttons; // this is the right click and the left click

            
            MouseEventHandler* handler; // this is for the handling of the mouse

        public:
            MouseDriver(osd::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler); // constructor
            ~MouseDriver(); // destructor
            virtual osd::common::uint32_t HandleInterrupt(osd::common::uint32_t esp); // for handling the interrupts of the mouse
            virtual void Activate(); // for activating the driver of the mouse
        };
    }
}

#endif