/*
Take input
    determine input rate
        fread()
        fscanf()
        fprintf()
        fopen()
    determine storage method
        vectors vs array?
    extract information/ features
        entropy, labeled values
Model the state machine
    Depending on query, simply. OR push selections or projections.
    A simple NDFA does the job. 
    At any point keep track of possible options or executable operator.
    Automata using the parallelizable operators.
    Start and End state
Generate randomness and record time of runs
    #include <time.h>   or compile time recording
    #include <stdlib.h> 
    Data available via linear road
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <chrono> 
// argc is the number of command line arguments
// argv is an array of command line arguments

void input(FILE * pFile, int database[][4], int window_size)
{
    int i;
    for(i=0;i<window_size;i++)
    {
        fscanf (pFile, "%i,%i,%i,%i", &database[i][0],&database[i][1],&database[i][2],&database[i][3]);
        //printf ("%i,\n",database[i][0]);
    }
    
}

void create_state_machine()
{
    int i = 0;
}

void format()
{
    int i = 0;
}

int main(int argc, char** argv)
{
    //The size of window we consider.
    int window_size=100;
    
    FILE * pFile;
    // Say the first arguement is the location of the stream.
    //pFile = fopen ("test.err","r");
    pFile = fopen (argv[1],"r");
    
    int database[window_size][4];
    int i=1;
    
    //The state machine creation
    create_state_machine();
    // The first assignment is going to be slower
    while(i)
    {
        
        auto start = std::chrono::high_resolution_clock::now();
        //Take input everytime
        input(pFile, database, window_size);
        
        //Extract information, features
        format();
        // 
        
        //std::cout <<1<<std::endl; adds 172 microsec
        //printf("%i\n",1);adds 121 microsec
        auto stop = std::chrono::high_resolution_clock::now(); 
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << duration.count() << std::endl;
        i=0;
    }  
    fclose(pFile);
    return 0;
}
