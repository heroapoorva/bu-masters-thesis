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
#include <algorithm>
// argc is the number of command line arguments
// argv is an array of command line arguments

void input_database(FILE * pFile, int d[][4],int window_size)
{
    int i,j;
    for(i=0;i<window_size;i++)
    {
        fscanf (pFile, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i", &j,
        &j,&j,&d[i][0],&d[i][1],&j,&d[i][2],&d[i][3],&j,&j,&j,&j,&j,&j,&j);
        //printf ("%i,\n",database[i][0]);
    }
}

void create_state_machine()
{
    int i = 0;
}

bool my_sort(std::vector<int> &v1, std::vector<int> &v2)
{
    int i;
    for(i=0;i<v1.size();i++)
    {
        if(v1[i]>v2[i])
        {
            return(v1[i]>v2[i]);
        }
        else if(v1[i]<v2[i])
        {
            return(v1[i]>v2[i]);
        }
        else
        {
            continue;
        }
    }
    return(v1[i]>v2[i]);
}

void print_2dvector(std::vector<std::vector<int>> output)
{
    int i,j;
    printf("%ld \n",output.size());
    for(i=0;i<output.size();i++)
    {
        for(j=0;j<output[i].size();j++)
        {
            printf("%d ",output[i][j]);
        }
        printf("\n");
    }
}


void format(int d[][4],std::vector<std::vector<int>> &output, std::vector<int> &vect, int window_size)
{
    int i = 0,j = 0;
    int count;
    std::vector<std::vector<int>> op;
    for(i;i<window_size;i++)
    {
        if(d[i][2]==0)
        {
            vect.push_back(d[i][1]);
            vect.push_back(d[i][2]);
            vect.push_back(d[i][3]);
            vect.push_back(d[i][0]);
            output.push_back(vect);
        }
        vect.clear();
    }

    if(output.size()>0)
    {
        count = 1;
        std::sort(output.begin(),output.end(), my_sort);
        //print_2dvector(output);
        op.push_back(output[0]);
        for(i=1;i<output.size();i++)
        {
            //printf("%d %d\n",op.back()[3], count);
            if((output[i][0]==output[i-count][0])&&(output[i][1]==output[i-count][1])&&(output[i][2]==output[i-count][2]))
            {
                op.back()[3]=op.back()[3]+output[i][3];
                count++;
            }
            else
            {
                op.back()[3]=op.back()[3]/(count);
                op.push_back(output[i]);
                count = 1;
            }
            //printf("%d %d\n\n",op.back()[3], count);
        }
        op.back()[3]=op.back()[3]/count;
        output.clear();
        output=op;
        op.clear();
        //print_2dvector(output);
    }
}

int main(int argc, char** argv)
{
    //The size of window we consider.
    int window_size=20000;
    
    FILE *pFile;
    // Say the first arguement is the location of the stream.
    // pFile = fopen ("test.err","r");
    pFile = fopen (argv[1],"r");
    
    int database[window_size][4];
    
    std::vector<int> vect;
    vect.clear();
    std::vector<std::vector<int>> output;
    
    //The state machine creation
    auto start = std::chrono::high_resolution_clock::now();
    while(!feof(pFile))
    {
        
        //Take input everytime
        input_database(pFile, database, window_size);
        
        //Extract information, features
        format(database, output, vect, window_size);
        // 
        
        output.clear();
        vect.clear();
        break;
    }
    auto stop = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << std::endl;
    fclose(pFile);
    return 0;
}
