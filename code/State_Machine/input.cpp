/*
Take input
    determine input rate
        fread()
        fscanf()
        fprintf()
        fopen()
    determine storage method
        
    extract information/ features
        entropy, labeled values
Model the state machine
    Depending on query, simply. OR push selections or projections.
    At any point keep track of possible options or executable operator.
    Automata using the parallelizable operators.
    Start and End state
Generate randomness and record time of runs
    #include <time.h>   or compile time recording
    #include <stdlib.h> 
    Data available via linear road
*/

#include <stdio.h>

// argc is the number of command line arguments
// argv is an array of command line arguments
int main(int argc, char** argv)
{
    FILE * pFile;
    // Say the first arguement is the location of the stream.
    pFile = fopen (argv[0],"r");
    
    char str [80];
    
}
