//
// Created by ac034 on 2020/11/1.
//

/*一些数字信号处理模块，如fft、离散小波变换、归一化*/
#include <iostream>
#include "native-Da_Chuang.h"

#ifndef PI
#define PI (3.14159265f)
#endif
extern nor_data F4;
extern const int FEA_NUM;

int set_complex(vector<double> y, complex x[])
{
    //为complex结构体数组赋值
    int len = y.size();
    //complex x[len];
    if (len == 0)
        return 0;
    for (int i = 0; i < len; i++)
    {
        x[i].real = y[i];
        x[i].img = 0;
    }
    return 1;
}
static void BitReverse(complex *x, complex *r, int n, int l)
{
    int i = 0;
    int j = 0;
    short stk = 0;
    static complex *temp = 0;

    temp = (complex *)malloc(sizeof(complex) * n);
    if (!temp)
    {
        return;
    }

    for (i = 0; i < n; i++)
    {
        stk = 0;
        j = 0;
        do
        {
            stk |= (i >> (j++)) & 0x01;
            if (j < l)
            {
                stk <<= 1;
            }
        } while (j < l);

        if (stk < n)
        { /* 满足倒位序输出 */
            temp[stk] = x[i];
        }
    }
    for (i = 0; i < n; i++)
    {
        r[i] = temp[i];
    }
    free(temp);
}
int fft(complex *x, int N)
{
    //模2fft算法,结果依然存储在x中
    //注意这个算法只能算2的整数次幂长度的数组
    int i, j, l, ip;
    static int M = 0;
    static int le, le2;
    static FFT_TYPE sR, sI, tR, tI, uR, uI;

    M = (int)(log(N) / log(2));

    BitReverse(x, x, N, M);

    for (l = 1; l <= M; l++)
    {
        le = (int)pow(2, l);
        le2 = (int)(le / 2);
        uR = 1;
        uI = 0;
        sR = cos(PI / le2);
        sI = -sin(PI / le2);
        for (j = 1; j <= le2; j++)
        {
            for (i = j - 1; i <= N - 1; i += le)
            {
                ip = i + le2;
                tR = x[ip].real * uR - x[ip].img * uI;
                tI = x[ip].real * uI + x[ip].img * uR;
                x[ip].real = x[i].real - tR;
                x[ip].img = x[i].img - tI;
                x[i].real += tR;
                x[i].img += tI;
            }
            tR = uR;
            uR = tR * sR - uI * sI;
            uI = tR * sI + uI * sR;
        }
    }
    return 0;
}
vector<double> get_fft_abs(vector<double> data)
{
    //获取data的fft的幅度值
    double tmp1, tmp2;
    int n = data.size();
    complex x[n];
    vector<double> abs;
    set_complex(data, x);
    fft(x, n);
    for (int i = 0; i < n; i++)
    {
        tmp1 = pow(x[i].img, 2);
        tmp2 = pow(x[i].real, 2);
        abs.push_back(pow(tmp1 + tmp2, 0.5));
    }
    return abs;
}
features get_features(vector<double> data)
{
    //获取数据特征
    //vector<double> tmp;
    int n = data.size();
    features data_features;
    data_features = {0};
    for (int i = 0; i < data.size(); i++)
        data_features.mean += data[i];
    data_features.mean /= data.size();
    double sum_3 = 0, sum_4 = 0, sum_2 = 0, SUM_2 = 0, std;
    for (int i = 0; i < data.size(); i++)
    {
        sum_2 += pow(data[i] - data_features.mean, 2);
        sum_3 += pow(data[i] - data_features.mean, 3);
        sum_4 += pow(data[i] - data_features.mean, 4);
        SUM_2 += pow(data[i], 2);
    }
    data_features.std = sum_2 / (n - 1);
    data_features.std = pow(data_features.std, 0.5);
    std = sum_2 / n;
    data_features.skewness = sum_3 / n / pow(std, 1.5);
    data_features.kurtosis = sum_4 / n / pow(std, 2);
    data_features.rms = pow(SUM_2 / n, 0.5);
    sort(data.begin(), data.end());
    if (data.size() % 2)
        data_features.median = data[(n - 1) / 2];
    else
        data_features.median = (data[n / 2] + data[n / 2 - 1]) / 2;
    return data_features;
}

Features get_Features(vector<double> data)
{
    //获取数据的时域和频域特征
    Features f;
    vector<double> data_fft = get_fft_abs(data);
    f.time = get_features(data);
    f.fre = get_features(data_fft);
    return f;
}
vector<vector<double>> normalize(vector<vector<double>> x)
{
    //x是一个特征个数*样本个数的矩阵
    vector<double> data;
    vector<vector<double>> result;
    features f;
    double temp[FEA_NUM][2];
    for (int j = 0; j < x.size(); j++)
    {
        data = x[j];
        vector<double> nor_data;
        f = get_features(data);
        temp[j][0]=f.mean;
        temp[j][1]=f.std;
        for (int i = 0; i < data.size(); i++)
            nor_data.push_back((data[i] - f.mean) / f.std);
        result.push_back(nor_data);
    }
    F4.time.kurtosis.mean=temp[0][0];
    F4.time.kurtosis.std=temp[0][1];
    F4.time.mean.mean=temp[1][0];
    F4.time.mean.std=temp[1][1];
    F4.time.median.mean=temp[2][0];
    F4.time.median.std=temp[2][1];
    F4.time.skewness.mean=temp[3][0];
    F4.time.skewness.std=temp[3][1];
    F4.time.std.mean=temp[4][0];
    F4.time.std.std=temp[4][1];
    F4.time.rms.mean=temp[5][0];
    F4.time.rms.std=temp[5][1];


    F4.fre.std.mean=temp[0][0];
    F4.fre.std.std=temp[0][1];
    F4.fre.rms.mean=temp[1][0];
    F4.fre.rms.std=temp[1][1];
    F4.fre.skewness.mean=temp[2][0];
    F4.fre.skewness.std=temp[2][1];
    F4.fre.median.mean=temp[3][0];
    F4.fre.median.std=temp[3][1];
    F4.fre.mean.mean=temp[4][0];
    F4.fre.mean.std=temp[4][1];
    F4.fre.kurtosis.mean=temp[5][0];
    F4.fre.kurtosis.std=temp[5][1];

    return result;
}
Features normalize(vector<double> x, nor_data F44)
{
    //获取单个测试数据x归一化后的特征集
    vector<double> data;
    Features f = get_Features(x);

    features f_time = f.time;
    struct foobar2 F_time = F44.time; //归一化时域特征
    f_time.kurtosis = (f_time.kurtosis - F_time.kurtosis.mean) / F_time.kurtosis.std;
    f_time.std = (f_time.std - F_time.std.mean) / F_time.std.std;
    f_time.mean = (f_time.mean - F_time.mean.mean) / F_time.mean.std;
    f_time.skewness = (f_time.skewness - F_time.skewness.mean) / F_time.skewness.std;
    f_time.median = (f_time.median - F_time.median.mean) / F_time.median.std;
    f_time.rms = (f_time.rms - F_time.rms.mean) / F_time.rms.std;

    f_time = f.fre;
    F_time = F44.fre; //归一化频域特征，因为懒就没换名称
    f_time.kurtosis = (f_time.kurtosis - F_time.kurtosis.mean) / F_time.kurtosis.std;
    f_time.std = (f_time.std - F_time.std.mean) / F_time.std.std;
    f_time.mean = (f_time.mean - F_time.mean.mean) / F_time.mean.std;
    f_time.skewness = (f_time.skewness - F_time.skewness.mean) / F_time.skewness.std;
    f_time.median = (f_time.median - F_time.median.mean) / F_time.median.std;
    f_time.rms = (f_time.rms - F_time.rms.mean) / F_time.rms.std;
    return f;
}
vector<double>Change_Format(Features F44){
    vector<double>x;
    features time=F44.time;
    features fre=F44.fre;
    x.push_back(time.kurtosis);
    x.push_back(time.mean);
    x.push_back(time.median);
    x.push_back(time.skewness);
    x.push_back(time.std);
    x.push_back(time.rms);
    x.push_back(fre.std);
    x.push_back(fre.rms);
    x.push_back(fre.skewness);
    x.push_back(fre.median);
    x.push_back(fre.mean);
    x.push_back(fre.kurtosis);
    return x;
}