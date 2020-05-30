#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define MAX_DEPTH 5     //�������

int chess_board[23][23]={
{44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44},
{44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44},
{44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44},
{44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,44},
{44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44},
{44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44},
{44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44},
{44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44},};

void put_board(void);   //�������
void illustrate(void);  //����˵��
int judge(void);        //�ж�����
void AI_play(void);     //AI����
void player(void);      //�������
void game(void);        //��Ϸ
int value(void);        //��������
int simulate(int depth,int inf,int sup);    //ģ�⺯��
long search_num=0,cut_num=0;
int AI_win=2,PLAY_win=0;
char name[20];

int main()
{
    illustrate();   //���������ʾ��Ϣ
    game();         //��Ϸ
    return 0;
}

void put_board(void)    //������̺���
{
    int i,j;
    printf("��������Ϊ��\n");
    printf("  1 2 3 4 5 6 7 8 9 a b c d e f\n");
    for(i=4;i<19;i++)
    {
        printf("%x ",i-3);
        for(j=4;j<19;j++)
        {
            printf("%c ",chess_board[i][j]==0?'0':(chess_board[i][j]==1?'Z':'X'));
        }
        printf("\n");
    }        
}

void illustrate()       //����˵������
{
    printf("****************************************");
    printf("\n**************����������AI**************");
    printf("\n**************����һս��****************\n");
    printf("****************************************");
    printf("\n��������ǣ�\n");
    scanf("%s",name);
    printf("���%s���������ǿ�ʼ��",name);
    printf("\n�����߰�\n");
}

int judge(void)         //�����жϺ�����0������Ϸ��1���ʤ����2AIʤ��
{
    int row,col,xx1,xx2;
    int i,j;
    for(i=4;i<15;i++)
    {
        for(j=4;j<15;j++)
        {
            if(chess_board[i][j]==0) continue;
            row=chess_board[i][j]+chess_board[i][j+1]+chess_board[i][j+2]+chess_board[i][j+3]+chess_board[i][j+4];
            col=chess_board[i][j]+chess_board[i+1][j]+chess_board[i+2][j]+chess_board[i+3][j]+chess_board[i+4][j];
            xx1=chess_board[i][j]+chess_board[i+1][j+1]+chess_board[i+2][j+2]+chess_board[i+3][j+3]+chess_board[i+4][j+4];
            xx2=chess_board[i][j]+chess_board[i-1][j+1]+chess_board[i-2][j+2]+chess_board[i-3][j+3]+chess_board[i-4][j+4];
            if(row==30||col==30||xx1==30||xx2==30) return 2;
            if(row==5||col==5||xx1==5||xx2==5) return 1;
        }
    }
    return 0;
}

void AI_play(void)      //AI���庯��
{
    search_num=0;
    cut_num=0;
    printf("AI�Ļغϣ�\n");
    printf("����˼��һ��...\n");
    int max_x=0,max_y=0;
    int i,j,temp;
    int max_score=-9999999;
    for(i=4;i<19;i++)
    {
        for(j=4;j<19;j++)
        {
            if(chess_board[i][j]!=0||((chess_board[i-1][j]==0||chess_board[i-1][j]==44)&&(chess_board[i+1][j]==0||chess_board[i+1][j]==44)&&(chess_board[i][j+1]==0||chess_board[i][j+1]==44)&&(chess_board[i][j-1]==0||chess_board[i][j-1]==44)&&(chess_board[i-1][j-1]==0||chess_board[i-1][j-1]==44)&&(chess_board[i-1][j+1]==0||chess_board[i-1][j+1]==44)&&(chess_board[i+1][j-1]==0||chess_board[i+1][j-1]==44)&&(chess_board[i+1][j+1]==0||chess_board[i+1][j+1]==44))) continue; 
            chess_board[i][j]=6;
            if((temp=simulate(1,max_score,9999999))>max_score)
            {
                max_x=i;
                max_y=j;
                max_score=temp;
            }
            chess_board[i][j]=0;
        }
    }
    if(max_x==0&&max_y==0)
    {
    //    printf("���þ��ߣ���\n");
    //    printf("���þ��ߣ���\n");
    //    printf("���þ��ߣ���\n");
        for(i=4;i<19;i++)
        {
            for(j=4;j<19;j++)
            {
                if(chess_board[i][j]!=0) continue; 
                chess_board[i][j]=6;
                if((temp=value())>max_score)
                {
                    max_x=i;
                    max_y=j;
                    max_score=temp;
                }
                chess_board[i][j]=0;
            }
        }    
    }
    chess_board[max_x][max_y]=6;
    printf("/*����%ld�Σ���֦%ld�Ρ�*/\n",search_num,cut_num);//debug��
}

void player(void)       //������庯��
{
    int a,b;
    put_board();    
    printf("��ҵĻغϣ�����Ҫ�µ�λ�ã����磺5 5\n");
    scanf("%x%x",&a,&b);
    while(chess_board[3+a][3+b]!=0)
    {
        printf("������������䣡\n");
        scanf("%x%x",&a,&b);
    }
    chess_board[3+a][3+b]=1;
}

void game(void)         //��Ϸ����
{
    char ch;
    int i,j;
    int who_play=1,flag=0;   //who_play��ʾ��ǰ������ˣ�1��ʾ�����֣�0��ʾAI���֣���flag�ж���Ϸ�Ƿ������ʤ����˭
    while(!flag)                                    //flagΪ0��ʾ������Ϸ��Ϊ1��ʾ���ʤ����Ϊ2��ʾAIʤ��
    {
        if(who_play)
        {
            player();
            who_play--;
        }
        else
        {
            AI_play();
            who_play++;
        }
        flag=judge();
    /*    printf("\nNow the value_score=%d\n",value());  */
    }
    if(flag==1){printf("��Ӯ�ˣ���\n");PLAY_win++;} 
    if(flag==2){put_board();printf("��Ӯ�ˣ���\n");AI_win++;}
    printf("\n��ǰ�ȷ�:\tAI:%s %d:%d",name,AI_win,PLAY_win); 
    printf("����һ�֣�������Y������Ϸ������N������\n");
    getchar();
    while((ch=getchar())!='Y'&&ch!='N') printf("��������\n");
    if(ch=='Y') 
    {
        for(i=4;i<19;i++)   //����
            for(j=4;j<19;j++)
            chess_board[i][j]=0;
        AI_win=0;
        PLAY_win=0;
        game();
    }
    return;
}

int value(void)     //��������
{
    int num,score=0;
    int i,j;
    int m,n,left,right;
    for(i=4;i<19;i++)
    {
        for(j=4;j<19;j++)
        {
            if(chess_board[i][j]==0) continue;
            if(chess_board[i][j]==1)
            {
                for(m=j;chess_board[i][m-1]==1;m--);
                for(n=j;chess_board[i][n+1]==1;n++);
                num=n-m+1;      //һ���������������ĸ���
                if(num==5) return -1000000;
                else
                {
                    for(left=m;chess_board[i][left-1]==0||chess_board[i][left-1]==1;left--);
                    for(right=n;chess_board[i][right+1]==0||chess_board[i][right+1]==1;right++);
                    if(right-left+1>=5)
                    {
                        switch(num)
                        {
                        case 1: score-=20+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 2: score-=300+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 3: if(left==m||right==n) score-=300+(right-n)+(m-left)-abs((right-n)-(m-left)); 
                                else score-=2000+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 4: score-=7500+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        }
                    }
                }

                for(m=i;chess_board[m-1][j]==1;m--);
                for(n=i;chess_board[n+1][j]==1;n++);
                num=n-m+1;      //һ���������������ĸ���
                if(num==5) return -1000000;
                else
                {
                    for(left=m;chess_board[left-1][j]==0||chess_board[left-1][j]==1;left--);
                    for(right=n;chess_board[right+1][j]==0||chess_board[right+1][j]==1;right++);
                    if(right-left+1>=5)
                    {
                        switch(num)
                        {
                        case 1: score-=20+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 2: score-=300+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 3: if(left==m||right==n) score-=300+(right-n)+(m-left)-abs((right-n)-(m-left)); 
                                else score-=2000+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 4: score-=7500+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        }
                    }
                }

                for(m=j;chess_board[i-(j-m)-1][m-1]==1;m--);
                for(n=j;chess_board[i+(n-j)+1][n+1]==1;n++);
                num=n-m+1;      //һ���������������ĸ���
                if(num==5) return -1000000;
                else
                {
                    for(left=m;chess_board[i-(j-m)-(m-left)-1][left-1]==0||chess_board[i-(m-left)-1][left-1]==1;left--);
                    for(right=n;chess_board[i+(n-j)+(right-n)+1][right+1]==0||chess_board[i+(right-n)+1][right+1]==1;right++)
                    {

                    };
                    if(right-left+1>=5)
                    {
                        switch(num)
                        {
                        case 1: score-=20+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 2: score-=300+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 3: if(left==m||right==n) score-=300+(right-n)+(m-left)-abs((right-n)-(m-left)); 
                                else score-=2000+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 4: score-=7500+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        }
                    }
                }

                for(m=j;chess_board[i+(j-m)+1][m-1]==1;m--);
                for(n=j;chess_board[i-(n-j)-1][n+1]==1;n++);
                num=n-m+1;      //һ���������������ĸ���
                if(num==5) return -1000000;
                else
                {
                    for(left=m;chess_board[i+(j-m)+(m-left)+1][left-1]==0||chess_board[i+(m-left)+1][left-1]==1;left--);
                    for(right=n;chess_board[i-(n-j)-(right-n)-1][right+1]==0||chess_board[i-(right-j)-1][right+1]==1;right++);
                    if(right-left+1>=5)
                    {
                        switch(num)
                        {
                        case 1: score-=20+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 2: score-=300+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 3: if(left==m||right==n) score-=300+(right-n)+(m-left)-abs((right-n)-(m-left)); 
                                else score-=2000+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 4: score-=7500+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        }
                    }
                }
            }
            else    //��AI������
            {
                for(m=j;chess_board[i][m-1]==6;m--);
                for(n=j;chess_board[i][n+1]==6;n++);
                num=n-m+1;      //һ���������������ĸ���
                if(num==5) return 1000000;
                else
                {
                    for(left=m;chess_board[i][left-1]==0||chess_board[i][left-1]==6;left--);
                    for(right=n;chess_board[i][right+1]==0||chess_board[i][right+1]==6;right++);
                    if(right-left+1>=5)
                    {
                        switch(num)
                        {
                        case 1: score+=20+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 2: score+=300+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 3: if(left==m||right==n) score+=300+(right-n)+(m-left)-abs((right-n)-(m-left)); 
                                else score+=1000+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 4: score+=4500+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        }
                    }
                }

                for(m=i;chess_board[m-1][j]==6;m--);
                for(n=i;chess_board[n+1][j]==6;n++);
                num=n-m+1;      //һ���������������ĸ���
                if(num==5) return 1000000;
                else        
                {
                    for(left=m;chess_board[left-1][j]==0||chess_board[left-1][j]==6;left--);
                    for(right=n;chess_board[right+1][j]==0||chess_board[right+1][j]==6;right++);
                    if(right-left+1>=5)
                    {
                        switch(num)
                        {
                        case 1: score+=20+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 2: score+=300+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 3: if(left==m||right==n) score+=300+(right-n)+(m-left)-abs((right-n)-(m-left)); 
                                else score+=1000+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 4: score+=4500+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        }
                    }
                }

                for(m=j;chess_board[i-(j-m)-1][m-1]==6;m--);
                for(n=j;chess_board[i+(n-j)+1][n+1]==6;n++);
                num=n-m+1;      //һ���������������ĸ���
                if(num==5) return 1000000;
                else
                {
                    for(left=m;chess_board[i-(j-m)-(m-left)-1][left-1]==0||chess_board[i-(m-left)-1][left-1]==6;left--);
                    for(right=n;chess_board[i+(n-j)+(right-n)+1][right+1]==0||chess_board[i+(right-n)+1][right+1]==6;right++);
                    if(right-left+1>=5)
                    {
                        switch(num)
                        {
                        case 1: score+=20+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 2: score+=300+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 3: if(left==m||right==n) score+=300+(right-n)+(m-left)-abs((right-n)-(m-left)); 
                                else score+=1000+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 4: score+=4500+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        }
                    }
                }

                for(m=j;chess_board[i+(j-m)+1][m-1]==6;m--);
                for(n=j;chess_board[i-(n-j)-1][n+1]==6;n++);
                num=n-m+1;      //һ���������������ĸ���
                if(num==5) return 1000000;
                else
                {
                    for(left=m;chess_board[i+(j-m)+(m-left)+1][left-1]==0||chess_board[i+(m-left)+1][left-1]==6;left--);
                    for(right=n;chess_board[i-(n-j)-(right-n)-1][right+1]==0||chess_board[i-(right-j)-1][right+1]==6;right++);
                    if(right-left+1>=5)
                    {
                        switch(num)
                        {
                        case 1: score+=20+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 2: score+=300+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 3: if(left==m||right==n) score+=300+(right-n)+(m-left)-abs((right-n)-(m-left)); 
                                else score+=1000+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        case 4: score+=4500+(right-n)+(m-left)-abs((right-n)-(m-left));break;
                        }
                    }
                }
            }
        }
    }
    return score;
}

int simulate(int depth,int inf,int sup) //ģ�⺯��
{
    search_num++;
    int score=value();
    if(score==1000000) return 1000000;
    if(score==-1000000) return -1000000;
    int i,j,temp;
    if(depth==MAX_DEPTH)
    {
        return score;
    }
    if(depth%2) //������ģ�������С����
    {
        for(i=4;i<19;i++)
        {
            for(j=4;j<19;j++)
            {
                if(chess_board[i][j]!=0||((chess_board[i-1][j]==0||chess_board[i-1][j]==44)&&(chess_board[i+1][j]==0||chess_board[i+1][j]==44)&&(chess_board[i][j+1]==0||chess_board[i][j+1]==44)&&(chess_board[i][j-1]==0||chess_board[i][j-1]==44)&&(chess_board[i-1][j-1]==0||chess_board[i-1][j-1]==44)&&(chess_board[i-1][j+1]==0||chess_board[i-1][j+1]==44)&&(chess_board[i+1][j-1]==0||chess_board[i+1][j-1]==44)&&(chess_board[i+1][j+1]==0||chess_board[i+1][j+1]==44))) continue;
                chess_board[i][j]=1;
                if((temp=simulate(depth+1,-9999999,score))<=inf){chess_board[i][j]=0;cut_num++;return -9999999;} 
                if(temp<score)
                {
                    score=temp;
                }
                if(score==-1000000)
                {chess_board[i][j]=0;
                return score;}
                chess_board[i][j]=0;
            }
        }
    }
    else        //ż����AI������
    {
        for(i=4;i<19;i++)
        {
            for(j=4;j<19;j++)
            {
                if(chess_board[i][j]!=0||((chess_board[i-1][j]==0||chess_board[i-1][j]==44)&&(chess_board[i+1][j]==0||chess_board[i+1][j]==44)&&(chess_board[i][j+1]==0||chess_board[i][j+1]==44)&&(chess_board[i][j-1]==0||chess_board[i][j-1]==44)&&(chess_board[i-1][j-1]==0||chess_board[i-1][j-1]==44)&&(chess_board[i-1][j+1]==0||chess_board[i-1][j+1]==44)&&(chess_board[i+1][j-1]==0||chess_board[i+1][j-1]==44)&&(chess_board[i+1][j+1]==0||chess_board[i+1][j+1]==44))) continue;
                chess_board[i][j]=6;
                if((temp=simulate(depth+1,score,9999999))>=sup){chess_board[i][j]=0;cut_num++;return 9999999;}
                if(temp>score)
                {
                    score=temp;
                }
                if(score==1000000){chess_board[i][j]=0;return score;}
                chess_board[i][j]=0;
            }
        }
    }
    return score;
}