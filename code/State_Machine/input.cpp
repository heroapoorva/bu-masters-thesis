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

void input_toll(FILE * pFile, int d[][4], int window_size)
{
    int i;
    for(i=0;i<window_size;i++)
    {
        fscanf (pFile, "%i,%i,%i,%i", &d[i][0],&d[i][1],&d[i][2],&d[i][3]);
        //printf ("%i,\n",database[i][0]);
    }
    
}

void input_database(FILE * pFile, int d[][15], int window_size)
{
    int i;
    /*
    for(i=0;i<window_size;i++)
    {
        fscanf (pFile, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i", &d[i][0],
        &d[i][1],&d[i][2],&d[i][3],&d[i][4],&d[i][5],&d[i][6],&d[i][7],&d[i][8],
        &d[i][9],&d[i][10],&d[i][11],&d[i][12],&d[i][13],&d[i][14]);
        //printf ("%i,\n",database[i][0]);
    }*/
    
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
    int window_size=20000;
    
    FILE *pFile,*pFile2;
    // Say the first arguement is the location of the stream.
    //pFile = fopen ("test.err","r");
    pFile = fopen (argv[1],"r");
    
    pFile2 = fopen (argv[2],"r");
    
    
    int toll[window_size][4];
    int database[window_size][15];
    int i=100;
    
    //The state machine creation
    create_state_machine();
    // The first assignment is going to be slower
    while(i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        //Take input everytime
        input_toll(pFile, toll, window_size);
        
        std::cout<<"yolo";
        
        input_database(pFile2, database, window_size);

        //Extract information, features
        //format();
        // 

        //std::cout <<1<<std::endl; adds 172 microsec
        //printf("%i\n",1);adds 121 microsec
        auto stop = std::chrono::high_resolution_clock::now(); 
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << duration.count() << std::endl;
        i--;
    }
    fclose(pFile);
    fclose(pFile2);
    return 0;
}
