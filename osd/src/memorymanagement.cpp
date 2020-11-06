#include <memorymanagement.h>

using namespace osd;
using namespace osd::common;


MemoryManager* MemoryManager::activeMemoryManager = 0;
        
MemoryManager::MemoryManager(size_t start, size_t size)
{
    activeMemoryManager = this;
    
    if(size < sizeof(MemoryChunk))
    {
        first = 0; // the first memory chunk is assigned
    }
    else
    {
        // this is where we put the first memory chunk
        first = (MemoryChunk*)start;
        
        first -> allocated = false;
        first -> prev = 0;
        first -> next = 0;
        first -> size = size - sizeof(MemoryChunk);
    }
}

MemoryManager::~MemoryManager()
{
    if(activeMemoryManager == this)
        activeMemoryManager = 0;
}
        
void* MemoryManager::malloc(size_t size)
{
    MemoryChunk *result = 0;
    
    // this is iterating the linked list and allocating chunks
    for(MemoryChunk* chunk = first; chunk != 0 && result == 0; chunk = chunk->next)
        if(chunk->size > size && !chunk->allocated)
            result = chunk;
        
    if(result == 0)
        return 0;
    
    // if the chunk is too small we will mark it as allocated
    if(result->size >= size + sizeof(MemoryChunk) + 1)
    {
        MemoryChunk* temp = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);
        
        // we also have to get the memory location for the next chunk
        temp->allocated = false;
        temp->size = result->size - size - sizeof(MemoryChunk);
        temp->prev = result;
        temp->next = result->next;
        if(temp->next != 0)
            temp->next->prev = temp;
        
        result->size = size;
        result->next = temp;
    }
    
    // we get the boolean value that the memory has been allocated
    result->allocated = true;
    return (void*)(((size_t)result) + sizeof(MemoryChunk));
}

void MemoryManager::free(void* ptr)
{
    // we are subtracting the size of memory chunk that has to be freed
    MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));
    
    chunk -> allocated = false;
    
    if(chunk->prev != 0 && !chunk->prev->allocated)
    {
        // we are merging the chunk with the previous one
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(MemoryChunk);

        if(chunk->next != 0)
            chunk->next->prev = chunk->prev;
        
        chunk = chunk->prev; // we move the chunk pointer to the previous one
    }
    
    if(chunk->next != 0 && !chunk->next->allocated)
    {
        chunk->size += chunk->next->size + sizeof(MemoryChunk);
        chunk->next = chunk->next->next;
        if(chunk->next != 0)
            chunk->next->prev = chunk;
    }
    
}


/*

void* operator new(unsigned size)
{
    if(osd::MemoryManager::activeMemoryManager == 0)
        return 0;
    return osd::MemoryManager::activeMemoryManager->malloc(size);
}

void* operator new[](unsigned size)
{
    if(osd::MemoryManager::activeMemoryManager == 0)
        return 0;
    return osd::MemoryManager::activeMemoryManager->malloc(size);
}

void* operator new(unsigned size, void* ptr)
{
    return ptr;
}

void* operator new[](unsigned size, void* ptr)
{
    return ptr;
}

void operator delete(void* ptr)
{
    if(osd::MemoryManager::activeMemoryManager != 0)
        osd::MemoryManager::activeMemoryManager->free(ptr);
}

void operator delete[](void* ptr)
{
    if(osd::MemoryManager::activeMemoryManager != 0)
        osd::MemoryManager::activeMemoryManager->free(ptr);
}

*/
