 
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace osd
{
    
    struct CPUState
    {
        // the cpu is having many different type of registers which is declared below
        osd::common::uint32_t eax; // this is the general purpose register
        osd::common::uint32_t ebx; // this is the base register
        osd::common::uint32_t ecx; // this is the count register
        osd::common::uint32_t edx; // this is the data register

        osd::common::uint32_t esi; // stack index
        osd::common::uint32_t edi; // data index
        osd::common::uint32_t ebp; // stack base pointer

        /*
        // elemets that are pushed in the stack
        // these are the things we pushed in interrupt subs
        osd::common::uint32_t gs; 
        osd::common::uint32_t fs;
        osd::common::uint32_t es;
        osd::common::uint32_t ds;
        */
        osd::common::uint32_t error; // this is for error code

        osd::common::uint32_t eip; // instruction pointer
        osd::common::uint32_t cs; // code segment
        osd::common::uint32_t eflags; // different flags
        osd::common::uint32_t esp; // stack pointer
        osd::common::uint32_t ss;  // stack segment     
    } __attribute__((packed));
    
    
    class Task
    {
    // we have a task manager here to divide the tasks
    friend class TaskManager; 
    private:
        // we now have our own reserve stack to send some stacks here
        osd::common::uint8_t stack[4096]; // 4 KiB
        // this is the cpu state so we can send the task here from cpu
        CPUState* cpustate;
    public:
        Task(osd::GlobalDescriptionTable *gdt, void entrypoint());
        ~Task();
    };
    
    
    class TaskManager
    {
    private:
        Task* tasks[256]; // this is the array task
        int numTasks; // contains the number of tasks in the array above
        int currentTask; // contains index of the current task
    public:
        TaskManager();
        ~TaskManager();
        bool AddTask(Task* task); // this is to add task to the array
        CPUState* Schedule(CPUState* cpustate); 
        // we will here use the round robin scheduling
        // we have linear stack of task pointers
        // we run into a time interrupt and then we keep on
        // doing the tasks until it is over
        // after that we start over from the top
    };
    
    
    
}


#endif