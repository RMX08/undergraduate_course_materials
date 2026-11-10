# include<stdio.h>
# include<stdlib.h>

# define ROW 5
# define COL 5
# define MAXSIZE 100

int Maze[ROW][COL] = {0};

typedef struct
{
    int i;     //当前方块的行标
    int j;     //当前方块的列标
    int di;    //当前行走方向 1-下 2-右 3-上 4-左 逆时针旋转
}Grid;

typedef struct
{
    Grid data[MAXSIZE];//存放可通行路线方块
    int top;
}StType;

void Get_Maze(int Maze[ROW][COL]);//输入迷宫地图

void Print_Maze(int Maze[ROW][COL]);//输出当前迷宫

void Maze_Path(int x1,int y1,int x2,int y2);//计算迷宫路径


int main()
{
    int x1,y1,x2,y2;   //(x1,y1)起点  (x2,y2)终点

    Get_Maze(Maze);
    Print_Maze(Maze);

    printf("请输入起点坐标：\n");
    scanf("%d %d",&x1,&y1);
    printf("请输入终点坐标：\n");
    scanf("%d %d",&x2,&y2);

    Maze_Path(x1,y1,x2,y2);

    return 0;
}

void Get_Maze(int Maze[ROW][COL])
{
    printf("请输入当前迷宫地图：\n");
    for (int i = 0; i < ROW; i ++)//0~4
      for (int j = 0; j < COL; j ++)
        scanf("%d",&Maze[i][j]);
}

void Print_Maze(int Maze[ROW][COL])
{
    printf("\n当前存储的迷宫地图为：\n");
    for (int i = 0; i < ROW; i ++)
    {
      for (int j = 0; j < COL; j ++)
      {
        printf("%d ",Maze[i][j]);
      }
      printf("\n");
    }
    printf("\n");
}

void Maze_Path(int x1,int y1,int x2,int y2)//(x1,y2)->(x2,y2)
{
    int i,j,di;
    StType st;
    //初始化栈底起始位置
    st.top = 0; 
    st.data[st.top].i = x1;
    st.data[st.top].j = y1;
    st.data[st.top].di = 0;
    Maze[x1][y1] = -1;

    while(st.top>-1)
    {
      //取当前栈顶方块
      i = st.data[st.top].i;
      j = st.data[st.top].j;
      di = st.data[st.top].di;

      if (i == x2 && j == y2)//找到出口
      {
        for (int m = 0; m < ROW; m ++)
        {
          for (int n = 0; n < COL; n ++)
          {
            if (Maze[m][n] == -1)
                printf("# ");
            else
                printf("%d ",Maze[m][n]);
          }
          printf("\n");
        }
        break;   
      }

      while (di <= 4 )
      {
        di ++;
       if (di == 5)//该方块四个方向都没有可走路径, di=5
        {
          Maze[st.data[st.top].i][st.data[st.top].j] = 0;
          st.top --;
          break;
          // printf("i=%d j=%d di=%d st.top=%d\n\n",i,j,di,st.top);
        } 
            

        if (di == 1)//向下走
        {
          i = st.data[st.top].i + 1;
          j = st.data[st.top].j;
        }
        else if (di == 2)//向右走
        {
          i = st.data[st.top].i;
          j = st.data[st.top].j + 1;
        }
        else if (di == 3)//向上走
        {
          i = st.data[st.top].i - 1;
          j = st.data[st.top].j;
        }
        else if (di == 4)//向左走
        {
          i = st.data[st.top].i;
          j = st.data[st.top].j - 1;
        }

        if (Maze[i][j]== 1 || (i >= ROW || i < 0) || (j >= COL || j < 0))//障碍物或超出边界
        {
          i = st.data[st.top].i;
          j = st.data[st.top].j;
        }
        else if (Maze[i][j] == 0)//找到可走方块
        {
          st.data[st.top].di = di;
          //增加新的栈顶元素
          st.top ++;
          st.data[st.top].i = i;
          st.data[st.top].j = j;
          st.data[st.top].di = 0;
          Maze[i][j] = -1;//确保有此方块的后续路径不会重复走到该方块
          break;
        }
        else if (Maze[i][j] == -1)//已经走过
        {
          i = st.data[st.top].i;
          j = st.data[st.top].j;
        }        
      }
    }
    if (st.top == -1)
        printf("没有路径！！\n");
}