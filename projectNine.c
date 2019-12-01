#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#define MAX_ID_NUMBER 100

// 1. Request for a contiguous block of memory
// 2. Release of a contiguous block of memory
// 3. Compact unused holes of memory into one single block
// 4. Report the regions of free and allocated memory

//TODO:add a wait queue for processes that don't fit into memory
//TODO: if two adjacent of memory are free, they are merged into one whole

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
 * Sparky Evangelista
 * **/

struct Node *head;
struct Node *temp;
char request[3];
char process[3];
char algo_type[2];
int space_requested;

struct Node
{
    int available_space;
    int start_address;
    int end_address;
    struct Node *next;
    char process_id[MAX_ID_NUMBER];
};

void allocationSpace(int address, struct Node *node)
{
}

void insertNewProcessor()
{
}

// Allocate the first hole that is big enough.
void first_fit(char process_id[3], int space_requested)
{
    temp = head;
    int left_over_space = 0;
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    strcpy(newNode->process_id, process_id);

    //base case in case there is no processes in allocated in memory
    if (temp->next == NULL)
    {
        temp->next = newNode;
        newNode->start_address = 0;
        newNode->end_address = space_requested;
        newNode->next = NULL;
        head->available_space = head->available_space - space_requested;
    }
    else
    {
        while (temp->next != NULL)
        {
            //if the block is being unused and big enough place the process in that memory space
            if (strcmp(temp->next->process_id, "Unused") == 0 &&
                space_requested <= (temp->next->end_address - temp->next->start_address))
            {
                //writing over the empty node
                strcpy(temp->next->process_id, process_id);
                //calculating avalible space left over
                head->available_space = head->available_space - space_requested;

                //if the space_requested is smaller then the space found.
                //make new node with leftover space, if left_over > 0
                if ((left_over_space = (temp->next->end_address - temp->next->start_address) - space_requested) > 0)
                {
                    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
                    strcpy(newNode->process_id, "Unused");
                    newNode->start_address = temp->next->end_address + 1; //starts at the end of prev node
                    newNode->end_address = newNode->start_address + left_over_space;
                    newNode->next = temp->next->next;
                    temp->next->next = newNode;
                }
            }
            else //else continue checking, if not send error
                temp = temp->next;
        }
    }
}

// Allocate the smallest hole that is big enough
void best_fit(char process_id[3], int space_requested)
{
}

// Allocate the largest hole
void worst_fit(char process_id[3], int space_requested)
{
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
        printf("Choose between best_fit, worst_fit, and first_fit. Try again");
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

    //this node will keep track of the amount of available free space
    head = (struct Node *)malloc(sizeof(struct Node));
    head->available_space = inital_memory;

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