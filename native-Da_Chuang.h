//
// Created by ac034 on 2020/11/1.
//

#ifndef DA_CHUANG_NATIVE_DA_CHUANG_H
#define DA_CHUANG_NATIVE_DA_CHUANG_H
#include <vector>
#include <cmath>
#include<algorithm>
using namespace std;
typedef double FFT_TYPE;
typedef struct complex_st
{
    //复数结构体，用于计算fft
    FFT_TYPE real;
    FFT_TYPE img;
} complex;
typedef struct
{
    //特征结构体，用于存储数据的均值、方差等特征
    double mean;
    double std;
    double skewness;
    double kurtosis;
    double rms;
    double median;
} features;
typedef struct{
    //时域和频域特征
    features time;
    features fre;
}Features;
struct foobar{
    double mean;
    double std;
};
struct foobar2{
    //用于测试数据归一化的训练集的均值和方差
    struct foobar mean;
    struct foobar std;
    struct foobar skewness;
    struct foobar kurtosis;
    struct foobar rms;
    struct foobar median;
};
typedef struct{
    struct foobar2 time;
    struct foobar2 fre;
}nor_data;


int get_start_index(vector<double> useful_data);
//获取手势相关数据段的起始点
int get_end_index(vector<double> useful_data, vector<double> pulse, int start_index);
//获取手势相关数据段的终止点
int get_pulse(vector<double> front_data, vector<double> &pulse);
//获取脉冲波形
int judge_gesture_exsit(vector<double> data);
//判断是否执行了手势


Features get_Features(vector<double> data);
//获取数据的时域和频域特征
vector<vector<double>> normalize(vector<vector<double>> x);
//x是一个特征个数*样本个数的矩阵，返回值是每个特征归一化后的矩阵
Features normalize(vector<double>x,nor_data F4);
//用训练集的特征数据对测试集特征归一化处理
vector<double>Change_Format(Features F4);
//将Features改变成vector模式
#endif //DA_CHUANG_NATIVE_DA_CHUANG_H
