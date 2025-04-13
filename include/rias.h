#pragma once

#include <stdlib.h>
#include <stdio.h>

//
//  public functions
//

typedef struct _rias_alloc
{
    void* ptr;
    size_t size;
    char file[128];
    int line;
    struct _rias_alloc* next;
}
rias_alloc;

typedef struct _rias_context
{
    int has_began;
    int alloc_count;
    rias_alloc alloc_head;
}
rias_context;

extern rias_context rias_ctx;

void rias_begin();
void rias_dump();
void rias_end();

void rias_allocate(void* ptr, size_t size, char* file, int line);
void rias_reallocate(void* old_ptr, void* new_ptr, size_t size, char* file, int line);
void rias_deallocate(void* ptr);

void* rias_malloc(size_t size, char* file, int line);
void* rias_calloc(size_t num, size_t size, char* file, int line);
void* rias_realloc(void* ptr, size_t size, char* file, int line);
void rias_free(void* ptr);

//
//  implementations
//

#ifdef RIAS_IMPLEMENTATION

rias_context rias_ctx;

void rias_begin()
{
    rias_ctx.has_began = 1;
    rias_ctx.alloc_count = 0;
    rias_ctx.alloc_head = (rias_alloc){
        .next = NULL
    };
}

void rias_dump()
{
    printf("-- rias exit dump --\n");
    printf("total unfreed allocations: %d\n\n", rias_ctx.alloc_count);

    rias_alloc* current = rias_ctx.alloc_head.next;
    while (current != NULL)
    {
        printf("unfreed allocation: %p\n", current->ptr);
        printf("allocation size: %lu bytes\n", (unsigned long)current->size);
        printf("allocated in: %s (line: %d)\n\n", current->file, current->line);
        
        rias_alloc* next = current->next;
        current = next;
    }
}

void rias_end()
{
#ifndef RIAS_DONT_DUMP_ON_EXIT
    rias_dump();
#endif

    rias_alloc* current = rias_ctx.alloc_head.next;
    while (current != NULL)
    {
        rias_alloc* next = current->next;
        free(current);
        current = next;
    }

    rias_ctx.has_began = 0;
    rias_ctx.alloc_count = 0;
    rias_ctx.alloc_head = (rias_alloc){
        .next = NULL
    };
}

void rias_allocate(void* ptr, size_t size, char* file, int line)
{
    if (ptr == NULL)
        return;

    rias_alloc* current = &rias_ctx.alloc_head;
    while (current->next != NULL)
        current = current->next;

    current->next = malloc(sizeof(rias_alloc));
    current->next->ptr = ptr;
    current->next->size = size;
    snprintf(current->next->file, sizeof(current->next->file), "%s", file);
    current->next->line = line;
    current->next->next = NULL;

    rias_ctx.alloc_count++;
}

void rias_reallocate(void* old_ptr, void* new_ptr, size_t size, char* file, int line)
{
    if (old_ptr == NULL || new_ptr == NULL)
        return;

    rias_alloc* current = &rias_ctx.alloc_head;
    while (current != NULL && current->ptr != old_ptr)
        current = current->next;

    if (current == NULL)
        return;

    current->ptr = new_ptr;
    current->size = size;
    snprintf(current->file, sizeof(current->file), "%s", file);
    current->line = line;
}

void rias_deallocate(void* ptr)
{
    if (ptr == NULL)
        return;

    rias_alloc* previous = NULL;
    rias_alloc* current = &rias_ctx.alloc_head;
    while (current != NULL && current->ptr != ptr)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
        return;

    previous->next = current->next;
    free(current);

    rias_ctx.alloc_count--;
}

void* rias_malloc(size_t size, char* file, int line)
{
    if (rias_ctx.has_began != 1)
    {
        printf("attempted to use rias_malloc without being started\n");
        return NULL;
    }

    void* ptr = malloc(size);
    rias_allocate(ptr, size, file, line);
    return ptr;
}

void* rias_calloc(size_t num, size_t size, char* file, int line)
{
    if (rias_ctx.has_began != 1)
    {
        printf("attempted to use rias_calloc without being started\n");
        return NULL;
    }

    void* ptr = calloc(num, size);
    rias_allocate(ptr, num * size, file, line);
    return ptr;
}

void* rias_realloc(void* ptr, size_t size, char* file, int line)
{
    if (rias_ctx.has_began != 1)
    {
        printf("attempted to use rias_realloc without being started\n");
        return NULL;
    }

    void* new_ptr = realloc(ptr, size);
    rias_reallocate(ptr, new_ptr, size, file, line);
    return new_ptr;
}

void rias_free(void* ptr)
{
    if (rias_ctx.has_began != 1)
    {
        printf("attempted to use rias_free without being started\n");
        return;
    }

    rias_deallocate(ptr);
    free(ptr);
}

#endif

//
//  main hijacking
//

#ifndef RIAS_DONT_HIJACK_MAIN
#ifdef RIAS_IMPLEMENTATION

int main(int argc, char* argv[])
{
    rias_begin();
    rias_main(argc, argv);
    rias_end();
}

#define main rias_main

#endif
#endif

//
//  stdlib function wrappers
//

#ifndef RIAS_DONT_WRAP_STDLIB

#define malloc(size) rias_malloc(size, __FILE__, __LINE__)
#define calloc(num, size) rias_calloc(num, size, __FILE__, __LINE__)
#define realloc(ptr, size) rias_realloc(ptr, size, __FILE__, __LINE__)
#define free(ptr) rias_free(ptr)

#endif