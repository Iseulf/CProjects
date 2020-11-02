//
// Created by ac034 on 2020/11/2.
//本文件用于提供训练和预测的接口，输入量是特征值数组或矩阵
//
//#include <jni.h>
#include "native-Da_Chuang.h"
#include "native-svm.h"
struct svm_model *model;
struct svm_parameter param;
extern const int FEA_NUM=11;
struct svm_problem prob;
nor_data F4;//用于归一化的数据

vector<double>Translate(double *p,int len){
    vector<double>x;
    for(int i=0;i<len;i++)
        x.push_back(x[i]);
    return x;
}
double predict(vector<double>data){
    //传入数据段x，求出手势
    // TODO: implement predict()
    /*int len=env->GetArrayLength(x);
    double *p=env->GetDoubleArrayElements(x,NULL);
    vector<double>data=Translate(p,len);*/
    int len=data.size();
    vector<double> data_f=Change_Format(normalize(data,F4));
    auto data_node=(struct svm_node *) malloc(len * sizeof(struct svm_node));
    for(int i=0;i<len;i++){
        data_node[i].index=i+1;
        data_node[i].value=data_f[i];
    }
    return svm_predict(model, data_node);
}
int train(vector<vector<double>>data,vector<double>labels){
    //传入训练集，训练成功返回True，否则返回False
    //data是样本*特征个数的矩阵，labels是对应样本标签的数组
    // TODO: implement train()
    param.svm_type = C_SVC;
    param.kernel_type = RBF;
    param.degree = 3;
    param.gamma = 0;	// 1/num_features
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 1;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 0;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    int len=labels.size();
    //double *p1=env->GetDoubleArrayElements(labels,NULL);//标签数组
    prob.l=len;
    prob.x=(struct svm_node**)malloc(prob.l*sizeof(struct svm_node*));
    prob.y=(double*)malloc(prob.l*sizeof(double));
    for(int i=0;i<len;i++)
        prob.y[i]=labels[i];
    // prob.l=FEA_NUM;
    vector<vector<double>>f(FEA_NUM,vector<double>(len));//存储数据特征
    //struct svm_node **x= (struct svm_node**)malloc(prob.l*sizeof(struct svm_node*));
    //struct svm_node *xspace=(struct svm_node*)malloc(len*sizeof(struct svm_node));
    for(int i=0;i<len;i++){
        //求出数据特征矩阵f
        // jdoubleArray tmp= (jdoubleArray)(env->GetObjectArrayElement(data, i));
        //double *p2=env->GetDoubleArrayElements(tmp,NULL);
        //int l=env->GetArrayLength(tmp);
        vector<double>y;
        Features F=get_Features(data[i]);
        y=Change_Format(F);
        //f.push_back(y);
        for(int j=0;j<prob.l;j++)
            f[j][i]=y[j];
    }
    f=normalize(f);
    for(int i=0;i<len;i++){
        prob.x[i]=(struct svm_node*)malloc(FEA_NUM*sizeof(struct svm_node));
        for(int j=0;j<FEA_NUM;j++){
            //将f赋值给prob.x
            prob.x[i][j].value=f[j][i];
            prob.x[i][j].index=j+1;
        }
    }
    //prob.x=x;
    model=svm_train(&prob,&param);
    return 1;
}
int main(){
    vector<double>labels={1,2,1,1};
    vector<vector<double>>y={{1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4}};
    train(y,labels);
    return 0;
}