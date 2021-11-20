#include<iostream>
#include<cmath>
#include<string>



//压缩变换矩阵
const int com_per[]={
                14,17,11,24,1,5,3,28,15,6,21,10,
                23,19,12,4,26,8,16,7,27,20,13,2,
                41,52,31,37,47,55,30,40,51,45,33,48,
                44,49,39,56,34,53,46,42,50,36,29,32};
    //扩展置换矩阵   
const int ex_per[]={  32,1,2,3,4,5,4,5,6,7,8,9,
                    8,9,10,11,12,13,12,13,14,15,16,17,
                    16,17,18,19,20,21,20,21,22,23,24,25,
                    24,25,26,27,28,29,28,29,30,31,32,1};
    //p置换矩阵
const int p_p[]={
                16,7,20,21,29,12,28,17,
                1,15,23,26,5,18,31,10,
                2,8,24,14,32,27,3,9,
                19,13,30,6,22,11,4,25};
    //S盒矩阵
const int SSS[8][4][16]={
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
//输出数组
void print_ch(std::string name, bool *ch,int length){
    int i=0;
    std::cout<<name;
    while(i<length){
        std::cout<<ch[i++];
    }
    std::cout<<std::endl;
}

//赋值
void assign_val(bool *output,const bool *input,int length){
    for(int i=0;i<length;i++){
        output[i]=input[i];
    }
}

//字节转为位
void ByteToBit(bool *DatOut,char *DatIn,int Num){
    for(int i=0;i<Num;i++){
        DatOut[i]=(DatIn[i/8]>>(i%8))&0x01;   
    }                                       
}

//位转为字节
void BitToByte(char *DatOut,bool *DatIn,int Num){
     for(int i=0;i<(Num/8);i++)
         DatOut[i]=0;
     for(int i=0;i<Num;i++)
         DatOut[i/8]|=DatIn[i]<<(i%8);
}

//二进制转为十六进制
void BitToHex(char *DatOut,bool *DatIn,int Num)
 {
     int i=0;
     for(i=0;i<Num/4;i++)
     {
         DatOut[i]=0;
     }
     for(i=0;i<Num/4;i++)
     {
         DatOut[i] = DatIn[i*4]+(DatIn[i*4+1]<<1)
                     +(DatIn[i*4+2]<<2)+(DatIn[i*4+3]<<3);
         if((DatOut[i]%16)>9)
         {
             DatOut[i]=DatOut[i]%16+'7';       //  余数大于9时处理 10-15 to A-F
         }                                     //  输出字符 
         else
         {
             DatOut[i]=DatOut[i]%16+'0';       //  输出字符       
         }
     }
     
 }
 

//十六进制字符转二进制
 void HexToBit(bool *DatOut,char *DatIn,int Num)
 {
     int i=0;                        // 字符型输入 
     for(i=0;i<Num;i++)
     {
         if((DatIn[i/4])>'9')         //  大于9 
        {
             DatOut[i]=((DatIn[i/4]-'7')>>(i%4))&0x01;               
         }
         else
         {
             DatOut[i]=((DatIn[i/4]-'0')>>(i%4))&0x01;     
         } 
     }    
 }

//明文初始化
void init_ch(bool *ch,int flag){
    int select;
    std::cin>>select;
    if(flag==1)
    std::cout<<"请输入需要加密的明文(按‘*’结束)"<<std::endl;
    if(flag==0)
    std::cout<<"请输入加密密钥(按‘*’结束)"<<std::endl;
    if(select==0){
        std::cout<<"正在退出"<<std::endl;
        system("pause");
        exit(0);
    }else if(select==1){
        bool input[64];
        char c;
        int i=0;
        std::cin>>c;
        while(i<64&&c!='*'){
            input[i++]=c-'0';
            std::cin>>c;
        }
        if(i>0&&i<64){
            input[i++]=0;
        }
        assign_val(ch,input,64);
    }else if(select==2){
        char input[16];
        char c;
        int i=0;
        std::cin>>c;
        while(i<16&&c!='*'){
            input[i++]=c;
            std::cin>>c;
        }
        if(i>0&&i<16){
            input[i++]='0';
        }
        HexToBit(ch,input,64);
    }else if(select==3){
        char input[8];
        char c;
        int i=0;
        std::cin>>c;
        while(i<8&&c!='*'){
            input[i++]=c;
            std::cin>>c;
        }
        if(i>0&&i<8){
            input[i++]='0';
        }
        ByteToBit(ch,input,64);
    }
}


//切割密钥
void get_pk_L_R(const bool * pk,bool *Lpk,bool *Rpk){
    int r=0;
    for(int i=0;i<32;i++){
        if((i+1)%8!=0)
            Lpk[i-r]=pk[i];
        else{
            r++;
        }
    }
    r=0;
    for(int i=32;i<64;i++){
        if((i+1)%8!=0)
            Rpk[i-32-r]=pk[i];
        else{
            r++;
        }
    }
}


void reverse(bool *a,int s,int e)
{
    int i,t;
    for(i=s;i<=(e+s)/2;i++)
    {
        t = a[i];
        a[i]=a[e+s-i];
        a[e+s-i]=t;
    }
}

int ArrayShift(bool *a,int n,int m)
{
    m = m%n;
    reverse(a,0,n-1); //全部逆序
    reverse(a,0,m-1); //逆序前m个
    reverse(a,m,n-1); //逆序后面的
    return 0;
}

void get_pk_one_rough(bool * pk,int rough,int flag){
    if(flag==1){
        int m=2;
        if((rough+1)==1||(rough+1)==2||(rough+1)==9||(rough+1)==16){
            m=1;     
        }
        ArrayShift(pk,28,28-m);
    }else{
        if(rough+1==17)
            return;
        int m=2;
        if((rough+1)==1||(rough+1)==2||(rough+1)==9||(rough+1)==16){
            m=1;     
        }
        ArrayShift(pk,28,m);
    }
}

//获取第i轮密钥
void get_pk_by_rough(bool * Lpk,bool *Rpk,const int rough,int flag){
    get_pk_one_rough(Lpk,rough,flag);
    get_pk_one_rough(Rpk,rough,flag);
}

//压缩置换
void compress_permutation(bool* temp,const bool*Lpki,const bool*Rpki){
    bool pki[56];
    for(int i=0;i<28;i++){
        pki[i]=Lpki[i];
    }
    for(int i=28;i<56;i++){
        pki[i]=Rpki[i-28];
    }
    for(int i=0;i<48;i++){
        temp[i]=pki[com_per[i]-1];
    }
}

//扩展置换
void expand_permutation(bool* pt_ex_per,const bool *RPT){
    for(int i=0;i<48;i++){
        pt_ex_per[i]=RPT[ex_per[i]-1];
    }
}

//S-盒替换
void xor_s_swap(bool* RPT_s, const bool* pk_com_per,const bool* RPT_ex){
    bool temp[48];
    bool num;
    for(int i=0;i<48;i++){
        num=pk_com_per[i];
        num^=RPT_ex[i];
        temp[i]=num;
    }
    for(int i=0;i<8;i++){
        int num1=0,num2=0;
        {
            num1=temp[i*6];
            num1=num1*2+temp[6*i+5];
            num2=temp[6*i+1];
            num2=num2*2+temp[6*i+2];
            num2=num2*2+temp[6*i+3];
            num2=num2*2+temp[6*i+4];
        }
        num=SSS[i][num1][num2];
        for(int j=4*i+3;j>=4*i;j--){
            if(num==0){
                RPT_s[j]=0;
            }else{
                RPT_s[j]=num%2;
                num/=2;
            }
        }
    }
}

//置换
void p_permutation(bool * p_per,const bool *RPT_s){
    for(int i=0;i<32;i++){
        p_per[i]=RPT_s[p_p[i]-1];
    }
}

//异或与交换
void xor_and_permutation(bool *LPT,bool *RPT,const bool *p_per){
    bool temp;
    for(int i=0;i<32;i++){
        temp=RPT[i];
        RPT[i]=LPT[i]^p_per[i];
        LPT[i]=temp;
    }
}

//置换
void permutation(bool *ret,const bool *ch,const int *p){
    for(int i=0;i<64;i++){
        ret[i]=ch[p[i]-1];
    }
}

//获取左右PT
void toGet_L_Rpt(const bool *temp,bool *LPT,bool *RPT){
    for(int i=0;i<32;i++){
        LPT[i]=temp[i];
    }
    for(int i=32;i<64;i++){
        RPT[i-32]=temp[i];
    }
}

void merge_L_RPT(bool *temp,const bool *LPT,const bool *RPT){
    for(int i=0;i<32;i++){
        temp[i]=LPT[i];
    }
    for(int i=32;i<64;i++){
        temp[i]=RPT[i-32];
    }
}

void rough_Function(bool * LPT,bool *RPT,bool *Lpk,bool *Rpk,const int i,int flag){
    bool pk_com_per[48];                //存放压缩置换后的密钥
    bool pk_ex_per[56];                 //存放扩展置换后的密钥
    bool RPT_ex[48];                    //困放扩展后的密文
    bool RPT_s[32]; //存放32位的RPT经过s-盒后的操作结果
    bool p_per[32]; //存放经过p盒处理后的32位数据
    get_pk_by_rough(Lpk,Rpk,i,flag);
    compress_permutation(pk_com_per,Lpk,Rpk);
    expand_permutation(RPT_ex,RPT);
    xor_s_swap(RPT_s,pk_com_per,RPT_ex); 
    p_permutation(p_per,RPT_s);
    xor_and_permutation(LPT,RPT,p_per);
}

void En_cryption(bool * mpt,bool * mpk,bool *Lpk,bool *Rpk,int *IP,int*FP){
    bool temp[64];                      //存放初始置换后的明文
    bool LPT[32],RPT[32];              //左右明文
    bool pk[64];
    assign_val(pk,mpk,64);
    permutation(temp,mpt,IP);
    toGet_L_Rpt(temp,LPT,RPT);
    get_pk_L_R(mpk,Lpk,Rpk);
    for(int i=0;i<16;i++)
    {
        rough_Function(LPT,RPT,Lpk,Rpk,i,1);
        //print_ch("Rpk:",Rpk,28);
    }
    merge_L_RPT(temp,LPT,RPT);
    permutation(mpt,temp,FP);
}

void De_cryption(bool * mpt,bool *Lpk,bool *Rpk,int *IP,int*FP){
    bool temp[64];                      //存放初始置换后的明文
    bool LPT[32],RPT[32];              //左右明文
    permutation(temp,mpt,IP);
    toGet_L_Rpt(temp,LPT,RPT);
    for(int i=15;i>=0;i--)
    {
        rough_Function(RPT,LPT,Lpk,Rpk,i+1,-1);
        //print_ch("Rpk:",Rpk,28);
    }
    merge_L_RPT(temp,LPT,RPT);
    permutation(mpt,temp,FP);
}

void test(){
    //bool *mpt=new bool [64]; //存放明文
    bool mpt[64]={0};
                        /*
    bool mpt[]={
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
    int IP[]={58,50,42,34,26,18,10,2,
                60,52,44,36,28,20,12,4,
                62,54,46,38,30,22,14,6,
                64,56,48,40,32,24,16,8,
                57,49,41,33,25,17,9,1,
                59,51,43,35,27,19,11,3,
                61,53,45,37,29,21,13,5,
                63,55,47,39,31,23,15,7};  //初始置换表
    int FP[]={40,8,48,16,56,24,64,32,
                39,7,47,15,55,23,63,31,
                38,6,46,14,54,22,62,30,
                37,5,45,13,53,21,61,29,
                36,4,44,12,52,20,60,28,
                35,3,43,11,51,19,59,27,
                34,2,42,10,50,18,58,26,
                33,1,41,9,49,17,57,25};      //最终置换表
    //bool *pk=new bool[64];    //存放指定密钥
    /*bool mpk[]={ 1,0,1,0,1,0,1,0,
                        1,0,1,0,1,0,1,0,
                        1,0,1,0,1,0,1,0,
                        1,0,1,0,1,0,1,0,
                        1,0,1,0,1,0,1,0,
                        1,0,1,0,1,0,1,0,
                        1,0,1,0,1,0,1,0,
                        1,0,1,0,1,0,1,0};*/
    bool mpk[64]={0};
    std::cout<<"请选择输入的进制(1：2进制,2：16进制,3：字节)，可按‘0’退出:"<<std::endl;      
    init_ch(mpt,1);
    std::cout<<"请选择输入的进制(1：2进制,2：16进制,3：字节)，可按‘0’退出:"<<std::endl;
    init_ch(mpk,0);
    bool Lpk[28],Rpk[28];              //存放左右密钥
    print_ch("mpt:",mpt,64);
    En_cryption(mpt,mpk,Lpk,Rpk,IP,FP);
    print_ch("mpt:",mpt,64);
    De_cryption(mpt,Lpk,Rpk,IP,FP);
    print_ch("mpt:",mpt,64);
}




int main(){
    test();
    return 0;
}
