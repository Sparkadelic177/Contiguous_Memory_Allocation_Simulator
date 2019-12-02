#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#define MAX_ID_NUMBER 100

//TODO:add a wait queue for processes that don't fit into memory
//TODO: if two adjacent of memory are free, they are merged into one whole
//TODO: refactor repeated code into a function
//TODO: Compact and Release function

// 1. Request for a contiguous block of memory
// 2. Release of a contiguous block of memory
// 3. Compact unused holes of memory into one single block
// 4. Report the regions of free and allocated memory

/**
 * 
 * First fit. Allocate the first hole that is big enough. 
 * Searching can start either at the beginning of the set 
 * of holes or at the location where the previous first-fit 
 * search ended. We can stop searching as soon as we find a free hole that is large enough.
 * 
 * Best fit. Allocate the smallest hole that is big enough. 
 * We must search the entire list, unless the list is ordered by size. 
 * This strategy produces the smallest leftover hole.
 * 
 * Worst fit. Allocate the largest hole. 
 * Again, we must search the entire list, unless it is sorted by size. 
 * This strategy produces the largest leftover hole, which may be more useful
 * than the smaller leftover hole from a best-fit approach. 
 * 
 * 
 * Code play by play:
 * 
 * The head node keeps track on the total avaliable space in memory,and is a dummy node
 * The block_of_space servers as the block of inital free space
 * The temp node help us traverse through the linkedlist
 * 
 * If a new process has space to fit inside a fragmented space and there are address spaces left over
 * after inserting, a new node is created taking up the left over address spaces 
 * and initalized as unused space
 * 
 * If two free spaces are adjacent to each other they are merged together into one node / block of space
 * 
 * **/

struct Node *head;           //dummy node keeps track of avaliable main memory
struct Node *block_of_space; //initial block of main memory
struct Node *temp;           // Pointer used to traverse throught contingous memory

char request[3];
char process[3];
char algo_type[2];
int last_address_space;
int space_requested;

struct Node
{
    int available_space; //space in that section / fragment
    int start_address;
    int end_address;
    struct Node *next;
    char process_id[MAX_ID_NUMBER];
};

// Allocate the first hole that is big enough.
void first_fit(char process_id[3], int space_requested)
{
    temp = head;
    int left_over_space = 0; //helps calculate if there was space left over

    while (temp->next != NULL)
    {
        //if the block is being unused and big enough place the process in that memory space
        if (strcmp(temp->next->process_id, "Unused") == 0 && temp->next->available_space >= space_requested)
        {
            //calculating avalible space left over
            head->available_space = head->available_space - space_requested;

            //writing over the unused node
            strcpy(temp->next->process_id, process_id);
            temp->next->end_address = temp->next->start_address + space_requested;

            //if the space_requested is smaller then the space found.
            //make new node with leftover space, if left_over > 0
            left_over_space = temp->next->available_space - space_requested;
            if (left_over_space > 0)
            {
                struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
                strcpy(newNode->process_id, "Unused");
                newNode->available_space = left_over_space;
                temp->next->available_space = space_requested;
                newNode->start_address = temp->next->end_address + 1; //starts at the end of prev node
                newNode->end_address = newNode->start_address + left_over_space;
                if (newNode->end_address > last_address_space) //if end_address is bigger than inital space
                    newNode->end_address = last_address_space;
                newNode->next = temp->next->next;
                temp->next->next = newNode;
            }

            return; //halt the function, when space is found and inserted
        }
        else //else continue traversing linkedlist
            temp = temp->next;
    }

    //if temp pointer reached the end of the LL without inserting & space is still avaliable
    printf("There is no space to place process %s, of %dkb\n", process_id, space_requested);
}

// Allocate the smallest hole that is big enough
void best_fit(char process_id[3], int space_requested)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    int smallest_space = 999999; //tracks the smallest space
    int left_over_space = 0;
    temp = head;

    //this loop will find the smallest possible avaliable space
    while (temp->next != NULL)
    {

        //if space is unused and big enough to fit, track min space
        if (strcmp(temp->next->process_id, "Unused") == 0 && temp->next->available_space >= space_requested)
        {
            //track min space
            if (temp->next->available_space <= smallest_space)
                smallest_space = temp->next->available_space;
        }
        else //keep traversing
            temp = temp->next;
    }

    temp = head;
    //this loop will place the new processor in the space
    while (temp->next != NULL)
    {
        //travserse till you find the smallest spot found
        if (temp->next->available_space == smallest_space)
        {
            //calculating avalible space left over
            head->available_space = head->available_space - space_requested;

            //writing over the space
            strcpy(temp->next->process_id, process_id);
            temp->next->end_address = temp->next->start_address + space_requested;

            //if theres space left over, make a new node representing unused left over space
            left_over_space = temp->next->available_space - space_requested;
            if (left_over_space > 0)
            {
                struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
                strcpy(newNode->process_id, "Unused");
                newNode->available_space = left_over_space;
                temp->next->available_space = space_requested;
                newNode->start_address = temp->next->end_address + 1; //starts at the end of prev node
                newNode->end_address = newNode->start_address + left_over_space;
                if (newNode->end_address > last_address_space) //if end_address is bigger than inital space
                    newNode->end_address = last_address_space;
                newNode->next = temp->next->next;
                temp->next->next = newNode;
            }

            return; //halt the function, when space is found and inserted
        }
        else
            temp = temp->next;
    }

    //if temp pointer reached the end of the LL without inserting & space is still avaliable
    printf("There is no space to place process %s, of %dkb\n", process_id, space_requested);
}

// Allocate the largest hole
void worst_fit(char process_id[3], int space_requested)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    int largest_space = -999999; //tracks the smallest space
    int left_over_space = 0;
    temp = head;

    //this loop will find the smallest possible avaliable space
    while (temp->next != NULL)
    {

        //if space is unused and big enough to fit, track min space
        if (strcmp(temp->next->process_id, "Unused") == 0 && temp->next->available_space >= space_requested)
        {
            //track max space
            if (temp->next->available_space >= largest_space)
                largest_space = temp->next->available_space;
        }
        else //keep traversing
            temp = temp->next;
    }

    temp = head;
    //this loop will place the new processor in the space
    while (temp->next != NULL)
    {
        //travserse till you find the largest spot found
        if (temp->next->available_space == largest_space)
        {
            //calculating avalible space left over
            head->available_space = head->available_space - space_requested;

            //writing over the space
            strcpy(temp->next->process_id, process_id);
            temp->next->end_address = temp->next->start_address + space_requested;

            //if theres space left over, make a new node representing unused left over space
            left_over_space = temp->next->available_space - space_requested;
            if (left_over_space > 0)
            {
                struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
                strcpy(newNode->process_id, "Unused");
                newNode->available_space = left_over_space;
                temp->next->available_space = space_requested;
                newNode->start_address = temp->next->end_address + 1; //starts at the end of prev node
                newNode->end_address = newNode->start_address + left_over_space;
                if (newNode->end_address > last_address_space) //if end_address is bigger than inital space
                    newNode->end_address = last_address_space;
                newNode->next = temp->next->next;
                temp->next->next = newNode;
            }

            return; //halt the function, when space is found and inserted
        }
        else
            temp = temp->next;
    }

    //if temp pointer reached the end of the LL without inserting & space is still avaliable
    printf("There is no space to place process %s, of %dkb \n", process_id, space_requested);
}

//this method handles all 3 algorithms for finding memeory spaces
void request_memory(char process_id[3], int space_requested, char algo[2])
{
    if (strcmp("W", algo) == 0)
        worst_fit(process_id, space_requested);
    else if (strcmp("B", algo) == 0)
        best_fit(process_id, space_requested);
    else if (strcmp("F", algo) == 0)
        first_fit(process_id, space_requested);
    else
    {
        printf("Choose between best_fit, worst_fit, and first_fit. Try again\n");
        return;
    }
}

void release_memory(char process_id[3])
{
}

void compact()
{
}

void status_report()
{
    temp = head;
    printf("avaliable space left: %d\n", head->available_space);
    while (temp->next != NULL)
    {
        printf("Addresses [%d : %d] Process %s\n", temp->next->start_address, temp->next->end_address, temp->next->process_id);
        temp = temp->next;
    }
}

int main(int argc, char *argv[])
{

    int inital_memory = atoi(argv[1]);
    printf("%d\n", inital_memory);
    char user_input[128];

    //read the user if, if return value is negative, syscall error
    printf("allocator>");
    fflush(stdout);

    if (read(0, user_input, 128) < 0)
        write(2, "An error occurred in the read.\n", 31);

    //the head node is a dummy node
    head = (struct Node *)malloc(sizeof(struct Node));
    //block_of_space is the amount of space in memory
    block_of_space = (struct Node *)malloc(sizeof(struct Node));

    strcpy(head->process_id, "Dummy Node");
    head->start_address = -1;
    head->end_address = -1;
    head->available_space = inital_memory;
    head->next = block_of_space;

    strcpy(block_of_space->process_id, "Unused");
    block_of_space->start_address = 0;
    block_of_space->end_address = inital_memory;
    block_of_space->available_space = block_of_space->end_address - block_of_space->start_address;
    block_of_space->next = NULL;

    last_address_space = inital_memory;

    while (user_input[0] != 'x')
    {
        if ((sscanf(user_input, "%s %s", request, process)) < 0)
            write(2, "An error occurred in the read.\n", 31);

        //TODO: find a way to lowercase the user input
        // char requested = tolower(request);
        // printf("%s\n", &requested);

        if (strcmp("RQ", request) == 0) //request new process command
        {

            sscanf(user_input, "%s %s %d %s", request, process, &space_requested, algo_type);
            request_memory(process, space_requested, algo_type);
        }
        else if (strcmp("RL", request) == 0) //release memory command
        {

            sscanf(user_input, "%s %s", request, process);
            release_memory(process);
        }
        else if (strcmp("C", request) == 0) //compact
        {
            compact();
        }
        else if (strcmp("STAT", request) == 0) //status of memory
        {
            status_report();
        }
        else //the command is not reconiged
        {
            printf("This command is not recognized, try again\n");
        }

        status_report();

        printf("allocator>");
        fflush(stdout);

        if (read(0, user_input, 128) < 0)
            write(2, "An error occurred in the read.\n", 31);
    }
}

//Sparky's Project