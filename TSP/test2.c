#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM 15
#define INDIV_NUM 20
int a[NUM]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},b[NUM]={15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
int child[NUM];
int main(){
    for(int kk=0;kk<1000;kk++){
            int sum=0;
            for(int j=0;j<NUM;j++) child[j]=a[j];
            int ran=rand()%8,save[3],lose[3];  //0-45
            for(int j=0;j<3;j++){
                save[j]=child[ran+j];
                child[ran+j]=b[ran+j];
                lose[j]=b[ran+j];
            }
            for(int j=0;j<NUM;j++){       //修正
                if(child[j]==lose[0]&&(j<ran||j>(ran+2))) child[j]=save[0];
                if(child[j]==lose[1]&&(j<ran||j>(ran+2))) child[j]=save[1];
                if(child[j]==lose[2]&&(j<ran||j>(ran+2))) child[j]=save[2];
            }
            for(int i=0;i<NUM;i++) {printf("%d ",child[i]);sum+=child[i];};

            printf(" sum: %d\n",sum);

            }
}