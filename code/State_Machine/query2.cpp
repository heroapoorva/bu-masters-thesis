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
#include <map>
#include <math.h>
#include <time.h>
#include <set>
#include <cmath> 
#include <numeric>
#include <iomanip>

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
        if(q==0)
        {
            d.push_back(vect);    
        }
    }
    vect.clear();
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
    d.push_back(vect); 
    start_time=vect[0];
    
    while(true)
    {
        fscanf (pFile, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i", &q,
        &vect[0],&vect[1],&j,&vect[2],&j,&vect[3],&vect[4],&vect[5],&j,&j,&j,&j,&j,&j);
        //printf ("%i,\n",database[i][0]);
        cur_time=vect[0];
        lines_read++;
        if(q==0)
        {
            d.push_back(vect);    
        }
        if(cur_time-start_time>=window_size or lines_read>1000000)
        {
            break;
        }
    }
    vect.clear();
}


bool my_sort(std::vector<int> &v1, std::vector<int> &v2)
{
    for(int i=0;i<v1.size();i++)
    {
        if(v1[i]!=v2[i])
        {
            return(v1[i]<v2[i]);
        }
        else
        {
            continue;
        }
    }
    return(v1[v1.size()-1]<v2[v2.size()-1]);
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
        vect[1]=d[i][5];
        vect[0]=d[i][1];
        vect[2]=d[i][2];
        vect[3]=d[i][3];
        vect[4]=d[i][4];
        temp.push_back(vect);
    }
    //temp=(carID,position,exp,dir,seg)
    std::sort(temp.begin(),temp.end(), my_sort);
    
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
    printf("Size of temp2 is, %d\n", temp2.size());
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
    printf("    Current Time is, %d\n", cur_time);
    for(i=0;i<d.size();i++)
    {
        if(d[i][0]>cur_time-300)
        {
            vect[0]=d[i][1];
            vect[1]=d[i][2];
            vect[2]=d[i][3];
            vect[3]=d[i][4];
            s.push_back(vect);
        }
    }
}

float entropy_of(std::vector<std::vector<int>> &t,int c)
{
    float entropy=0;
    std::map<int,int> count;
    
    int i;
//    printf("    Starting dictionary.\n");
    for(i=0;i<t.size();i++)
    {
        count[t[i][c]]++;
    }
    std::map<int,int>::iterator it=count.begin();
    float px;
//    printf("    Calculating entropy.\n");
    while(it!=count.end())
    {
        px = (float)it->second/t.size();
        if(px>0)
        {
            entropy -= px*log(px)/log(2);
        }
        it++;
    }
    count.clear();
    return entropy;
}

std::vector<float> entropy_calc(std::vector<std::vector<int>> &s, std::vector<std::vector<int>> &a)
{
    std::vector<float> vect;
    vect={0,0,0,0,0,0,0,0,0,0};
    int i;
    if(s.size()>0)
    {
        for(i=0;i<s[0].size();i++)
        {
            vect[i]=entropy_of(s,i);
//            printf("    Entrope of %d done.\n",i);
        }
    }    
    if(a.size()>0)
    {
        for(i=0;i<a[0].size();i++)
        {
//            printf("    Entrope of a column %d start.\n",a[0].size()+i);
            vect[s[0].size()+i]=entropy_of(a,i);
        }
    }
    return vect;
}

void state_machine(std::vector<std::vector<int>> &s, std::vector<std::vector<int>> &a, std::ofstream &op_file)
{
    //a=(position,exp,dir,seg)
    //s=(carid,exp,dir,seg)
    int i,j,k;
    int count=0;
    int c1,c2;
    int operation;
    std::vector<float> entropy;
    entropy=entropy_calc(s,a);
    for(i=0;i<entropy.size();i++)
    {
       // fprintf(op_file, "%f ", entropy[i]);
        op_file<<entropy[i]<<" ";
    }
    op_file<<std::endl;
    op_file<<s.size()<<" "<<a.size()<<std::endl;
    entropy.clear();
    
    int times;
    int temp_order[10]={0,1,2,3,4,5,6,7,8,9};
    
    std::vector<int> order;
    order.resize(10);
    srand(time(NULL));
    for(times=0;times<1;times++)
    {
        //printf("        loop %d\n",times);
        
        std::random_shuffle(temp_order, temp_order+10);
        
        for(i=0;i<order.size();i++)
        {
            order[i]=temp_order[i];
        }

        auto start = std::chrono::high_resolution_clock::now();
        for(i=0;i<s.size();i++)
        {
            for(j=0;j<a.size();j++)
            {
                c1=0;
                c2=0;
                for(k=0;k<order.size();k++)
                {
                    operation=order[k];
                    switch(operation)
                    {
                        case 0:
                            if(a[j][1]==s[i][1])
                            {
                                c1++;
                            }
                        case 1:
                            if(a[j][2]==1)
                            {
                                c1++;
                            }
                        case 2:
                            if(s[i][2]==1)
                            {
                                c1++;
                            }
                        case 3:
                            if(a[j][3]<s[i][3])
                            {
                                c1++;
                            }
                        case 4:
                            if(a[j][3]>s[i][3]-5)
                            {
                                c1++;
                            }
                        case 5:
                            if(a[j][1]==s[i][1])
                            {
                                c2++;
                            }
                        case 6:
                            if(a[j][2]==0)
                            {
                                c2++;
                            }
                        case 7:
                            if(s[i][2]==0)
                            {
                                c2++;
                            }
                        case 8:
                            if(a[j][3]>s[i][3])
                            {
                                c2++;
                            }
                        case 9:
                            if(a[j][3]<s[i][3]+5)
                            {
                                c2++;
                            } 
                    }
                    if(c1==5 or c2==5)
                    {
                        break;
                    }
                }
            }
        }
        auto stop = std::chrono::high_resolution_clock::now(); 
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        
        // Have to write entropy, order and duration.
        for(i=0;i<10;i++)
        {
            op_file<<order[i]<<" ";
//            fprintf(op_file, "%d ", order[i]);
        }
        op_file<<std::endl<<duration.count()<<std::endl;
        
        //fprintf(op_file,)
    }
}




// argc is the number of command line arguments
// argv is an array of command line arguments
int main(int argc, char** argv)
{
    //The size of window we consider.
    int window_size=262144;
    //int window_size=32768;
    int time_frame=300;
    FILE *pFile;
    // Say the first arguement is the location of the stream.
    // pFile = fopen ("test.err","r");
    pFile = fopen (argv[1],"r");
    FILE *op_file;
    op_file= fopen("q2out.txt","w");
    
    std::vector<std::vector<int>> database;
    
    std::vector<std::vector<int>> accSeg;
    std::vector<std::vector<int>> carCurSeg;
    
    //The state machine creation
    int read_times=0;
    while(!feof(pFile))
    {
        //Take input everytime
        //(Time, carID, expressway, Dir, Seg, Position)
        //input_database(pFile, database, window_size);
        input_database_timeframe(pFile, database, time_frame);
        
        
        curcarseg(database, carCurSeg);
        
        accseg(database,accSeg); 
        if(accSeg.size()>0)
        {   
            printf("The window size is %d, number of windows read = %d\n", database.size() ,read_times);
            printf("    size of carCurSeg is, %d \n", carCurSeg.size());
            printf("    size of accSeg is, %d \n", accSeg.size());
            state_machine(carCurSeg, accSeg, op_file);
            accSeg.clear();
        }        
        carCurSeg.clear();
        database.clear();
        read_times++;
        /*
        if(read_times>2)
        {
            break;
        }
        */
    }

    fclose(pFile);
    fclose(op_file);
    return 0;
}
