#include <stdio.h>
int main()
{
    int data[10][5];
    FILE *fp = fopen("test.txt", "r");//test.txt换成你要存数据的txt文件，格式是每行5个数据，共10行
    //txt文件要和c文件在同一文件夹下
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 5; j++)
            fscanf(fp, "%d", &data[i][j]);
    fclose(fp);
}