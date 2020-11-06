#include <multitasking.h>

using namespace osd;
using namespace osd::common;


Task::Task(osd::GlobalDescriptionTable *gdt, void entrypoint())
{
    // this should be the pointer to the start of the reserved stack memory space
    // below it is a pointer to the stack
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
    
    // now we are setting the different registers initially
    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // the below is only needed when we are taking many security levels in account
    // here we are not doing such thing
    // cpustate -> esp = ; 
    cpustate -> eip = (uint32_t)entrypoint;
    // this should always be the code segment selector from the global descriptor table
    cpustate -> cs = gdt->CodeSegmentSelector(); 
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;
    
}

Task::~Task()
{
}

        
TaskManager::TaskManager()
{
    numTasks = 0; // we have no tasks initially
    currentTask = -1; // no current tasks given to it initially
}

TaskManager::~TaskManager()
{
}

bool TaskManager::AddTask(Task* task)
{
    if(numTasks >= 256)
        return false; // we cannot add if the array is full
    tasks[numTasks++] = task; // otherwise we add and return true
    return true;
}

CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if(numTasks <= 0)
        return cpustate; // case if we don't have any task yet
    
    // if we are doing the scheduling then we should update cpu state
    if(currentTask >= 0)
        tasks[currentTask]->cpustate = cpustate; 
    
    // we are doing the round robin
    // if the current taks reach the end array the we start another pointer from the beginning
    if(++currentTask >= numTasks)
        currentTask %= numTasks;
    return tasks[currentTask]->cpustate;
}

    