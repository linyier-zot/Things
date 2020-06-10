#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM 48
#define ANT_NUM 48
#define N_C 20
#define MSG_INIT 1.0
#define ALPHA 1
#define BETA 4
#define RHO 0.5
#define Q 50
struct node
{
    int x;
    int y;
}points[NUM];               //点集
struct ant
{
    int now_in;
    int ant_way[NUM];   //路
    double length;      //路长
}ants[ANT_NUM];

int next;
int way[NUM];               //搜索到的路径
int bestway[NUM];           //最优路径
double matrix[NUM][NUM];    //边值矩阵
double msg[NUM][NUM];       //信息素矩阵
//double msg_change[NUM][NUM];    //蚂蚁留下信息矩阵
int ban_list[ANT_NUM][NUM];     //禁忌表
int test=0;
int test2=0;
double min = 9999999;

void init();
void assess();
void init_ants();//初始化蚂蚁
void msg_update();  //信息素更新
void move_next(int ants_index,int step_index);  //move
void this_result();
void debug(int a,int b);
double length(int*loc_way);

int main()
{
    init();     //初始化点集、边矩阵、信息素
//    debug(NUM,NUM);
    for(int N=0;N<N_C;N++)   //小于迭代次数
    {
        init_ants();   //初始化蚂蚁群及禁忌表

        for(int i=0;i<NUM-1;i++)  //NUM-1步
        {
            for(int j=0;j<ANT_NUM;j++)
            {
                move_next(j,i);
            }
        }


//        debug(ANT_NUM,NUM);

        msg_update();

        this_result();
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

    for(int i=0;i<NUM;i++)                  /*初始化边集、信息素矩阵*/
    {
        for(int j=0;j<NUM;j++)
        {
            int x_d = abs(points[i].x - points[j].x);
            int y_d = abs(points[i].y - points[j].y);
            matrix[i][j]=sqrt(x_d*x_d+y_d*y_d);
            msg[i][j]=MSG_INIT;
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

/*初始化蚂蚁群*/
void init_ants()
{
    srand(1122);
    for(int i=0;i<ANT_NUM;i++)
    {
        for(int j=0;j<NUM;j++)
        {
            ban_list[i][j]=0;   //清空禁忌表
            ants[i].ant_way[j]=0;//重置储存的路径
        }
        int temp = rand()%NUM;  //出生的城市
//        int temp = 0;  //出生的城市
        ants[i].now_in = temp;
        ants[i].ant_way[0] = temp;
        ban_list[i][temp]=1;
        ants[i].length=0;
    }
}

/*move*/
void move_next(int ants_index,int step_index)
{
    int now = ants[ants_index].now_in;
    double change[NUM]={0},sum=0;
    for(int i=0;i<NUM;i++){
        if (!ban_list[ants_index][i]){
            change[i]=pow(msg[now][i],ALPHA)*pow(1000./matrix[now][i],BETA);
            sum+=change[i];
        }
    }

    for(int i=0;i<NUM;i++){
        change[i]=change[i]/sum;
    }
    double possible = rand()*1.0/RAND_MAX;

    if(test2){
        printf("//%f//",sum);
        for(int i=0;i<NUM;i++)
        {
            printf("%f  ",change[i]);
        }
        test2=0;
    } 

    for(int k=0;k<NUM;k++){
                        if(!ban_list[ants_index][k]){
                            possible -= change[k];
                            if(possible <= 0 || k==NUM-1){
                                next = k;
                                break;
                            }
                        }
                    }                
    if(test) printf("\n%d\n",next),printf("%f",possible),test--;
    ants[ants_index].ant_way[step_index+1]=next;
    ants[ants_index].length+=matrix[now][next];
    ants[ants_index].now_in=next;
    ban_list[ants_index][next]=1;
}

/*更新信息素*/
void msg_update()
{
    for(int i=0;i<NUM;i++){
        for(int j=0;j<NUM;j++) msg[i][j]*=RHO;
    }
    for(int i=0;i<ANT_NUM;i++){
        for(int j=0;j<NUM-1;j++){
            int a=ants[i].ant_way[j],b=ants[i].ant_way[j+1];
            msg[a][b]+=Q/matrix[a][b];
        }
        int a=ants[i].ant_way[0],b=ants[i].ant_way[NUM-1];
        msg[a][b]+=Q/matrix[a][b];
        ants[i].length+=matrix[a][b];
    }
}

/*最优结果*/
void this_result()
{
    int index=0;
    double sum=0;
    for(int i=1;i<ANT_NUM;i++)
    {
        if(ants[i].length<min){
            index=i;
            min=ants[i].length;
            for(int j=0;j<NUM;j++){
                way[j]=ants[index].ant_way[j];
            }
        }
        sum+=ants[i].length; 
    }
    printf("This time average length: %.0f\n",sum/ANT_NUM);
    // printf("%.0f\n", min);
    
}

/*debug*/
void debug(int a,int b)
{
    printf("\n***debug***\n");
    for(int i=0;i<a;i++)
    {
        printf("\nAnt %d: ",i);
        for(int j=0;j<b;j++)
        {
            printf("%d", ants[i].ant_way[j]);
        }
    }
    printf("\n***debug***\n");
}