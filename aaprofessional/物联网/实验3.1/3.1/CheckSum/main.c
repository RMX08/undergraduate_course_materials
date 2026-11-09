#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short getCheckSum(unsigned char *string, int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += string[i];
    }
    unsigned short result = sum % 0x10000;
    result = result ^ 0xFFFF; // 反码
    result += 1;             // 补码
    return result; 
}

int main()
{
	while (1){
		printf("---------------------\n");
		printf("生成校验和\n");
		printf("---------------------\n");
		printf("选择要校验字符的格式\n");
		printf("1、普通字符串校验\n");
		printf("2、16进制字符串校验\n");
		printf("---------------------\n");
		printf("请选择1或者2:");
		int flag;
		scanf("%d", &flag);

		if (flag == 1)
		{
			printf("\t《普通字符串校验！》\n");
			unsigned char str[1024];
			int i = 0;
			getchar();
			printf("请输入需要校验的字符串，以回车结束:\n");
			while ((str[i] = getchar()) != '\n')
			{
				i++;
			}
			unsigned short res = getCheckSum(str, i);
			printf("CheckSum result is:%02X %02X\n", (res >> 8) & 0xFF, res & 0xFF);
		}
		else if (flag == 2)
		{
			printf("\t《16进制字符串校验！》\n");
			unsigned char a[100];
			int n, i;
			printf("请输入字节的数量: ");
			scanf("%d", &n);//输入个数
			printf("请输入需要校验的字节，以空格分开，回车结束：\n");
			for (i = 0; i<n; i++)
				scanf("%x", &a[i]);
			printf("CheckSum result is: %X \n\n", getCheckSum(a, n));
		}
		else
		{
			printf("选择有误\n");
		}
	}
	return 0;
}

