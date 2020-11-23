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

void input_database(FILE * pFile, std::vector<std::vector<int>> &d,int window_size)
{
    int i,j,q;
    std::vector<int> vect;
    vect.resize(6);
    for(i=0;i<window_size;i++)
    {
        fscanf (pFile, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i", &q,
        &vect[0],&vect[1],&j,&vect[2],&j,&vect[3],&vect[4],&vect[5],&j,&j,&j,&j,&j,&j);
        //printf ("%i,\n",database[i][0]);
        vect[4]=vect[5]/52800;
        if(q==0)
        {
            d.push_back(vect);    
        }
    }
}

void input_database_timeframe(FILE * pFile, std::vector<std::vector<int>> &d,int window_size)
{
    int i,j,q;
    std::vector<int> vect;
    vect.resize(6);
    int start_time=0;
    int cur_time=0;
    int lines_read=0;
    fscanf (pFile, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i", &q,&vect[0],
    &vect[1],&j,&vect[2],&j,&vect[3],&vect[4],&vect[5],&j,&j,&j,&j,&j,&j);
    vect[4]=vect[5]/52800;
    d.push_back(vect); 
    start_time=vect[0];
    
    while(true)
    {
        fscanf (pFile, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i", &q,
        &vect[0],&vect[1],&j,&vect[2],&j,&vect[3],&vect[4],&vect[5],&j,&j,&j,&j,&j,&j);
        //printf ("%i,\n",database[i][0]);
        vect[4]=vect[5]/52800;
        cur_time=vect[0];
        lines_read++;
        if(q==0)
        {
            d.push_back(vect);    
        }
        if(cur_time-start_time>=window_size or lines_read>300000)
        {
            break;
        }
    }
}


bool my_sort(std::vector<int> &v1, std::vector<int> &v2)
{
    int i = 0;
    for(i;i<v1.size();i++)
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
    return(v1[0]<v2[0]);
}

void print_1dvector(std::vector<int> t)
{
    int i;
    for(i=0;i<t.size();i++)
    {
        printf("%d ",t[i]);
    }
}
void print_2dvector(std::vector<std::vector<int>> t, int l)
{
    int i;
    int lines;
    if(t.size()<l)
    {
        lines=t.size();
    }
    else
    {
        lines=l;
    }
    for(i=0;i<l;i++)
    {
        print_1dvector(t[i]);
        printf("\n");
    }
}


void accseg(std::vector<std::vector<int>> &d, std::vector<std::vector<int>> &a)
{
    //d=(Time, carID, expressway, Dir, Seg, Position)
    int i;
    std::vector<int> vect;
    vect.resize(5);
    std::vector<std::vector<int>> temp;
    for(i=0;i<d.size();i++)
    {
        vect[0]=d[i][1];
        vect[1]=d[i][5];
        vect[2]=d[i][2];
        vect[3]=d[i][3];
        vect[4]=d[i][4];
        temp.push_back(vect);
    }
    //temp=(carID,position,exp,dir,seg)
    std::sort(temp.begin(),temp.end(), my_sort);
    //printf("size of temp is, %d \n", temp.size());
    std::vector<std::vector<int>> temp2;
    int count=1;
    vect.resize(4);
    
    for(i=1;i<temp.size();i++)
    {
        if(temp[i][0]==temp[i-1][0] and temp[i][1]==temp[i-1][1])
        //if(temp[i][0]==temp[i-1][0])
        {
            count++;
        }
        else
        {
            count=1;
        }
        if(count==4)
        {
            vect[0]=temp[i][1];
            vect[1]=temp[i][2];
            vect[2]=temp[i][3];
            vect[3]=temp[i][4];
            temp2.push_back(vect);
        }
    }
    temp.clear();
    std::sort(temp2.begin(),temp2.end(), my_sort);
    
    
    if(temp2.size()>0)
    {
        
        a.push_back(temp2[0]);    
        
        int j;
        int add;
        for(i=1;i<temp2.size();i++)
        {
            add=0;
            for(j=0;j<temp2[i].size();j++)
            {
                if(temp2[i][j]!=temp2[i-1][j])
                {
                    add=1;
                    break;
                }
            }
            a.push_back(temp2[i]);
        }
        temp2.clear();
    }
}

void curcarseg(std::vector<std::vector<int>> &d, std::vector<std::vector<int>> &s)
{
    int cur_time=0;
    int i;
    std::vector<int> vect;
    vect.resize(4);
    for(i=0;i<d.size();i++)
    {
        if(cur_time<d[i][0])
        {
            cur_time=d[i][0];
        }
    }
    for(i=0;i<d.size();i++)
    {
        if(d[i][0]>cur_time-30)
        {
            vect[0]=d[i][1];
            vect[1]=d[i][2];
            vect[2]=d[i][3];
            vect[3]=d[i][4];
            s.push_back(vect);
        }
    }
}

void state_machine(std::vector<std::vector<int>> &s, std::vector<std::vector<int>> &a)
{
    //a=(position,exp,dir,seg)
    //s=(carid,exp,dir,seg)
    int i,j;
    int count=0;
    
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> order;
    order.resize(10);
    for(i=0;i<a.size();i++)
    {
        for(j=0;j<s.size();j++)
        {
            if(
            ((a[i][1]==s[j][1])and(a[i][2]==1)and(s[i][2]==1)and(s[i][3]<a[i][3])and(s[i][3]>a[i][3]-5))
            or
            ((a[i][1]==s[j][1])and(a[i][2]==0)and(s[i][2]==0)and(s[i][3]>a[i][3])and(s[i][3]<a[i][3]+5))
            )
            {
                count++;
            }
        }
    }
    auto stop = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    printf("%d \n",duration.count());
//    std::cout << duration.count() << std::endl;
    printf("COUNT IS %d\n", count);
}

// argc is the number of command line arguments
// argv is an array of command line arguments
int main(int argc, char** argv)
{
    //The size of window we consider.
    int window_size=262144;
    int time_frame=30;
    FILE *pFile;
    // Say the first arguement is the location of the stream.
    // pFile = fopen ("test.err","r");
    pFile = fopen (argv[1],"r");
    
    std::vector<std::vector<int>> database;
    
    std::vector<int> vect;
    
    std::vector<std::vector<int>> accSeg;
    std::vector<std::vector<int>> carCurSeg;
    
    //The state machine creation
    int read_times=0;
    while(!feof(pFile))
    {
        //Take input everytime
        //(Time, carID, expressway, Dir, Seg, Position)
        input_database(pFile, database, window_size);
        //input_database_timeframe(pFile, database, time_frame);
        //
        
        printf("The window size is %d, number of windows read = %d\n", database.size() ,read_times);
        curcarseg(database, carCurSeg);
        //
        printf("    size of carCurSeg is, %d \n", carCurSeg.size());
        accseg(database,accSeg); 
        printf("    size of accSeg is, %d \n", accSeg.size());
          
        state_machine(carCurSeg, accSeg);
                
        accSeg.clear();
        carCurSeg.clear();
        database.clear();
        read_times++;
    }

    fclose(pFile);
    return 0;
}
