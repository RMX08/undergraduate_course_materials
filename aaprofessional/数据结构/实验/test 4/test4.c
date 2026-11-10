#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  // 添加errno头文件
#include <direct.h>  // Windows系统使用direct.h

#define MAX_CHAR 128  // ASCII字符集大小
#define MAX_TREE_HT 100

// Huffman树节点结构
typedef struct {
    int weight;
    int parent, lchild, rchild;
    int ch; // 新增：保存字符的ASCII码，非叶子节点为-1
} HTNode, *HuffmanTree;

typedef char **HuffmanCode;  // 动态二维数组，存储Huffman编码


void countFrequency(const char* filename, int* freq);  // 统计字符频率

void selectMin(HuffmanTree HT, int n, int* s1, int* s2);  // 选择权值最小的两个节点

void createHuffmanTree(HuffmanTree* HT, int* freq, int* charList, int n);  // 创建Huffman树

void createHuffmanCode(HuffmanTree HT, char** HC, int n, int* charList);  // 生成Huffman编码

void encodeFile(const char* inputFile, const char* outputFile, char** HC);  // 编码文件
 
void decodeFile(const char* inputFile, const char* outputFile, HuffmanTree HT, int n);  // 解码文件

int getCharList(const int* freq, int* charList, int* n);  // 统计频率时，记录字符和频率

int main() {
    int freq[MAX_CHAR] = {0};
    int charList[MAX_CHAR]; // 保存出现过的字符ASCII码
    int n = 0;
    HuffmanTree HT;
    char* HC[MAX_CHAR] = {0}; // 编码表

    printf("开始处理文件...\n");
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("当前工作目录: %s\n", cwd);
    }

    // 使用相对路径
    const char* inputFile = "../a.txt";
    const char* encodedFile = "../b.txt";
    const char* decodedFile = "../c.txt";

    // 统计字符频率
    countFrequency(inputFile, freq);

    // 获取字符列表
    getCharList(freq, charList, &n);

    // 创建Huffman树
    createHuffmanTree(&HT, freq, charList, n);

    // 生成Huffman编码
    createHuffmanCode(HT, HC, n, charList);

    // 编码文件
    encodeFile(inputFile, encodedFile, HC);

    // 解码文件
    decodeFile(encodedFile, decodedFile, HT, n);

    // 释放内存
    free(HT);
    for (int i = 0; i < MAX_CHAR; i++) {
        if (HC[i]) free(HC[i]);
    }
    return 0;
}

// 统计文件中字符出现的频率
void countFrequency(const char* filename, int* freq) {
    printf("尝试打开文件: %s\n", filename);
    FILE* fp = fopen(filename, "r");
    if(!fp) {
        printf("无法打开文件 %s\n", filename);
        printf("错误信息: %s\n", strerror(errno));
        char cwd[1024];
        if (_getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("当前工作目录: %s\n", cwd);
        }
        exit(1);
    }
    printf("成功打开文件\n");

    char ch;
    while((ch = fgetc(fp)) != EOF) {
        if(ch >= 0 && ch < MAX_CHAR) {
            freq[ch]++;  // 对应字符的频率加1
        }
    }
    fclose(fp);
}

// 在Huffman树中选择权值最小的两个节点
void selectMin(HuffmanTree HT, int n, int* s1, int* s2) {
    int min1 = INT_MAX, min2 = INT_MAX;  // 初始化两个最小值为最大整数
    *s1 = *s2 = 0;  // 初始化两个最小节点的下标

    // 遍历所有节点，找出权值最小的两个节点
    for(int i = 1; i <= n; i++) {
        if(HT[i].parent == 0) {  // 只考虑未被选中的节点
            if(HT[i].weight < min1) {
                min2 = min1;
                *s2 = *s1;
                min1 = HT[i].weight;
                *s1 = i;
            }
            else if(HT[i].weight < min2) {
                min2 = HT[i].weight;
                *s2 = i;
            }
        }
    }
}

// 统计频率时，记录字符和频率
int getCharList(const int* freq, int* charList, int* n) {
    *n = 0;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (freq[i] > 0) {
            charList[*n] = i;
            (*n)++;
        }
    }
    return *n;
}

// 构建Huffman树，叶子节点保存字符ASCII码
void createHuffmanTree(HuffmanTree* HT, int* freq, int* charList, int n) {
    if (n <= 1) return;
    int m = 2 * n - 1;
    *HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));
    if (!*HT) exit(1);
    // 初始化叶子节点
    for (int i = 1; i <= n; i++) {
        (*HT)[i].weight = freq[charList[i - 1]];
        (*HT)[i].parent = (*HT)[i].lchild = (*HT)[i].rchild = 0;
        (*HT)[i].ch = charList[i - 1]; // 保存ASCII码
    }
    // 初始化非叶子节点
    for (int i = n + 1; i <= m; i++) {
        (*HT)[i].weight = 0;
        (*HT)[i].parent = (*HT)[i].lchild = (*HT)[i].rchild = 0;
        (*HT)[i].ch = -1;
    }
    // 构建树
    for (int i = n + 1; i <= m; i++) {
        int s1 = 0, s2 = 0;
        selectMin(*HT, i - 1, &s1, &s2);
        (*HT)[s1].parent = (*HT)[s2].parent = i;
        (*HT)[i].lchild = s1;
        (*HT)[i].rchild = s2;
        (*HT)[i].weight = (*HT)[s1].weight + (*HT)[s2].weight;
    }
}

// 生成Huffman编码，编码表下标为ASCII码
void createHuffmanCode(HuffmanTree HT, char** HC, int n, int* charList) {
    char* cd = (char*)malloc(n * sizeof(char));
    cd[n - 1] = '\0';
    for (int i = 1; i <= n; i++) {
        int start = n - 1;
        int c = i, f = HT[i].parent;
        while (f != 0) {
            if (HT[f].lchild == c)
                cd[--start] = '0';
            else
                cd[--start] = '1';
            c = f;
            f = HT[f].parent;
        }
        HC[HT[i].ch] = (char*)malloc(n - start);
        strcpy(HC[HT[i].ch], &cd[start]);
    }
    free(cd);
}

// 编码文件
void encodeFile(const char* inputFile, const char* outputFile, char** HC) {
    printf("开始编码文件...\n");
    FILE* in = fopen(inputFile, "r");
    FILE* out = fopen(outputFile, "w");
    if (!in || !out) {
        printf("无法打开文件\n");
        exit(1);
    }
    char ch;
    while ((ch = fgetc(in)) != EOF) {
        if (ch >= 0 && ch < MAX_CHAR && HC[(unsigned char)ch]) {
            fprintf(out, "%s", HC[(unsigned char)ch]);
        }
    }
    fclose(in);
    fclose(out);
}

// 解码文件
void decodeFile(const char* inputFile, const char* outputFile, HuffmanTree HT, int n) {
    FILE* in = fopen(inputFile, "r");
    FILE* out = fopen(outputFile, "w");
    if (!in || !out) {
        printf("无法打开文件\n");
        exit(1);
    }
    int root = 2 * n - 1;
    int p = root;
    char ch;
    while ((ch = fgetc(in)) != EOF) {
        if (ch == '0')
            p = HT[p].lchild;
        else if (ch == '1')
            p = HT[p].rchild;
        if (HT[p].lchild == 0 && HT[p].rchild == 0) {
            fputc(HT[p].ch, out);
            p = root;
        }
    }
    fclose(in);
    fclose(out);
}
