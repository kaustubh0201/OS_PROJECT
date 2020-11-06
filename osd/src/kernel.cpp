#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>
#include <memorymanagement.h>


// #define GRAPHICSMODE

using namespace osd;
using namespace osd::common;
using namespace osd::drivers;
using namespace osd::hardwarecommunication;
using namespace osd::gui;


// the print fucntion that has been written explicitly 
void printf(char *str)
{
    // separated this in arrays of unsigned integers 
    static uint16_t *VideoMemory = (uint16_t *)0xb8000;

    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {

        switch(str[i])
        {
            case '\n':
                y++;
                x = 0;
                break;
            default:
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
                x++;
        }

        if(x >= 80)
        {
            y++;
            x = 0;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
            {
                for(x = 0; x < 80; x++)
                    {
                        VideoMemory[80*y*x] = (VideoMemory[80*y*x] & 0xFF00) | ' ';
                    }
            }
            x = 0;
            y = 0;
        }
    }
}

// hexadecimal print
void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0F];
    foo[1] = hex[key & 0x0F];
    printf(foo);
}

// extending the keyboard event handler class
class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

// extending the mouse event handler
class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:

    MouseToConsole()
    {
        static uint16_t *VideoMemory = (uint16_t *)0xb8000;

        x = 40; y = 12;

        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) | ((VideoMemory[80*y+x] & 0x0F00) << 4) | (VideoMemory[80*y+x] & 0x00FF);
    }

    void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t *VideoMemory = (uint16_t *)0xb8000;

        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) | ((VideoMemory[80*y+x] & 0x0F00) << 4) | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset; 
        // we have to put the constraints so that the cursor don't run out of the screen
        if(x < 0) x = 0;
        if(x >= 80) x = 79;

        y += yoffset;
        // constraints for the cursor
        if(y < 0) y = 0;
        if(y >= 25) y = 24;


        // we are changing the color bits in the video memory
        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4) | ((VideoMemory[80*y+x] & 0x0F00) << 4) | (VideoMemory[80*y+x] & 0x00FF); 
    }
};

void taskA()
{
    while (true)
    {
        printf("A");
    }
    
}

void taskB()
{
    while (true)
    {
        printf("B");
    }
    
}


typedef void (*constructor)();
extern "C" constructor *start_ctors;
extern "C" constructor *end_ctors;

extern "C" void callConstructors()
{
    for (constructor *i = start_ctors; i != end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t /*magicnumber*/)
{   
    // string that will be showm on the screen when OS starts
    printf("This operating system is written by Kaustubh.\n");

    GlobalDescriptionTable gdt;

    // we are allocating the dynamic memory here

    
    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
    // we are also subtracting the heap where the memory space is starting
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);

    // we are initializing the memory space 
    // also adding some values to it

    // this will print the hex code of the heap allocated
    printf("heap: 0x");
    printfHex((heap >> 24) & 0xFF);
    printfHex((heap >> 16) & 0xFF);
    printfHex((heap >> 8) & 0xFF);
    printfHex((heap) & 0xFF);

    void* allocated = memoryManager.malloc(1024);
    // this will print the hex code of the allocated memory space
    printf("\nallocated: 0x");
    printfHex(((size_t)allocated >> 24) & 0xFF);
    printfHex(((size_t)allocated >> 16) & 0xFF);
    printfHex(((size_t)allocated >> 8) & 0xFF);
    printfHex(((size_t)allocated) & 0xFF);
    printf("\n");


    // we are declaring the task manager here
    TaskManager taskManager;
    // here we are making some tasks
    // in this A task will be run infinitely and then B task
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    
    // we are adding the task in the task manager
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    
    // the interrupt handler need to talk to the task manager for the task
    InterruptManager interrupts(0x20, &gdt, &taskManager);

    //for some status message
    printf("Initializing Hardware, Stage 1\n");

#ifdef GRAPHICSMODE
    // we are initializing the drivers above the 
    Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
#endif

    DriverManager drvManager;

        //PrintfKeyboardEventHandler kbhandler; // initialzing the keyboard handler
        // initiating the keyboard driver
        //KeyboardDriver keyboard(&interrupts, &kbhandler);
        #ifdef GRAPHICSMODE
            KeyboardDriver keyboard(&interrupts, &desktop);
        #else
            PrintfKeyboardEventHandler kbhandler;
            KeyboardDriver keyboard(&interrupts, &kbhandler);
        #endif
        drvManager.AddDriver(&keyboard);

        //MouseToConsole mousehandler; // initializing the mouse handler
        // inititating the mouse driver
        //MouseDriver mouse(&interrupts, &mousehandler);
        #ifdef GRAPHICSMODE
            MouseDriver mouse(&interrupts, &desktop);
        #else
            MouseToConsole mousehandler;
            MouseDriver mouse(&interrupts, &mousehandler);
        #endif
        drvManager.AddDriver(&mouse);

        // initializing the PCI
        PeripheralComponentInterconnectController PCIController;
        PCIController.SelectDrivers(&drvManager, &interrupts);

        // initilizing the Video Graphics Mode
        #ifdef GRAPHICSMODE
            VideoGraphicsArray vga;
        #endif

    
    // status message
    printf("Initializing Hardware, Stage 2\n");
    // this will activate all the drivers
    drvManager.ActivateAll(); 

    // status message
    printf("Initializing Hardware, Stage 3\n");
    
#ifdef GRAPHICSMODE
    // writing the code for making the screen blue
    vga.SetMode(320, 200, 8);

    // below I have initialized two windows on the desktop
    Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
    desktop.AddChild(&win1);// also updating the desktop according to window
    Window win2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
    desktop.AddChild(&win2);// also updating the desktop according to window
#endif
    // activating the iDT and PiC
    interrupts.Activate();

    /*
    for(int32_t y = 0; y < 200; y++)
        for(int32_t x = 0; x < 320; x++)
            vga.PutPixel(x, y, 0x00, 0x00, 0xA8);
    */
    while (1)
    {
        #ifdef GRAPHICSMODE
        // we have put it in the loop so that the desktop is updated again and again
            desktop.Draw(&vga);
        #endif
    }
}
