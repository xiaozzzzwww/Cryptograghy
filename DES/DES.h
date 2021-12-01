// DES.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>

// TODO: 在此处引用程序需要的其他标头。
class DES
{
private:
    uint8_t mpt_byte[8]={0x00};                 //存放明文，按字节存储
    uint8_t en_mpt_byte[8]= { 0x00 };            //存放加密后的明文，即密文，按字节存储
    uint8_t de_mpt_byte[8]= { 0x00 };            //存放解密后的明文，按字节存储
    bool mpt[64]={0x00};                            //存放明文，按二进制存储
    bool mpk[64]={0x00};                             //存放密文，按二进制存储
    bool en_mpt[64]={0x00};                           //存放加密后的明文，及密文，按二进制存储
    bool de_mpt[64]={0x00};                            //存放解密后的明文，按二进制存储
    int real_byte_length=0;

protected:
    //压缩变换矩阵
    const int com_per[48] = {
                14,17,11,24,1,5,3,28,15,6,21,10,
                23,19,12,4,26,8,16,7,27,20,13,2,
                41,52,31,37,47,55,30,40,51,45,33,48,
                44,49,39,56,34,53,46,42,50,36,29,32 };
    //扩展置换矩阵   
    const int ex_per[48] = { 32,1,2,3,4,5,4,5,6,7,8,9,
                        8,9,10,11,12,13,12,13,14,15,16,17,
                        16,17,18,19,20,21,20,21,22,23,24,25,
                        24,25,26,27,28,29,28,29,30,31,32,1 };
    //p置换矩阵
    const int p_p[32] = {
                    16,7,20,21,29,12,28,17,
                    1,15,23,26,5,18,31,10,
                    2,8,24,14,32,27,3,9,
                    19,13,30,6,22,11,4,25 };
    //S盒矩阵
    const int SSS[8][4][16] = {
                    {
                    {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
                    {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
                    {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
                    {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
                    {
                    {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
                    {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
                    {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
                    {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
                    {
                    {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
                    {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
                    {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
                    {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
                    {
                    {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
                    {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
                    {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
                    {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
                    {
                    {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
                    {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
                    {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
                    {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
                    {
                    {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
                    {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
                    {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
                    {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
                    {
                    {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
                    {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
                    {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
                    {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
                    {
                    {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
                    {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
                    {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
                    {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
    };

    //初始置换表
    const int IP[64] = { 58,50,42,34,26,18,10,2,
                60,52,44,36,28,20,12,4,
                62,54,46,38,30,22,14,6,
                64,56,48,40,32,24,16,8,
                57,49,41,33,25,17,9,1,
                59,51,43,35,27,19,11,3,
                61,53,45,37,29,21,13,5,
                63,55,47,39,31,23,15,7 }; 
     //最终置换表
	const int FP[64] = { 40,8,48,16,56,24,64,32,
                39,7,47,15,55,23,63,31,
                38,6,46,14,54,22,62,30,
                37,5,45,13,53,21,61,29,
                36,4,44,12,52,20,60,28,
                35,3,43,11,51,19,59,27,
                34,2,42,10,50,18,58,26,
                33,1,41,9,49,17,57,25 };
    //第i轮获取子密钥需要位移的位数
    const int key_move[17] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1,0 };
public:
    void print_ch(std::string name, bool* ch, int length);                      //打印长度为length的bool型数组
    void print_mpt();                                           //打印明文
    void print_enmpt();                                         //打印加密后的明文，即密文
    void print_dempt();                                         //打印解密后的明文
    void print_mpt_byte();                                      //打印明文，按字节输出
    void print_en_mpt_byte();                                   //打印加密后的明文,明文，按字节输出
    void print_de_mpt_byte();                                   //打印解密后的明文，按字节输出
    //void assign_val(bool* output, const bool* input, int length);
    template<typename t1,typename t2>                           //模板
    void assign_val(t1 output, const t2 input, int length);     //将长度为length的input赋值给output
    void ByteToBit(bool* DatOut, unsigned char* DatIn, int Num);//字节转为二进制
    void BitToByte(unsigned char* DatOut, bool* DatIn, int Num);//二进制转为字节
    void BitToHex(unsigned char* DatOut, bool* DatIn, int Num); //字节转十六进制
    void HexToBit(bool* DatOut, unsigned char* DatIn, int Num); //十六进制转为字节
    void init_mpt();            //初始化明文
    void init_mpk();            //初始化密文
    void get_pk_L_R(const bool* pk, bool* Lpk, bool* Rpk);  //将密钥分为左右密钥
    void reverse(bool* a, int s, int e);                    //反转
    int ArrayShift(bool* a, int n, int m);                  //循环左移
    void get_pk_one_rough(bool* pk, int rough, int flag);   //获取一轮密钥
    void get_pk_by_rough(bool* Lpk, bool* Rpk, const int rough, int flag);      //根据轮数获取密钥
    void compress_permutation(bool* temp, const bool* Lpki, const bool* Rpki);  //压缩置换
    void permutation(bool* output, const bool* input,const int* p, int p_length);    //置换，根据规则p将input置换成output
    void xor_s_swap(bool* RPT_s, const bool* pk_com_per, const bool* RPT_ex);    //异或后s盒置换
    void xor_and_permutation(bool* LPT, bool* RPT, const bool* p_per);           //异或置换
    void toGet_L_Rpt(const bool* temp, bool* LPT, bool* RPT);                      //获取左右明文
    void merge_L_RPT(bool* temp, const bool* LPT, const bool* RPT);                //合并左右明文
    void rough_Function(bool* LPT, bool* RPT, bool* Lpk, bool* Rpk, const int i, int flag); //轮函数
    void En_cryption();         //加密
    void En_cryption(uint8_t* output, uint8_t* ch, uint8_t pk[8]);
    void De_cryption();         //解密
    void De_cryption(uint8_t* output, uint8_t* ch, uint8_t pk[8]);
    void test_decryption();
    void test();
};
