#include<stdio.h>
#include <stdlib.h>

int main()
{
    int arr[10]={20,3,4,5,6,7,8,9,10,11};
    int sum=0;
    for (int i = 0; i < 10; i++)
    {
        sum+=arr[i];
    }
    double probli[10];
    for (int i = 0; i < 10; i++)
    {
        probli[i]=1.0*arr[i]/sum;
        printf("%f ", probli[i]);
    }
    int n=0,time[10]={0};
    double ran;
    while(n<100000)
    {
        ran=1.0*rand()/RAND_MAX;
        for(int k=0;k<10;k++)
        {
            ran-=probli[k];
            if(ran<=0||k==9){
                time[k]+=1;
                break;
            }  
        }
        n++;
    }
    printf("\n");
    for(int i=0;i<10;i++) printf("%f ",1.0*time[i]/100000);
    
}