#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_VERTEX 20  // 最大顶点数
#define INF 999999    // 表示无穷大

// 景点信息结构
typedef struct {
    int code;           // 景点代号
    char name[50];      // 景点名称
    char info[200];     // 景点简介
} VertexInfo;

// 图结构
typedef struct {
    VertexInfo vertices[MAX_VERTEX];  // 顶点数组
    int edges[MAX_VERTEX][MAX_VERTEX]; // 邻接矩阵
    int vertexCount;                  // 顶点数量
} Graph;

// 函数声明
void initGraph(Graph* g);//初始化图

void addVertex(Graph* g, int code, const char* name, const char* info);//添加顶点

void addEdge(Graph* g, int v1, int v2, int weight);//添加边

void displayAllSpots(Graph* g);//显示所有景点信息

void displaySpotInfo(Graph* g, int code);//显示指定景点信息

void findShortestPath(Graph* g, int start, int end);//寻找最短路径

void dijkstra(Graph* g, int start, int end, int* dist, int* path);//Dijkstra算法实现

int main() {
    Graph g;
    initGraph(&g);
    
    // 添加景点信息
    addVertex(&g, 1, "图书馆", "中国海洋大学图书馆，是学校的标志性建筑之一，藏书丰富，环境优美。");
    addVertex(&g, 2, "体育馆", "现代化的体育场馆，配备完善的体育设施，是师生运动健身的好去处。");
    addVertex(&g, 3, "教学楼", "主要教学场所，配备先进的多媒体教学设备。");
    addVertex(&g, 4, "学生食堂", "提供多样化的餐饮选择，环境整洁。");
    addVertex(&g, 5, "行政楼", "学校行政办公中心，处理各类行政事务。");
    addVertex(&g, 6, "信息南楼", "配备先进的实验设备，是科研和实验教学的重要场所。");
    addVertex(&g, 7, "中海苑", "舒适的学生住宿区，配备完善的生活设施。");
    addVertex(&g, 8, "五子顶", "学校后山，风景优美，是休闲散步的好去处。");
    addVertex(&g, 9, "运动场", "标准田径场，可举办各类体育赛事。");
    addVertex(&g, 10, "树下空间", "教学楼8区的咖啡厅，可享受美味中西餐的同时，享受树下空间带来的惬意。");
    
    // 添加路径信息（边的权重表示距离，单位：米）
    addEdge(&g, 1, 2, 200);
    addEdge(&g, 1, 3, 300);
    addEdge(&g, 2, 4, 100);
    addEdge(&g, 3, 4, 250);
    addEdge(&g, 3, 5, 400);
    addEdge(&g, 4, 6, 300);
    addEdge(&g, 5, 6, 230);
    addEdge(&g, 6, 7, 450);
    addEdge(&g, 7, 8, 320);
    addEdge(&g, 8, 9, 120);
    addEdge(&g, 9, 10, 560);
    
    int choice;
    while (1) {
        printf("\n=== 中国海洋大学崂山校区导游系统 ===\n");
        printf("1. 显示所有景点信息\n");
        printf("2. 查询景点信息\n");
        printf("3. 查询最短路径\n");
        printf("0. 退出系统\n");
        printf("请选择功能 (0-3): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                displayAllSpots(&g);
                break;
            case 2: {
                int code;
                printf("请输入要查询的景点代号 (1-%d): ", g.vertexCount);
                scanf("%d", &code);
                displaySpotInfo(&g, code);
                break;
            }
            case 3: {
                int start, end;
                printf("请输入起点景点代号 (1-%d): ", g.vertexCount);
                scanf("%d", &start);
                printf("请输入终点景点代号 (1-%d): ", g.vertexCount);
                scanf("%d", &end);
                findShortestPath(&g, start, end);
                break;
            }
            case 0:
                printf("感谢使用，再见！\n");
                return 0;
            default:
                printf("无效的选择，请重试！\n");
        }
    }
    
    return 0;
}

// 初始化图
void initGraph(Graph* g) {
    g->vertexCount = 0;
    // 初始化邻接矩阵
    for (int i = 0; i < MAX_VERTEX; i++) {
        for (int j = 0; j < MAX_VERTEX; j++) {
            g->edges[i][j] = (i == j) ? 0 : INF;
        }
    }
}

// 添加顶点
void addVertex(Graph* g, int code, const char* name, const char* info) {
    if (g->vertexCount >= MAX_VERTEX) {
        printf("错误：超出最大顶点数限制！\n");
        return;
    }
    
    g->vertices[g->vertexCount].code = code;
    strcpy(g->vertices[g->vertexCount].name, name);
    strcpy(g->vertices[g->vertexCount].info, info);
    g->vertexCount++;
}

// 添加边
void addEdge(Graph* g, int v1, int v2, int weight) {
    // 由于是无向图，需要对称赋值
    g->edges[v1-1][v2-1] = weight;
    g->edges[v2-1][v1-1] = weight;
}

// 显示所有景点信息
void displayAllSpots(Graph* g) {
    printf("\n=== 景点列表 ===\n");
    for (int i = 0; i < g->vertexCount; i++) {
        printf("%d. %s\n", g->vertices[i].code, g->vertices[i].name);
    }
}

// 显示指定景点信息
void displaySpotInfo(Graph* g, int code) {
    if (code < 1 || code > g->vertexCount) {
        printf("错误：无效的景点代号！\n");
        return;
    }
    
    printf("\n=== 景点信息 ===\n");
    printf("代号：%d\n", g->vertices[code-1].code);
    printf("名称：%s\n", g->vertices[code-1].name);
    printf("简介：%s\n", g->vertices[code-1].info);
}

// 寻找最短路径
void findShortestPath(Graph* g, int start, int end) {
    if (start < 1 || start > g->vertexCount || end < 1 || end > g->vertexCount) {
        printf("错误：无效的景点代号！\n");
        return;
    }
    
    int dist[MAX_VERTEX];  // 存储最短距离
    int path[MAX_VERTEX];  // 存储路径
    
    dijkstra(g, start, end, dist, path);
    
    // 输出最短路径
    printf("\n=== 最短路径 ===\n");
    printf("从 %s 到 %s 的最短路径：\n", 
           g->vertices[start-1].name, g->vertices[end-1].name);
    
    if (dist[end-1] == INF) {
        printf("无法到达！\n");
        return;
    }
    
    // 输出路径
    int current = end - 1;
    int pathStack[MAX_VERTEX];
    int pathCount = 0;
    
    while (current != start - 1) {
        pathStack[pathCount++] = current;
        current = path[current];
    }
    pathStack[pathCount++] = start - 1;
    
    // 反向输出路径
    for (int i = pathCount - 1; i >= 0; i--) {
        printf("%s", g->vertices[pathStack[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n总距离：%d 米\n", dist[end-1]);
}

// Dijkstra算法实现
void dijkstra(Graph* g, int start, int end, int* dist, int* path) {
    int visited[MAX_VERTEX] = {0};  // 标记数组
    
    // 初始化距离数组
    for (int i = 0; i < g->vertexCount; i++) {
        dist[i] = g->edges[start-1][i];
        path[i] = (dist[i] < INF) ? start-1 : -1;
    }
    
    visited[start-1] = 1;
    
    // 主循环
    for (int i = 1; i < g->vertexCount; i++) {
        int min = INF;
        int u = -1;
        
        // 找到未访问的最短距离顶点
        for (int j = 0; j < g->vertexCount; j++) {
            if (!visited[j] && dist[j] < min) {
                min = dist[j];
                u = j;
            }
        }
        
        if (u == -1) break;  // 没有可达的顶点
        visited[u] = 1;
        
        // 更新距离
        for (int v = 0; v < g->vertexCount; v++) {
            if (!visited[v] && g->edges[u][v] < INF) {
                int newDist = dist[u] + g->edges[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    path[v] = u;
                }
            }
        }
    }
}
