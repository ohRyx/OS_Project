#include <stdio.h>
#include <string.h>
#define max_size 905

//Declare blockArray's individual block sizes as per documentation
int blockArray[12] = {160, 350, 650, 80, 410, 50, 720, 905, 570, 130, 260, 830};

//Declare allocation array all to -1 to use for process to block allocation
//If value stays at -1 during runtime, means process is not allocated
int allocArray[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

//Program uses this variable for array size in for loops
int blockLength = sizeof(blockArray) / sizeof(blockArray[0]);

//Declare the three fit algorithms
int computeFirstFit();
int computeWorstFit();
int computeBestFit();

//Declare the for loop counter variables
int i, j;

int main(){
    //Declare an array with 10 elements for user input
    int processArray[10];
    //Uses this array length number for loop counter limit  
    int processLength = sizeof(processArray) / sizeof(processArray[0]);
    //Declare user input
    int userInput;

    //Displays the message to users to key in 10 numbers including the message to make sure user keys in maximum value of 905KB
    printf("\nEnter the 10 process numbers (Please do not exceed %dKB per process size):\n", max_size);
    for (i=0; i<processLength;i++){
        //A do-while loop to loop users to re-enter another number if user input exceeds 905 or is a negative number
        do{
            printf("\nEnter process number #%d\n", i+1);
            scanf("\n%d", &userInput);

            //Displays error message if input is not between 0 and 905 before prompting user to enter another 
            if(userInput<0 || userInput>max_size){
                printf("\nInvalid number entered (Must be between 0 and 905) for process #%d. Please enter another number.", i+1);
            }
        }while(userInput<0 || userInput>max_size);

        //Adds the user input into processArray after validation is done
        processArray[i] = userInput;        
    }

    printf("\n");
    for (i=0; i<processLength;i++){
        printf("P%d\t %d\n", i+1, processArray[i]);
    }
    printf("\n"); 

    //Takes in the array and array length parameters to compute the 3 algorithms
    computeFirstFit(processArray, processLength);
    computeWorstFit(processArray, processLength);
    computeBestFit(processArray, processLength);
    return 0;
}

// First fit : Allocate the first hole that is big enough
int computeFirstFit(int p[], int pLength){

    //Declare two arrays and copy the original blockArray and allocation Array so the computation
    //will not affect the other two methods' calculation
    int firstFitAllocArray[10];
    int firstFitBlockArray[12];
    memcpy(firstFitBlockArray, blockArray, sizeof blockArray);
    memcpy(firstFitAllocArray, allocArray, sizeof allocArray);

    //For visual purpose
    printf("First Fit Algorithm\n");
    printf("Process\t\tSize\tAssigned Block\tBlock Memory\tRemaining Memory");
    
    //Iterates each process by input sequence and find blocks that can hold its size
    //without regards to its best fit
    for(i=0; i<pLength; i++){
        for(j=0; j<blockLength; j++){
            //Allocates process to block as soon as it finds a block big enough to hold the process size
            if(p[i]<=firstFitBlockArray[j]){
                //Allocation Block number j to firstFitAllocArray[i] process
                firstFitAllocArray[i] = j;

                //Reduce the available memory in this particular block firstFitBlockArray[j]
                firstFitBlockArray[j] -= p[i];
                break;
            }            
        }
    }

    //Displays the remaining memory partition in a table format for easy visualisation purposes.
    printf("\n");
    for (i = 0; i < 10; i++){ 
        printf("%d\t\t%d\t",(i+1),p[i]); 
        if (firstFitAllocArray[i] != -1) 
            printf("%d\t\t%d\t\t%d\n",firstFitAllocArray[i] + 1, blockArray[firstFitAllocArray[i]], firstFitBlockArray[firstFitAllocArray[i]]);
        else
            printf("Not Allocated. This process needs to wait for a free block.\n");
    }
    printf("\n\n");
}

// Worst fit : Allocate the largest hole; must also search
// entire list (Produces the largest leftover hole).
int computeWorstFit(int p[], int pLength){

    //Declare two arrays and copy the original blockArray and allocation Array so the computation
    //will not affect the other two methods' calculation
    int worstFitAllocArray[10];
    int worstFitBlockArray[12];
    memcpy(worstFitBlockArray, blockArray, sizeof blockArray);
    memcpy(worstFitAllocArray, allocArray, sizeof allocArray);

    //Uses this variable to find the worst fit block for the process it's checking
    int worstFitId;

    //For visual purpose
    printf("Worst Fit Algorithm\n");
    printf("Process\t\tSize\tAssigned Block\tBlock Memory\tRemaining Memory");

    //Iterates each process by input sequence and find the largest block space available
    for(i=0; i<pLength; i++){
        //Uses this variable to find the worst fit block for the current process
        worstFitId = -1;
        for(j=0; j<blockLength; j++){
            //Checks if the block is larger than the process size
            if(p[i]<=worstFitBlockArray[j]){
                //Assigns the process to the block if unassigned
                if(worstFitId == -1){
                    worstFitId = j;
                }
                //Otherwise, try to find bigger blocks available for the process and re-assigns
                //to that block instead
                else if (worstFitBlockArray[worstFitId] < worstFitBlockArray[j])
                worstFitId = j;
            }
        }

        //If worst block found, allocate it to the block
        //and reduce the available memory in that block
        if(worstFitId != -1){
            worstFitAllocArray[i] = worstFitId;
            worstFitBlockArray[worstFitId] -= p[i];
        }
    }

    //Displays the remaining memory partition in a table format for easy visualisation purposes.
    printf("\n");
    for (i = 0; i < 10; i++){ 
        printf(" %d\t\t%d\t",(i+1),p[i]); 
        if (worstFitAllocArray[i] != -1) 
            printf("%d\t\t%d\t\t%d\n",worstFitAllocArray[i] + 1, blockArray[worstFitAllocArray[i]], worstFitBlockArray[worstFitAllocArray[i]]); 
        else
            printf("Not Allocated. This process needs to wait for a free block.\n");
    }
    printf("\n\n");
}   

// Best fit : Allocate the smallest hole that is big
// enough; must search entire list, unless ordered by
// size. (Produces the smallest leftover hole).
int computeBestFit(int p[], int pLength){

    //Declare two arrays and copy the original blockArray and allocation Array so the computation
    //will not affect the other two methods' calculation
    int bestFitAllocArray[10];
    int bestFitBlockArray[12];
    memcpy(bestFitBlockArray, blockArray, sizeof blockArray);
    memcpy(bestFitAllocArray, allocArray, sizeof allocArray);

    //Uses this variable to find the best fit block for the process it's checking
    int bestFitId;

    //For visual purpose
    printf("Best Fit Algorithm\n");
    printf("Process\t\tSize\tAssigned Block\tBlock Memory\tRemaining Memory");
    
    //Iterates each process by input sequence and find the largest block space available
    for(i=0; i<pLength; i++){
        //Uses this variable to find the best fit block for the current process
        bestFitId = -1;
        for(j=0; j<blockLength; j++){
            //Checks if the block is larger than the process size
            if(p[i]<=bestFitBlockArray[j]){
                //Assigns the process to the block if unassigned
                if(bestFitId == -1){
                    bestFitId = j;
                }
                //Otherwise, try to find smaller blocks available for the process and re-assigns
                //to that block instead for the best fit
                else if (bestFitBlockArray[bestFitId] > bestFitBlockArray[j])
                bestFitId = j;
            }
        }

        //If best fit block found, allocate it to the block
        //and reduce the available memory in that block
        if(bestFitId != -1){
            bestFitAllocArray[i] = bestFitId;
            bestFitBlockArray[bestFitId] -= p[i];
        }
    }

    //Displays the remaining memory partition in a table format for easy visualisation purposes.
    printf("\n");
    for (i = 0; i < 10; i++){ 
        printf(" %d\t\t%d\t",(i+1),p[i]); 
        if (bestFitAllocArray[i] != -1) 
            printf("%d\t\t%d\t\t%d\n",bestFitAllocArray[i] + 1, blockArray[bestFitAllocArray[i]], bestFitBlockArray[bestFitAllocArray[i]]); 
        else
            printf("Not Allocated. This process needs to wait for a free block.\n");
    }
    printf("\n\n");
}

