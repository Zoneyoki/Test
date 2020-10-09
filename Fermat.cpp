#include "miracl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//引入需要的C函数库以及miracl库
int Fermat_Function(big Aim);
#define k 4   //设置安全参数


int main() {
	FILE* fp;   //声明fp是指针，用来指向FILE类型的对象
	big _TheTargetnumber;

	miracl* mip = mirsys(3000, 10);//定义该变量最大长度为3000，输入、输出、运算全部用十进制表示
	mip->IOBASE = 10;//子输入的变量和输出的变量所使用的进制都是16进制
	_TheTargetnumber = mirvar(0);//为其保留适当数量的内存位置来初始化变量，可以通过对函数mirkill()的后续调用来释放该存储器

	if ((fp = fopen("data.txt", "r+")) == NULL) {//打开文件
		printf("对不起打开文件失败......\n");
		exit(0);//正常运行程序并退出程序；
	} //判断文件是否能够正确打开

	while (!feof(fp)) {    //检测文件是否读取到末尾
		cinnum(_TheTargetnumber, fp); //从文件中读取一个数字进入，并将其强制转化为十六进制表的大数obj
		cotnum(_TheTargetnumber, stdout); //向屏幕上输出一个大数obj

		if (Fermat_Function(_TheTargetnumber))
			printf("该大数有 %6.4f%% 的概率是素数.\n", 100 * (1 - pow(0.5, k)));
		else
			printf("该大数一定是合数 \n");
	}
	fclose(fp);//关闭文件

	mirkill(_TheTargetnumber);  //释放大数所占用的空间
	mirexit();     //清除miracl系统
	return 0;
}


//Fermat小定理函数
int Fermat_Function(big Aim) {
	//变量声明
	int _CounterA, _CounterB;//声明两个循环计数器
	big _randomA, _Thegcd,_Testpow,_Index, _Consnumber;

	miracl* mip = mirsys(1500, 16);
	mip->IOBASE = 16;

	//变量初始化
	_CounterA = 0;
	_CounterB = 0;
	_randomA = mirvar(0);
	_Thegcd = mirvar(0);
	_Testpow = mirvar(0);
	_Index =  mirvar(0);
	_Consnumber = mirvar(1);

	srand((unsigned int)time(NULL));//防止随机数重复，就用系统时间进行初始化
	for (_CounterA = 0; _CounterA < k; _CounterA++)
	{
		bigrand(Aim, _randomA); //生成所需要的随机数A
		egcd(_randomA, Aim, _Thegcd);  //计算Aim和生成的随机数的最大公因数
		if (_Thegcd== _Consnumber)  //判断和随机数是否互素，它们的最大公因数如果不是1的话，compare函数将会返回1，不满足条件
		{
			powmod(_randomA, _Index, Aim, _Testpow);  //计算 ，如果r=1，则obj可能是素数，进入下一个if语句
			if (_Thegcd== _Consnumber)  _CounterB++;  //j是判断因子，如果一个数能够满足在当前的轮数下，满足上述的算法，则j能够计数；如果j不等于轮数，那么这个数就不是素数
				
		}
	}

	if (_CounterB == k)
		return 1;
	else
		return 0;

	/*mirkill(Aim);
	mirkill(_randomA);
	mirkill(_Index);
	mirkill(_Testpow);
	mirkill(_Thegcd);*/

}