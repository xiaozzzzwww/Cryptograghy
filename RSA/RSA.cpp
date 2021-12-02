#include<iostream>
#include<gmpxx.h>

#include <cstdlib>
using namespace std;

mpz_class randbits(int bits) //base = 2
{
    gmp_randclass a(gmp_randinit_default);
    a.seed(rand());
    mpz_class l(bits);
    return a.get_z_bits(l);
}
 
inline mpz_class randprime(int bits)
{
    mpz_class a = randbits(bits);
    mpz_class ret;
    mpz_nextprime(ret.get_mpz_t(),a.get_mpz_t());
    return ret;
}
 
void setKey(mpz_class &n, mpz_class &e, mpz_class &d,const int nbits,int ebits=16)
{
    if(nbits/2<=ebits) {
         ebits = nbits/2;
    }
    mpz_class p = randprime(nbits/2);
    mpz_class q = randprime(nbits/2);
    n = q*p;
    mpz_class fn = (p-1)*(q-1);
    mpz_class gcd;
    do{
        e = randprime(ebits);
        mpz_gcd(gcd.get_mpz_t(),e.get_mpz_t(),fn.get_mpz_t());
    }while(gcd!=1);
    mpz_gcdext(p.get_mpz_t(),d.get_mpz_t(),q.get_mpz_t(),e.get_mpz_t(),fn.get_mpz_t());
}
 
inline mpz_class encrypt(const mpz_class &m,const mpz_class &e,const mpz_class &n)
{
    mpz_class ret;
    mpz_powm(ret.get_mpz_t(),m.get_mpz_t(),e.get_mpz_t(),n.get_mpz_t());
    return ret;
}
 
inline mpz_class decrypt(const mpz_class &c,const mpz_class &d,const mpz_class &n)
{
    return encrypt(c,d,n);
}

int main()
{
    int nbits;
    cout<<"输入大数比特数:";
    cin>>nbits;
    mpz_class n,e,d;
    setKey(n,e,d,nbits);
    cout<<"公钥:("<<e.get_str()<<", "<<n.get_str()<<")"<<endl;
    cout<<"私钥:("<<d.get_str()<<", "<<n.get_str()<<")"<<endl;
 
    cout<<"输入加密数据:";
    string s;
    cin>>s;
 
    mpz_class m(s);
    mpz_class c;
    c = encrypt(m,e,n);
    cout<<"加密后:"<<c.get_str()<<endl;
    c = decrypt(c,d,n);
    cout<<"解密后:"<<c.get_str()<<endl;
    if (c==m) cout<<"加/解密成功!"<<endl<<endl;
     else cout<<"加/解密失败!"<<endl<<endl;
 
    string q;
    cout<<"是否继续(Y/N):";
    cin>>q;
    if(q=="y"||q=="Y")
        main();
    return 0;
}
