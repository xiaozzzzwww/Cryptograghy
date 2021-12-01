// DES.cpp: 定义应用程序的入口点。
//

#include "DES.h"

using namespace std;

//输出数组
void DES::print_ch(std::string name, bool* ch, int length) {
    int i = 0;
    std::cout << name;
    while (i < length) {
        std::cout << ch[i++];
    }
    std::cout << std::endl;
}

void DES::print_mpt()
{
    print_ch("初始明文:",mpt,64);
}

void DES::print_enmpt()
{
    print_ch("加密后密文为:", en_mpt, 64);
}

void DES::print_dempt()
{
    print_ch("解密后明文为:", de_mpt, 64);
}

void DES::print_mpt_byte()
{
    cout << "初始明文为(按字节输出)：";
    for(int i=0;i<real_byte_length;i++)
    {
        printf("%2x", mpt_byte[i]);
    }
    cout << endl;
}

void DES::print_en_mpt_byte()
{
    cout << "加密后密文为(按字节输出)：";
    for (int i = 0; i < 8; i++)
    {
        //cout << en_mpt_byte[i];
        printf("%2x", en_mpt_byte[i]);
    }
    cout << endl;
}

void DES::print_de_mpt_byte()
{
    cout << "解密后明文为(按字节输出)：";
    for (int i = 0; i < real_byte_length; i++)
    {
        //cout << de_mpt_byte[i];
        printf("%2x", de_mpt_byte[i]);
    }
    cout << endl;
}

//赋值
 template<typename t1, typename t2>
 inline void DES::assign_val(t1 output, const t2 input, int length)
 {
     for (int i = 0; i < length; i++) {
         output[i] = input[i];
     }
 }

//字节转为位
void DES::ByteToBit(bool* DatOut, unsigned char* DatIn, int Num) {
    for (int i = 0; i < Num; i++) {
        DatOut[i] = (DatIn[i / 8] >> (i % 8)) & 0x01;
    }
}

//位转为字节
void DES::BitToByte(unsigned char* DatOut, bool* DatIn, int Num) {
    for (int i = 0; i < (Num / 8); i++)
        DatOut[i] = 0;
    for (int i = 0; i < Num; i++)
        DatOut[i / 8] |= DatIn[i] << (i % 8);
}

//二进制转为十六进制
void DES::BitToHex(unsigned char* DatOut, bool* DatIn, int Num)
{
    int i = 0;
    for (i = 0; i < Num / 4; i++)
    {
        DatOut[i] = 0;
    }
    for (i = 0; i < Num / 4; i++)
    {
        DatOut[i] = DatIn[i * 4] + (DatIn[i * 4 + 1] << 1)
            + (DatIn[i * 4 + 2] << 2) + (DatIn[i * 4 + 3] << 3);
        if ((DatOut[i] % 16) > 9)
        {
            DatOut[i] = DatOut[i] % 16 + '7';       //  余数大于9时处理 10-15 to A-F
        }                                     //  输出字符 
        else
        {
            DatOut[i] = DatOut[i] % 16 + '0';       //  输出字符       
        }
    }

}


//十六进制字符转二进制
void DES::HexToBit(bool* DatOut, unsigned char* DatIn, int Num)
{
    int i = 0;                        // 字符型输入 
    for (i = 0; i < Num; i++)
    {
        if ((DatIn[i / 4]) > '9')         //  大于9 
        {
            DatOut[i] = ((DatIn[i / 4] - '7') >> (i % 4)) & 0x01;
        }
        else
        {
            DatOut[i] = ((DatIn[i / 4] - '0') >> (i % 4)) & 0x01;
        }
    }
}

//明文初始化
void DES::init_mpt() {
    std::cout << "请按字节输入最多8字节（不足8字节填充0x00）" << std::endl;
	std::cout << "请输入需要加密的明文(按”*“结束)" << std::endl;
	unsigned char input[8];
	char c;
	int i = 0;
	std::cin >> c;
	while (i < 8 && c != '*') {
		input[i++] = c;
		std::cin >> c;
	}
	real_byte_length = i;
	while (i > 0 && i < 8) {
		input[i++] = 0x00;
	}
	ByteToBit(mpt, input, 64);
    assign_val(mpt_byte, input, 8);
}

void DES::init_mpk() {
    std::cout << "请按字节输入最多8字节（不足8字节填充0x00）" << std::endl;
    std::cout << "请输入加密用的密钥(按”*“结束)" << std::endl;
    unsigned char input[8];
    char c;
    int i = 0;
    std::cin >> c;
    while (i < 8 && c != '*') {
        input[i++] = c;
        std::cin >> c;
    }
    while (i > 0 && i < 8) {
        input[i++] = 0x00;
    }
    ByteToBit(mpk, input, 64);

}

//切割密钥
void DES::get_pk_L_R(const bool* pk, bool* Lpk, bool* Rpk) {
    int r = 0;
    for (int i = 0; i < 32; i++) {
        if ((i + 1) % 8 != 0)
            Lpk[i - r] = pk[i];
        else {
            r++;
        }
    }
    r = 0;
    for (int i = 32; i < 64; i++) {
        if ((i + 1) % 8 != 0)
            Rpk[i - 32 - r] = pk[i];
        else {
            r++;
        }
    }
}


void DES::reverse(bool* a, int s, int e)
{
    int i, t;
    for (i = s; i <= (e + s) / 2; i++)
    {
        t = a[i];
        a[i] = a[e + s - i];
        a[e + s - i] = t;
    }
}

int DES::ArrayShift(bool* a, int n, int m)
{
    if (m == 0) return 0;
    m = m % n;
    reverse(a, 0, n - 1); //全部逆序
    reverse(a, 0, m - 1); //逆序前m个
    reverse(a, m, n - 1); //逆序后面的
    return 0;
}

//flag==1 代表加密
void DES::get_pk_one_rough(bool* pk, int rough, int flag) {
    if (flag == 1) {
        ArrayShift(pk, 28, 28 - key_move[rough]);
    }
    else {
        ArrayShift(pk, 28, key_move[rough]);
    }
}

//获取第i轮密钥
void DES::get_pk_by_rough(bool* Lpk, bool* Rpk, const int rough, int flag) {
    get_pk_one_rough(Lpk, rough, flag);
    get_pk_one_rough(Rpk, rough, flag);
}

//压缩置换
void DES::compress_permutation(bool* temp, const bool* Lpki, const bool* Rpki) {
    bool pki[56];
    for (int i = 0; i < 28; i++) {
        pki[i] = Lpki[i];
    }
    for (int i = 28; i < 56; i++) {
        pki[i] = Rpki[i - 28];
    }
    for (int i = 0; i < 48; i++) {
        temp[i] = pki[com_per[i] - 1];
    }
}

//置换
void DES::permutation(bool* output, const bool* input,const int *p,int p_length) {
    for (int i = 0; i < p_length; i++) {
        output[i] = input[p[i] - 1];
    }
}

//S-盒替换
void DES::xor_s_swap(bool* RPT_s, const bool* pk_com_per, const bool* RPT_ex) {
    bool temp[48];
    bool num;
    for (int i = 0; i < 48; i++) {
        num = pk_com_per[i];
        num ^= RPT_ex[i];
        temp[i] = num;
    }
    for (int i = 0; i < 8; i++) {
        int num1 = 0, num2 = 0;
        {
            num1 = temp[i * 6];
            num1 = num1 * 2 + temp[6 * i + 5];
            num2 = temp[6 * i + 1];
            num2 = num2 * 2 + temp[6 * i + 2];
            num2 = num2 * 2 + temp[6 * i + 3];
            num2 = num2 * 2 + temp[6 * i + 4];
        }
        num = SSS[i][num1][num2];
        for (int j = 4 * i + 3; j >= 4 * i; j--) {
            if (num == 0) {
                RPT_s[j] = 0;
            }
            else {
                RPT_s[j] = num % 2;
                num /= 2;
            }
        }
    }
}

//异或与交换
void DES::xor_and_permutation(bool* LPT, bool* RPT, const bool* p_per) {
    bool temp;
    for (int i = 0; i < 32; i++) {
        temp = RPT[i];
        RPT[i] = LPT[i] ^ p_per[i];
        LPT[i] = temp;
    }
}


//获取左右PT
void DES::toGet_L_Rpt(const bool* temp, bool* LPT, bool* RPT) {
    for (int i = 0; i < 32; i++) {
        LPT[i] = temp[i];
    }
    for (int i = 32; i < 64; i++) {
        RPT[i - 32] = temp[i];
    }
}

//合并左右明文
void DES::merge_L_RPT(bool* temp, const bool* LPT, const bool* RPT) {
    for (int i = 0; i < 32; i++) {
        temp[i] = LPT[i];
    }
    for (int i = 32; i < 64; i++) {
        temp[i] = RPT[i - 32];
    }
}

void DES::rough_Function(bool* LPT, bool* RPT, bool* Lpk, bool* Rpk, const int i, int flag) {
    bool pk_com_per[48];                //存放压缩置换后的密钥
    //bool pk_ex_per[56];                 //存放扩展置换后的密钥
    bool RPT_ex[48];                    //困放扩展后的密文
    bool RPT_s[32]; //存放32位的RPT经过s-盒后的操作结果
    bool p_per[32]; //存放经过p盒处理后的32位数据
    get_pk_by_rough(Lpk, Rpk, i, flag);
    compress_permutation(pk_com_per, Lpk, Rpk);
    permutation(RPT_ex, RPT,ex_per,48);
    xor_s_swap(RPT_s, pk_com_per, RPT_ex);
    permutation(p_per, RPT_s,p_p,32);
    xor_and_permutation(LPT, RPT, p_per);
}

void DES::En_cryption() {
    bool temp[64];                      //存放初始置换后的明文
    bool LPT[32], RPT[32];              //左右明文
    bool pk[64];
    bool Lpk[28], Rpk[28];
    assign_val(pk, mpk, 64);
    permutation(temp, mpt, IP,64);
    toGet_L_Rpt(temp, LPT, RPT);
    get_pk_L_R(mpk, Lpk, Rpk);
    for (int i = 0; i < 16; i++)
    {
        rough_Function(LPT, RPT, Lpk, Rpk, i, 1);
        //print_ch("Rpk:",Rpk,28);
    }
    merge_L_RPT(temp, LPT, RPT);
    permutation(en_mpt, temp, FP,64);
    BitToByte(en_mpt_byte, en_mpt, 64);
}


void DES::De_cryption() {
    bool temp[64];                      //存放初始置换后的明文
    bool LPT[32], RPT[32];              //左右明文
    bool Lpk[28], Rpk[28];
    permutation(temp, en_mpt, IP,64);
    toGet_L_Rpt(temp, LPT, RPT);
    get_pk_L_R(mpk, Lpk, Rpk);
    for (int i = 15; i >= 0; i--)
    {
        rough_Function(RPT, LPT, Lpk, Rpk, i + 1, -1);
        //print_ch("Rpk:",Rpk,28);
    }
    merge_L_RPT(temp, LPT, RPT);
    permutation(de_mpt, temp, FP,64);
    BitToByte(de_mpt_byte, de_mpt, 64);
}

void DES::test() {
    //bool *mpt=new bool [64]; //存放明文
    /*
    mpt={
                    1,0,1,0,1,0,1,0,
                    1,0,1,0,1,0,1,0,
                    1,0,1,0,1,0,1,0,
                    1,0,1,0,1,0,1,0,
                    1,0,1,0,1,0,1,0,
                    1,0,1,0,1,0,1,0,
                    1,0,1,0,1,0,1,0,
                    1,0,1,0,1,0,1,0
                    };
                    */

    /*mpk={ 1,0,1,0,1,0,1,0,
            1,0,1,0,1,0,1,0,
            1,0,1,0,1,0,1,0,
            1,0,1,0,1,0,1,0,
            1,0,1,0,1,0,1,0,
            1,0,1,0,1,0,1,0,
            1,0,1,0,1,0,1,0,
            1,0,1,0,1,0,1,0};*/
    init_mpt();
    init_mpk();
    print_mpt_byte();
    //print_mpt();
    En_cryption();
    print_en_mpt_byte();
    //print_enmpt();
    De_cryption();
    print_de_mpt_byte();
    //print_dempt();

}





