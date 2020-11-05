//#include <jni.h>
#include <stdio.h>
#include <string>
#include "native-Da_Chuang.h"
#include "native-svm.h"
struct svm_model *model;
struct svm_parameter param;
extern const int FEA_NUM=11;
struct svm_problem prob;
nor_data F4;//用于归一化的数据
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_da_1chuang_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    printf("hello");
    return env->NewStringUTF(hello.c_str());
}
