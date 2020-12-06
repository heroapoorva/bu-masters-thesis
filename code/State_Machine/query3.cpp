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
#include <fstream> 

void input_database(FILE * pFile, std::vector<std::vector<int>> &d,int window_size)
{
    int i,j,q;
    std::vector<int> vect;
    vect.resize(7);
    for(i=0;i<window_size;i++)
    {
        fscanf (pFile, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i", &q,
        &vect[0],&vect[1],&vect[6],&vect[2],&j,&vect[3],&vect[4],&vect[5],&j,&j,&j,&j,&j,&j);
        //printf ("%i,\n",database[i][0]);
        if(q==0)
        {
            d.push_back(vect);    
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


void curcarseg(std::vector<std::vector<int>> &d, std::vector<std::vector<int>> &s)
{
    //d=(Time, carID, expressway, Dir, Seg, Position,speed)
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


void segavgspeed(std::vector<std::vector<int>> &d, std::vector<std::vector<int>> &s)
{
    //d=(Time, carID, expressway, Dir, Seg, Position,speed)
    std::vector<std::vector<int>> temp;
    int cur_time=0;
    int i,j;
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
        if(d[i][0]>cur_time-300)
        {
            vect[0]=d[i][2];
            vect[1]=d[i][3];
            vect[2]=d[i][4];
            vect[3]=d[i][6];
            temp.push_back(vect);
        }
    }
    //temp=()
    std::sort(temp.begin(),temp.end(), my_sort);
    //(express,dir,seg, average speed)
    int count = 1;
    s.push_back(temp[0]);
    for(i=1;i<temp.size();i++)
    {
        if(temp[i][0]==temp[i-1][0] and temp[i][1]==temp[i-1][1] and temp[i][2]==temp[i-1][2])
        {
            s.back()[3]=s.back()[3]+temp[i][3];
            count++;
        }
        else
        {
            s.back()[3]=s.back()[3]/(count);
            s.push_back(temp[i]);
            count = 1;
        }
    }
    s.back()[3]=s.back()[3]/count;
    /*for(i=0;i<s.size();i++)
    {
        for(j=0;j<s[i].size();j++)
        {
            printf("%d ",s[i][j]);
        }
        printf("\n");
    }*/
    temp.clear();
}

bool my_sort2(std::vector<int> &v1, std::vector<int> &v2)
{
    int i;
    for(i=1;i<v1.size();i++)
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
    return(v1[0]<v2[0]);
}


void segvol(std::vector<std::vector<int>> &d, std::vector<std::vector<int>> &s)
{
    //d=(carid,exp,dir,seg)
    std::sort(d.begin(),d.end(), my_sort2);
    int i,j;
    s.push_back(d[0]);
    s.back()[0]=1;
    for(i=1;i<d.size();i++)
    {
        if(d[i][3]==d[i-1][3] and d[i][1]==d[i-1][1] and d[i][2]==d[i-1][2])
        {
            s.back()[0]++;
        }
        else
        {
            s.push_back(d[i]);
            s.back()[0]=1;
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
    vect.resize(s[0].size()+a[0].size());
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

void my_swap(std::vector<int> &d, int a, int b)
{
    int temp;
    temp= d[a];
    d[a]=d[b];
    d[b]=temp;
}

void my_reverse(std::vector<int> &d, int a)
{
    int start,end;
    start=a+1;
    end=d.size()-1;
    while(start>=end)
    {
        my_swap(d,start,end);
        start++;
        end--;
    }
}

void next_perm(std::vector<int> &d)
{
    int i,bp,target;
    bp=-1;
    for(i=d.size()-1;i>0;i--)
    {
        if(d[i]>d[i-1])
        {
            bp=i-1;
            break;
        }
    }
    if(bp!=-1)
    {
        for(i=d.size()-1;i>bp;i--)
        {
            if(d[i]>d[bp])
            {
                my_swap(d,i,bp);
                break;
            }
        }
        my_reverse(d,bp);
    }
}

void segtoll(std::vector<std::vector<int>> &s, std::vector<std::vector<int>> &v, std::ofstream &op_file)
{       
    int c1,r1,i,j,k,operation;
    std::vector<float> entropy;
    entropy=entropy_calc(s,v);
    
   
    for(i=0;i<entropy.size();i++)
    {
       // fprintf(op_file, "%f ", entropy[i]);
        op_file<<entropy[i]<<" ";
    }
    op_file<<std::endl;
    op_file<<s.size()<<" "<<v.size()<<std::endl;
    entropy.clear();
    //v=(count of cars,exp,dir,seg)
    //s=(express,dir,seg, average speed)
    int times;
    std::vector<int> order;
    order.push_back(0);
    order.push_back(1);
    order.push_back(2);
    order.push_back(3);

    int max_times=24;

    for(times=0;times<max_times;times++)
    {
        
        auto start = std::chrono::high_resolution_clock::now();
        for(i=0;i<s.size();i++)
        {
            for(j=0;j<v.size();j++)
            {
                for(k=0;k<4;k++)
                {
                    operation=order[k];
                    switch(operation)
                    {
                        case 0:
                            if(s[i][0]!=v[j][1])
                            {
                                break;
                            }
                        case 1:
                            if(s[i][1]!=v[j][2])
                            {
                                break;
                            }
                        case 2:
                            if(s[i][2]!=v[j][3])
                            {
                                break;
                            }
                        case 3:
                            if(s[i][3]>=40)
                            {
                                break;
                            }
                    }
                }
            }
        }
        auto stop = std::chrono::high_resolution_clock::now(); 
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        
        
        for(i=0;i<4;i++)
        {
            op_file<<order[i]<<" ";
//            fprintf(op_file, "%d ", order[i]);
        }
        op_file<<std::endl<<duration.count()<<std::endl;
//        fprintf(op_file, "\n");
//        fprintf(op_file, "%d", duration.count());
//        fprintf(op_file, "\n");

        next_perm(order);
    }
}

int main(int argc, char** argv)
{
    int window_size=10000;
    
    FILE *pFile;
    pFile = fopen (argv[1],"r");
    
    std::ofstream op_file;
    op_file.open("q3out.txt");
    
    std::vector<std::vector<int>> database;
    std::vector<std::vector<int>> curCarSeg;
    std::vector<std::vector<int>> segAvgSpeed;
    std::vector<std::vector<int>> segVol;
    
    int read_times=0;
    while(!feof(pFile))
    {
        
        input_database(pFile, database, window_size);
        read_times++;
        printf("read times is, %d\n", read_times);
        curcarseg(database, curCarSeg);
        printf("    CurCarSeg size is, %d\n", curCarSeg.size());
        segavgspeed(database,segAvgSpeed);
        printf("    SegAvgSpeed size is, %d\n", segAvgSpeed.size());
        segvol(curCarSeg,segVol);
        printf("    SegVol size is, %d\n", segVol.size());
        segtoll(segAvgSpeed,segVol,op_file);
        
        curCarSeg.clear();
        segAvgSpeed.clear();
        segVol.clear();
        database.clear();
        /*
        if(read_times>3)
        {
            break;
        }
        */
    }
    fclose(pFile);
    op_file.close();
    return 0;
}
