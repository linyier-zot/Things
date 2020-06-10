#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM 48

struct node
{
    int x;
    int y;
}points[NUM];

int arrive[NUM]={0};
int way[NUM];
int bestway[NUM];
double matrix[NUM][NUM];

void init();
void assess();
double length(int*loc_way);


int main()
{
    init();
    int now = 0;    //设置第一个出发点
    way[0]=now;
    arrive[now]=1;

    for(int i=1;i<NUM;i++)  //总共需要NUM-1步(第一步已确认)
    {
        int min = 99999999;             //贪心求解
        int min_num;
        for(int j=0;j<NUM;j++)
        {
            if(arrive[j]) continue;    //已到达
            if(matrix[now][j]<min)
            {
                min_num=j;
                min = matrix[now][j];
            }
        }
        now = min_num; //设置now为最近的城市
        way[i]=now;
        arrive[now]=1;
    }

    assess();
    return 0;
}

/*初始化*/
void init()
{
    FILE *data;
    data = fopen(".\\data.txt", "r");   /*初始化点集*/
    for(int i=0;i<NUM;i++)
    {   
        fscanf(data,"%*d%d%d",&points[i].x,&points[i].y);
    }
    fclose(data);

    for(int i=0;i<NUM;i++)                  /*初始化边集*/
    {
        for(int j=0;j<NUM;j++)
        {
            int x_d = abs(points[i].x - points[j].x);
            int y_d = abs(points[i].y - points[j].y);
            matrix[i][j]=sqrt(x_d*x_d+y_d*y_d);
        }
    }
    return;
}

/*评估*/
void assess()
{
    FILE *file=fopen(".\\bestway.txt","r");
    printf("\nbest way:  \t");
    for (int i = 0; i < NUM; i++){
        fscanf(file,"%d", &bestway[i]);
        printf("%d ",bestway[i]);
    }
    fclose(file);

    double best=matrix[bestway[0]-1][bestway[NUM-1]-1];
    printf("\nmy way:  \t");
    for(int i=0;i<NUM;i++)  printf("%d ",way[i]);
    for (int i = 0; i < NUM-1; i++) best+=matrix[bestway[i]-1][bestway[i+1]-1];

    printf("\nbest: %f\nmine: %f\n", best,length(way));
    return;
}

/*输入路径，输出长度*/
double length(int*loc_way)
{
    double len=matrix[loc_way[0]][loc_way[NUM-1]];
    for (int i = 0; i < NUM-1; i++) len+=matrix[loc_way[i]][loc_way[i+1]];
    return len;
}