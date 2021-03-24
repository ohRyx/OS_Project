#include <stdio.h>
#include <stdlib.h>
#define minResA 14
#define minResB 11
#define minResC 14
#define minResD 18

/*Allocation Matrix*/
int allocation[6][4]= { {2,1,3,3},
                        {2,3,1,2},
                        {3,3,3,1},
                        {2,1,3,4},
                        {3,2,2,5},
                        {2,1,2,3}};

/*Max Matrix*/
int max[6][4]=      {   {7,3,4,5},
                        {8,6,2,4},
                        {9,5,5,6},
                        {6,4,6,6},
                        {8,3,2,6},
                        {8,3,2,3}};

/*Need Matrix*/
int need[6][4]=     {   {0,0,0,0},
                        {0,0,0,0},
                        {0,0,0,0},
                        {0,0,0,0},
                        {0,0,0,0},
                        {0,0,0,0}};

/*Available Matrix*/
int available[4]=       {0,0,0,0};     

/*Sequence Variable: To record possible execution sequence*/
int safesequence[6] =   {0,0,0,0,0,0};  //Safe sequence matrix records the sequence of process completion
int sequence = 0;                       //Sequence variable is a counter that helps with sequencing the sequence arrays

/*Unsafe process matrix: To record processes that are incomplete*/
int unsafeprocess[6] = {1,2,3,4,5,6};  //Unsafe process matrix checks which processes are incomplete

/*Initialize function*/
int Calculation();          //Function to check between Need and Availability, and adjust the array accordingly
int checkstate(int safe);   //Function to check the state of the system 
void printmatrix();         //Function to print all the matrix
int validateInput(int input, int min);

int main(){
    
    /*Variable for user to input resources*/
    int resourcea = 0;
    int resourceb = 0;
    int resourcec = 0;
    int resourced = 0; 

    /*Variable for sum of allocation*/
    int allocatedA = 0;
    int allocatedB = 0;
    int allocatedC = 0;
    int allocatedD = 0;
    
    /*State of the system                                   */
    /*Safe = 0: System is unsafe, terminate                 */
    /*Safe = 1: System is safe, processes are incomplete    */
    /*Safe = 2: System is safe, processes are completed     */
    int safe = 1;
    

    /*Temporary variable*/
    int i = 0;
    int j = 0;

    /*Let user know the minimum input*/
    printf("Type A needs to be larger than %d\n", minResA);
    printf("Type B needs to be larger than %d\n", minResB);
    printf("Type C needs to be larger than %d\n", minResC);
    printf("Type D needs to be larger than %d\n", minResD);
    printf("\n");


    /* Get user input*/
    printf("\nEnter resource A(Must be larger than %d)\n", minResA);
    scanf("%d", &resourcea);
    validateInput(resourcea, minResA);

    printf("\nEnter resource B(Must be larger than %d)\n", minResB);
    scanf("%d", &resourceb);
    validateInput(resourceb, minResB);

    printf("\nEnter resource C(Must be larger than %d)\n", minResC);
    scanf("%d", &resourcec);
    validateInput(resourcec, minResC);

    printf("\nEnter resource D(Must be larger than %d)\n", minResD);
    scanf("%d", &resourced);
    validateInput(resourced, minResD);

    /*Caculating Need matrix    */ 
    /*Need = Max - Allocation   */
    for (i=0;i<6;i++){
        for (j=0;j<6;j++){
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    
    /*Calculate the sum of allocation for each resource*/
    for (i=0;i<6;i++){
        allocatedA += allocation[i][0];
    }
    for (i=0;i<6;i++){
        allocatedB += allocation[i][1];
    }
    for (i=0;i<6;i++){
        allocatedC += allocation[i][2];
    }
    for (i=0;i<6;i++){
        allocatedD += allocation[i][3];
    }

    /*Checking Availability                                                     */
    /*Available = Total amount of resource - Total amount of allocated resource */
    available[0]= resourcea - allocatedA;
    available[1]= resourceb - allocatedB;
    available[2]= resourcec - allocatedC;
    available[3]= resourced - allocatedD;

    /*Print the initial Matrix*/
    printmatrix();

    do
    {   
        safe = Calculation();       //Function to checks if available resources satisfy needs
        safe = checkstate(safe);    //Function to check system state
    }   while (safe==1);            //Loop while safe == 1, when the system is safe but has not completed all processes

    /*Print unsafe state*/
    if (safe == 0){                 //safe == 0, System is in an unsafe state
        printf("\nUnsafe state\n");
        printmatrix();
        printf("\nUnsafe state\n");
        
        /*Print out processes that still required resources*/
        printf("Processes that need resource:\n");
        for (i = 0; i < 6; i++)
        {
            if (unsafeprocess[i]>0){
                printf("P%d, ",unsafeprocess[i]);
            }    
        }
        printf("does not have enough resource.");   
    }

    /*Print safe state*/
    if (safe == 2){                 //safe == 2, System is in an unsafe state
        printf("\nSafe state\n");
        
        /*Print out possible sequence*/
        printf("Possible sequence:\n");
        for (i = 0; i < 5; i++)
        {
            printf("P%d, ",safesequence[i]);
        }   
        printf("P%d ",safesequence[5]);
    }
}

int Calculation(){
    /*
    Purpose of calculation function:
    1. Checks between Need and Available resources
    1.1 Should Need be [0,0,0,0] process is completed, pass to 1.2.
    1.2 Else check if there's enough resources to satisfy need: check = need - availability
    1.3 If there's no change return 0 (unsafe state), the system is still running but does not have enough resources to satisfy need.
    
    2. Adjust array, if 1.2.
    2.1 Available array = Available array + Allocation. The system finished the process, process is returning the resources.
    2.2 Allocation = [0,0,0,0]. The system has finished the process. Therefore allocation to 0.
    2.3 Need = [0,0,0,0].The system has finished the process. Therefore Need to 0.
    */

    /*Variable to check if there's enough resource to satisfy need, for resource A, resource B ,resource C, resource D respectively*/
    int check1 = 0;
    int check2 = 0;
    int check3 = 0;
    int check4 = 0;

    /*Temporary variable*/
    int i = 0;
    int j = 0;

    for (i=0;i<6;i++)                                                               //Loop to check through 6 processes     
        {
        if (need[i][0]==0 && need[i][1]==0 && need[i][2]==0 && need[i][3]==0)       //1.1 Check is process is completed 
            {
            int pass = 0; //line to pass
            }
        else{                                                                        
            check1 = need[i][0] - available[0];                                     //1.2 Check if available resource A satisfy need
            check2 = need[i][1] - available[1];                                     //1.2 Check if available resource B satisfy need
            check3 = need[i][2] - available[2];                                     //1.2 Check if available resource C satisfy need
            check4 = need[i][3] - available[3];                                     //1.2 Check if available resource D satisfy need
            if (check1<1 && check2<1 && check3<1 && check4<1){                      //1.2 Check if all resources satisfy need
                
                                                                                    //2. Adjusting array accordingly                                                                    
                available[0] = available[0] + allocation[i][0];                     //2.1 New available resource A = old available + process allocation 
                available[1] = available[1] + allocation[i][1];                     //2.1 New available resource B = old available + process allocation
                available[2] = available[2] + allocation[i][2];                     //2.1 New available resource C = old available + process allocation
                available[3] = available[3] + allocation[i][3];                     //2.1 New available resource D = old available + process allocation

                allocation[i][0] = 0;                                               //2.2 Change Resource A allocated to the current processes to 0            
                allocation[i][1] = 0;                                               //2.2 Change Resource B allocated to the current processes to 0
                allocation[i][2] = 0;                                               //2.2 Change Resource C allocated to the current processes to 0
                allocation[i][3] = 0;                                               //2.2 Change Resource D allocated to the current processes to 0

                need[i][0] = 0;                                                     //2.3 Change Need A for the current processes to 0
                need[i][1] = 0;                                                     //2.3 Change Need B for the current processes to 0
                need[i][2] = 0;                                                     //2.3 Change Need C for the current processes to 0
                need[i][3] = 0;                                                     //2.3 Change Need D for the current processes to 0


                safesequence[sequence] = i+1;                                       // Save the current process execution in the sequence matrix
                sequence++;                                                         // Ensure that the next process will be saved in the next element
                unsafeprocess[i]= 0;                                                // Change current process to completed

                printmatrix();                                                      // Print Allocation, Need, Available, Max matrix                                                          
                
                return 1;                                                           // There was a change, return 1 (system is currently in safe state)
                }
            }              
        }
    return 0;                                                                       //1.3 There was no change, return 0 (Unsafe state)
}

int checkstate(int safe){
    /*
    Purpose of the function:
    1. Check if the system has completed all processes
    
    2. Check the system state
    2.1 Check if the system is in a unsafe state
    2.2 Check if the system is in a safe state, and has not finished all processes 
    2.3 Check if the system is in a safe state, and has finished all processes
    */

    /*Temporary variable*/
    int i = 0;
    int j = 0;
    
    /*Variable for checking completion of processes*/
    int sum = 0;

    for (i=0;i<6;i++){                           //1. Checks if the system has completed all processes
        for (j=0;j<4;j++)                        //Loops through the need matrix, to check if any processes are not complete
        {                                               //If processes are not complete sum > 0
            sum = sum + need[i][j];                     //If processes are complete sum == 0 
        }
    }

    if (safe == 0){                                     //2.1 Checks if the system is in unsafe state           
        return 0;
    }

    if (sum > 1){                                       //2.2 Checks if the system is in safe state, but has not finished all processes
        return 1;
    }

    if(sum == 0){                                       //2.3 Checks if the system is in safe state, and has finished all processes
        return 2;
    }

    return 1;
}

void printmatrix()
{
    /*
    Purpose of function:
    1. Print out all the matrixes
    1.1 Print out allocation matrix
    1.2 Print out need matrix
    1.3 Print out max matrix
    1.4 Print out available matrix
    */
    int i = 0;
    int j = 0;
    
    printf("\n");
    printf("Allocation    Need          Max          Available\n");
    
    for (i=0;i<6;i++){                              //1.1 Print out allocation matrix
        for (j=0;j<4;j++){
            printf("%d ", allocation[i][j]);
            }
        printf("      ");
        
        for (j=0;j<4;j++){                          //1.2 Print out need matrix
            printf("%d ", need[i][j]);
            }
        printf("      ");

        for (j=0;j<4;j++){                          //1.3 Print out max matrix
            printf("%d ", max[i][j]);
            }
        printf("     ");

        if(i==0){                                   //1.4 Print out available matrix
            for (j=0;j<4;j++){
                printf("%d ", available[j]);
                }
        }          

        printf("\n");
    }
}

int validateInput(int input, int min)
{   
    /*
        Purpose of this function,
        Is to validate if user input a number larger than the minimum requirement
    */

   // A While loop that will keep looping until user enter the correct requirement
    while (1)
    {   
        // Check if user enter a number larger than the minimum requirement
        if (min + 1 > input)
        {
            printf("\nPlease enter a number larger than %d\n", min);
            scanf("%d", &input);
        }
        else
        {
            // Return input after validation and break off from the loop
            return input;
            break;
        }

    }
}
