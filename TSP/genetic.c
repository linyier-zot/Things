#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM 48
#define INDIV_NUM 100                //种群中个体总数
#define N_C 400000         //迭代次数
#define CHANGE_BIT 30
#define PARENTS_NUM 66
#define varaiation_probability 0.01  //变异概率

struct node{
    int x;
    int y;
}points[NUM];               //点集

int way[NUM];               //搜索到的路径
int bestway[NUM];           //最优路径
double matrix[NUM][NUM];    //边值矩阵
int group[INDIV_NUM][NUM];  //种群|解集合
double grp_value[INDIV_NUM];//种群适应度
int childs[PARENTS_NUM/2][NUM];         //每代的孩子
// int v_times = 0;

double length(int*loc_way); //输入路径，输出长度
void value();//计算适应度
void init();                //初始化
void inherit();
void cross_exchange(int a,int b,int i);      //交叉互换
void variation();           //变异
void assess();              //最后与最优解比较
void debug(int a,int b);
void this_result();

int main()
{
    init();     //初始化点集、边矩阵、种群
    for(int n=0;n<=N_C;n++)
    {
        // debug(INDIV_NUM,NUM);
        value();
        inherit();
        if(!(n%100000)) {printf("\nGeneration%7d: ",n);this_result();}
    }

    
    // this_result();
    assess();       //比较结果与最优解
    // printf("\nv_times: %d\n",v_times);
    return 0;
}

/*初始化*/
void init(){
    FILE *data=fopen(".\\data.txt", "r");
    for(int i=0;i<NUM;i++){ /*初始化点集*/
        fscanf(data,"%*d%d%d",&points[i].x,&points[i].y);
    }
    fclose(data);

    for(int i=0;i<NUM;i++){ /*初始化边集*/
        for(int j=0;j<NUM;j++)
        {
            int x_d = abs(points[i].x - points[j].x);
            int y_d = abs(points[i].y - points[j].y);
            matrix[i][j]=sqrt(x_d*x_d+y_d*y_d);
        }
    }
    for(int i=0;i<INDIV_NUM;i++){   //种群随机初始化
        int identy[NUM]={0};
        for(int j=0;j<NUM;j++){
            int ab=rand()%NUM;
            while(identy[ab]) ab=rand()%NUM; 
            identy[ab]=1;
            group[i][j]=ab;
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
    double min=99999999;
    int index=0;
    for(int i=0;i<INDIV_NUM;i++){
        if(length(group[i])<min){
            min=length(group[i]);
            index=i;
        }
    }
    for(int i=0;i<NUM;i++) way[i]=group[index][i];
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

/*适应度函数*/
void value(){
    double sum=0;
    for(int i=0;i<INDIV_NUM;i++){
        grp_value[i]=(100000.0/length(group[i]));
        sum+=grp_value[i];
    }
    for(int i=0;i<INDIV_NUM;i++)  grp_value[i]=grp_value[i]/sum;
}

void cross_exchange(int a,int b,int i){
    for(int j=0;j<NUM;j++) childs[i][j]=group[a][j];
    int ran=rand()%(NUM-CHANGE_BIT),save[CHANGE_BIT],lose[CHANGE_BIT];  //0~NUM-CHANGE_BIT
    for(int j=0;j<CHANGE_BIT;j++){
        save[j]=childs[i][ran+j];
        childs[i][ran+j]=group[b][ran+j];
        lose[j]=group[b][ran+j];
    }
/*     if(1){
        printf("\nsave: %d,%d,%d,%d,%d",save[0],save[1],save[2],save[3],save[4]);
        printf("\nlose: %d,%d,%d,%d,%d",lose[0],lose[1],lose[2],lose[3],lose[4]);
    } */

    int flag=CHANGE_BIT; //交叉的位中有 flag 位不同
    for(int kk=0;kk<CHANGE_BIT;kk++){
        for(int jj=0;jj<CHANGE_BIT;jj++){
            if(save[kk]==lose[jj]){
                save[kk]=-1;
                lose[jj]=-1;
                flag--;
                break;
            }
        }
    }
/*     printf(" flag %d",flag);
    if(1){
        printf("\nsave: %d,%d,%d,%d,%d",save[0],save[1],save[2],save[3],save[4]);
        printf("\nlose: %d,%d,%d,%d,%d",lose[0],lose[1],lose[2],lose[3],lose[4]);
    } */
    for(int kk=0,j=0,l=0;kk<flag;kk++){
        while((save[kk]=save[j++])==-1);
        while((lose[kk]=lose[l++])==-1);
    }
    for(int j=0;j<NUM;j++){       //修正
        for(int k=0;k<flag;k++){
            if(childs[i][j]==lose[k]&&(j<ran||j>(ran+CHANGE_BIT-1))) childs[i][j]=save[k];
        }
    }
/*     if(1){
        printf("\nsave: %d,%d,%d,%d,%d",save[0],save[1],save[2],save[3],save[4]);
        printf("\nlose: %d,%d,%d,%d,%d\n",lose[0],lose[1],lose[2],lose[3],lose[4]);
    } */
}
void variation(){
    for(int i=0;i<PARENTS_NUM/2;i++){
        if((1.0*rand()/RAND_MAX)>varaiation_probability) continue;
        int a=rand()%NUM,b=rand()%NUM,temp;
        temp=childs[i][a];
        childs[i][a]=childs[i][b];
        childs[i][b]=temp;
        // v_times++;
    }
}
void inherit(){
    /*找5对交叉互换生出5个，依概率决定是否变异*/
    int mark1[INDIV_NUM]={0},parents[PARENTS_NUM];
    for(int i=0;i<PARENTS_NUM;i++){
        while(1){       //依照适应度概率选择父母
            int k;
            double ran=1.0*rand()/RAND_MAX;
            for(k=0;k<INDIV_NUM;k++){
                ran-=grp_value[k];
                if(ran<=0||k==INDIV_NUM-1)break;
            }
            if(!mark1[k]){mark1[k]=1;parents[i]=k;break;}
        }
    }

/*     printf("\nchoose parents\n");
    for(int i=0;i<10;i++) printf("%d ",parents[i]); */

    for(int i=0;i<PARENTS_NUM/2;i++){
        int a=2*i,b=2*i+1;
        cross_exchange(parents[a],parents[b],i);
    }
    variation();
    
    // debug(5,NUM);

    // printf("debug childs");
    // debug(5,NUM);
    
    int mark2[INDIV_NUM]={0},replace[PARENTS_NUM/2];
    for(int i=0;i<PARENTS_NUM/2;i++){
        while (1)
        {
            int k=rand()%INDIV_NUM;
            if(!mark2[k]&&!mark1[k]){mark2[k]=1;replace[i]=k;break;}
        }
    }
/*     printf("\nreplace:");
    for(int i=0;i<5;i++) printf("%d ", replace[i]); */
    for(int i=0;i<PARENTS_NUM/2;i++){
        for(int j=0,a=replace[i];j<NUM;j++){
            group[a][j]=childs[i][j];
        }
    }

    return;
}

void this_result(){
    double aver=0;
    for(int i=0;i<INDIV_NUM;i++) aver+=length(group[i]);
    printf("Now average length: %f\n",aver/INDIV_NUM);
}

/*debug*/
void debug(int a,int b)
{
    // printf("\n***debug***\n");
    for(int i=0;i<a;i++)
    {
        printf("\nrow %d: ",i);
        for(int j=0;j<b;j++)
        {
            printf("%d ",group[i][j]);
        }
    }
    // printf("\n***debug***\n");
}