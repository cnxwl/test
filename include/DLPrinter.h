
//#include <pango-1.0/pango/pango.h>
//#include <zint.h>
//#include <zlib.h>
#include <string.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>
#include <stdbool.h>

#define WIDTH 70/25.4
#define HEIGHT 35/25.4       //毫米

//typedef uint8_t BYTE;

//using namespace std;

struct device_info {
    char name[128];
    int vid;
    int pid;
};

struct print_option {
    int darkness;       //打印浓度，1-30
    double dXPos;
    double dYPos;   //纵向位置，英寸
    bool btype;         //1,直接打印；0,发送命令 
};  

struct font_option {
    char fontName[128];            //字体名
    bool fontSlant;         //0：直，1：斜体
    bool fontWeight;     //0：细，1：粗体
    int fontSize;               //字体大小
};

struct code128_option {
    double codeHeight;     //条码高度
    int wScale;             //字符宽倍数
    int hScale;
    int width;                  //模块宽度
    bool readchar;      //是否打印字符，1打印，0不打印
    bool readPos;       //字符位置，1上方，0下方
};

void logptr(char *mes);

void setdpi(int ptrdpi);        //设置dpi，默认为203

int enumDevice(struct device_info *device);    //枚举设备

int openDevice(struct device_info *device);     //打开设备

int closeDevice(struct device_info *device);        //关闭设备

int limitDirect(struct print_option *print);            //检查横纵向范围

int DLPrintText(struct print_option *print, struct font_option *font, char *data);      //打印文本

int DLPrintCode(struct print_option *print,  struct code128_option *code128, char *data);      //打印条码  

int DLPrintQR(struct print_option *print, char errLevel, double qrSize, char *data);            //打印qr码

int DLPrintImage(struct print_option *print, char *path);       ////打印图片

int startPrint();       //开始一张标签，套打

int endPrint();         //结束标签，套打