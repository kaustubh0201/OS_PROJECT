#ifndef __OSD__MEMORYMANAGEMENT_H
#define __OSD__MEMORYMANAGEMENT_H

#include <common/types.h>


namespace osd
{
    // we are basically making a double linked list here
    struct MemoryChunk
    {
        MemoryChunk *next;
        MemoryChunk *prev;
        bool allocated;
        osd::common::size_t size;
    };
    
    
    class MemoryManager
    {
        
    protected:
        // this is the pointer to the first chunk
        MemoryChunk* first;
    public:
        
        static MemoryManager *activeMemoryManager;
        
        // constructors and destructors are defined here
        MemoryManager(osd::common::size_t first, osd::common::size_t size);
        ~MemoryManager();
        
        void* malloc(osd::common::size_t size);
        void free(void* ptr);
    };
}

/*
void* operator new(unsigned size);
void* operator new[](unsigned size);

// placement new
void* operator new(unsigned size, void* ptr);
void* operator new[](unsigned size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);
*/

#endif