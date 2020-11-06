#ifndef __OSD__DRIVERS__KEYBOARD_H
#define __OSD__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace osd
{
    namespace drivers
    {
        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();
            // with this handler we can do anything between when the keys are pressed and when 
            // it is displayed on the screen

            virtual void OnKeyDown(char); // this is the method for when key is pressed
            virtual void OnKeyUp(char); // this is the method when the key is left after pressing
        };

        class KeyboardDriver : public osd::hardwarecommunication::InterruptHandler, public Driver
        {
            osd::hardwarecommunication::Port8bit dataport; // it is for reading the typed key values
            osd::hardwarecommunication::Port8bit commandport; // this is for commanding throught the port to the processor what to do

            KeyboardEventHandler* handler; // this handler is for handling the keyboard driver
        public:
            KeyboardDriver(osd::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler);
            ~KeyboardDriver();
            virtual osd::common::uint32_t HandleInterrupt(osd::common::uint32_t esp); // for handling the keyboard interrupts
            virtual void Activate(); // this is for activating the keyboard
        };
    }
}

#endif