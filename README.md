# Contiguous_Memory_Allocation_Simulator
This program illustrates how processes are allocation in continuous memory using a linked list.


## The program responds to four different requests:
  1. Request for a contiguous block of memory
  2. Release of a contiguous block of memory
  3. Compact unused holes of memory into one single block
  4. Report the regions of free and allocated memory
  
The program will be passed the initial amount of memory at startup. For example, the following initializes the program with 1 MB (1,048,576 bytes) of memory:
#### ./Contingous_Memory_Allocation_Simulator 1048576

Once the program has started, it will present the user with the following prompt:
#### allocator>

It will then respond to the following commands: 
#### RQ (request), RL (release), C (compact), STAT (status report), and X (exit).
 
A request for 40,000 bytes will appear as follows:
#### allocator>RQ P0 40000 W
The first parameter to the RQ command is the new process that requires the memory, followed by the amount of memory being requested, and finally the strategy. (In this situation, “W” refers to worst fit.)
Similarly, a release will appear as:
#### allocator>RL P0
This command will release the memory that has been allocated to process P0.
The command for compaction is entered as:
#### allocator>C
This command will compact unused holes of memory into one region.
Finally, the STAT command for reporting the status of memory is entered as:
#### allocator>STAT
Given this command, the program will report the regions of memory that are allocated and the regions that are unused. For example, one possible arrangement of memory allocation would be as follows:


* Addresses [0:315000] Process P1
* Addresses [315001: 512500] Process P3
* Addresses [512501:625575] Unused
* Addresses [625575:725100] Process P6
* Addresses [725001] . . .


## Allocating Memory
The program will allocate memory using one of the three approachesthat is passed to the RQ command. The flags are:
* F—first fit
* B—best fit
* W—worst fit

* First fit. Allocate the first hole that is big enough. Searching can start either at the beginning of the set of holes or at the location where the previous first-fit search ended. We can stop searching as soon as we find a free hole that is large enough.
 
* Best fit. Allocate the smallest hole that is big enough. Search the entire list, unless the list is ordered by size. This strategy produces the smallest leftover hole.
 
* Worst fit. Allocate the largest hole. Again, search the entire list, unless it is sorted by size. 
This strategy produces the largest leftover hole, which may be more useful
than the smaller leftover hole from a best-fit approach.

The program will keep track of the different holes representing available memory. When a request for memory arrives, it will allocate the memory from one of the available holes based on the allocation strategy. If there is insufficient memory to allocate to a request, it will output an error message and reject the request, and put the process in a waitng queue

The program will also keep track of which region of memory has been allocated to which process. This is necessary to support the STAT command and is also needed when memory is released via the RL command, as the process releasing memory is passed to this command. If a partition being released is adjacent to an existing hole the two holes will merge into a single hole.

## Compaction
If the user enters the C command, the program will compact the set of holes into one larger hole. For example, if you have four separate holes of size 550 KB, 375 KB, 1,900 KB, and 4,500 KB, the program will combine these four holes into one large hole of size 7,325 KB.
