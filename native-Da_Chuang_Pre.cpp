//
// Created by ac034 on 2020/11/1.
//

/*对信号的预处理操作*/
#include <iostream>
#include "native-Da_Chuang.h"
const int FREQUENCE = 10;                    //采样频率
const int CON_TIME_LENGTH = 4.5 * FREQUENCE; //手势最长持续时间
const int WINDOW = FREQUENCE * 1.2;          //平均手势持续时间
const int LEFT_PULSE = FREQUENCE * 0.2;      //脉冲左侧离峰值点平均距离
const int RIGHT_PULSE = FREQUENCE * 0.4;     //脉冲右侧离峰值点平均距离

//using namespace std;
double distance(double x, double y)
{
    //计算dtw距离时的距离函数
    return (x - y) * (x - y);
}
int get_max_index(vector<double> x, int n)
{
    //获取数组x的第n个局部最大值的位置,n=-1表返回最大的
    if (x.size() == 0)
        return -1;
    if (x.size() == 1)
        return 0;
    if (x.size() == 2)
        return x[0] > x[1] ? 0 : 1;
    int max_index = 0, sum = 0;
    if (n == -1)
    {
        for (int i = 1; i < x.size(); i++)
            if (x[i] > x[max_index])
                max_index = i;
    }
    else
    {
        for (int i = 1; i < x.size(); i++)
        {
            if (x[i] >= x[i + 1] && x[i] >= x[i - 1])
            {
                sum++;
                max_index = i;
            }
            if (sum == n)
                break;
        }
    }
    return max_index;
}
int get_min_index(vector<double> x, int n)
{
    //获取数组x的第n个局部最小值的位置,n=-1表返回最小
    //如果不存在第n个，则返回最后一个
    if (x.size() == 0)
        return -1;
    if (x.size() == 1)
        return 0;
    if (x.size() == 2)
        return x[0] > x[1] ? 1 : 0;
    int min_index, sum = 0;
    if (n == -1)
    {
        for (int i = 1; i < x.size(); i++)
            if (x[i] < x[min_index])
                min_index = i;
    }
    else
    {
        for (int i = 1; i < x.size(); i++)
        {
            if (x[i] <= x[i + 1] && x[i] <= x[i - 1])
            {
                sum++;
                min_index = i;
            }
            if (sum == n)
                break;
        }
    }
    return min_index;
}
vector<double> short_energy(vector<double> data)
{
    //获取信号的短时能量
    //energy保存短时能量值
    vector<double> energy;
    if (data.size() == 0)
        return energy;
    int len = data.size();
    double now_energy;
    for (int i = 0; i < len; i++)
    {
        now_energy = 0;
        if (i + WINDOW <= len)
        {
            for (int j = i; j < i + WINDOW; j++)
                now_energy += data[j] * data[j];
        }
        else
        {
            for (int j = i; j < len; j++)
                now_energy += data[j] * data[j];
        }
        energy.push_back(now_energy);
    }
    return energy;
}
int get_start_index(vector<double> useful_data)
{
    //获取可能存在手势相关的数据段
    //useful_data用于保存手势相关数据段
    //成功返回1
    vector<double> energy=short_energy(useful_data);
    int start_index = 0;
    if (energy.size())
        return get_max_index(energy, -1);
}
double dtw_distance(vector<double> data, vector<double> noise)
{
    //计算数据与脉冲噪音之间的dtw距离
    vector<vector<double>> cost(data.size());
    double min;
    for (int i = 0; i < cost.size(); i++)
        cost[i].resize(noise.size());
    cost[0][0] = distance(data[0], noise[0]);
    for (int i = 1; i < data.size(); i++)
        cost[i][0] = cost[i - 1][0] + distance(data[i], noise[0]);
    for (int j = 1; j < noise.size(); j++)
        cost[0][j] = cost[0][j - 1] + distance(data[0], noise[j]);
    for (int i = 1; i < data.size(); i++)
        for (int j = 1; j < noise.size(); j++)
        {
            min = (cost[i - 1][j - 1] < cost[i][j - 1]) ? cost[i - 1][j - 1] : cost[i][j - 1];
            if (min > cost[i - 1][j])
                min = cost[i - 1][j];
            cost[i][j] = min + distance(data[i], noise[j]);
        }
    for (int i = 0; i <data.size(); i++)
    {
        for (int j = 0; j < noise.size(); j++)
            cout << cost[i][j] << '\t';
        cout << '\n';
    }
    return cost[data.size() - 1][noise.size() - 1];
}
int get_end_index(vector<double> useful_data, vector<double> pulse, int start_index)
{
    //获取手势相关数据段的终止点
    vector<double> dtw;
    double temp;
    for (int i = start_index; i < useful_data.size(); i++)
    {
        //得到dtw距离，保存至dtw数组中
        if (i + WINDOW <= useful_data.size())
        {
            vector<double> foobar(useful_data.begin() + i, useful_data.begin() + i + WINDOW);
            temp = dtw_distance(foobar, pulse);
        }
        dtw.push_back(temp);
    }
    return get_min_index(dtw, 1) + start_index;
}
int get_pulse(vector<double> front_data, vector<double> &pulse)
{
    //获得脉冲信号波形
    if (front_data.size() == 0)
        return 0;
    int max_index = get_max_index(front_data,-1);
    int left, right;
    left = (max_index - LEFT_PULSE) > 0 ? (max_index - LEFT_PULSE) : 0;
    right = (max_index + RIGHT_PULSE) > front_data.size() ? front_data.size() : (max_index + RIGHT_PULSE);
    for (int i = left; i < right; i++)
        pulse.push_back(front_data[i]);
    return 1;
}
int judge_exist(vector<double>data,vector<double>pulse){
    vector<double>data_energy;

}
int main()
{
    vector<double> a({1, 2, 0, 1, 1, 2});
    vector<double> b({1, 0, 1});
    int d = dtw_distance(a, b);
    cout << d;
    getchar();
}