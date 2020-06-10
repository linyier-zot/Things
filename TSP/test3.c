#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
    printf("calc sum\n");
    int arr[48],sum=0;
    for(int i=0;i<48;i++){
        scanf("%d",&arr[i]);
        sum+=arr[i];
    }
    printf("\nsum: %d:",sum);

}