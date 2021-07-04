/*
  Question 4:
  Dynamic Memory Allocation (Stack vs Heap)

  The Stack
  What is the stack? It's a special region of your computer's memory that
  stores temporary variables created by each function (including the main()
  function). The stack is a 'LIFO' (last in, first out) data structure,
  that is managed and optimized by the CPU quite closely.
  Every time a function declares a new variable, it is 'pushed' onto the stack.
  Then every time a function exits, all of the variables pushed onto the stack\
  by that function, are freed (that is to say, they are deleted).
  Once a stack variable is freed, that region of memory becomes available for
  other stack variables. The advantage of using the stack to store variables,
  is that memory is managed for you.
  You don't have to allocate memory by hand, or free it once you don't need it
  any more. What's more, because the CPU organizes stack memory so efficiently,
  reading from and writing to stack variables is very fast.
  The size of the stack for a method is calculated when the program is compiling.
  If the stack calls goes beyond the memory, then an error will occurs
  i.e. stack overflow.

  The Heap
  The heap is a region of your computer's memory that is not managed
  automatically for you, and is not as tightly managed by the CPU.
  It is a more free-floating region of memory (and is larger).
  To allocate memory on the heap, you must use malloc() or calloc(),
  which are built-in C functions. Once you have allocated memory on the heap,
  you are responsible for using free() to deallocate that memory once
  you don't need it any more. If you fail to do this, your program will have
  what is known as a memory leak.
  So, we can keep the data in the heap till what time we need
  i.e. we can have control on the allocation and de-allocation.
 */

#if 0
For Stack
- very fast access
- don't have to explicitly de-allocate variables
- space is managed efficiently by CPU, memory will not become fragmented
- local variables only
- limit on stack size (OS-dependent)
- variables cannot be resized

For Heap
- variables can be accessed globally
- no limit on memory size
- (relatively) slower access
- no guaranteed efficient use of space, memory may become fragmented over time
  as blocks of memory are allocated, then freed
- you must manage memory(you're in charge of allocating and freeing variables)
- variables can be resized using realloc()

#endif
