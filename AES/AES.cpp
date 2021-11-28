// AES.cpp: 定义应用程序的入口点。


#include "AES.h"

using namespace std;


//获取子密钥
void AES::Get_Child_Key(uint8_t** input)
{
	uint8_t** temp = new uint8_t * [4];
	init_two_dim_array(temp, row_num, 2 * col_num);
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			temp[i][j] = input[i][j];
		}
	}
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			temp[i][j + col_num] = temp[i][j] ^ temp[i][j + col_num - 1];
		}
	}
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			input[i][j] = temp[i][j + 4];
		}
	}
	delete_two_dim_array(temp);
}

uint8_t** AES::GetChild_Key(uint8_t** input)
{
	uint8_t** temp = new uint8_t * [4];
	init_two_dim_array(temp, row_num, 2 * col_num);
	uint8_t** ret = new uint8_t * [4];
	init_two_dim_array(temp, row_num, col_num);
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			temp[i][j] = input[i][j];
		}
	}
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			temp[i][j + col_num] = temp[i][j] ^ temp[i][j + col_num - 1];
		}
	}
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			 ret[i][j] = temp[i][j + 4];
		}
	}
	delete_two_dim_array(temp);
	return ret;
}

//初始化子密钥
void AES::init_child_key(uint8_t **input)
{
	mpk_child = new uint8_t **[12];
	for(int i=0;i<12;i++)
	{
		mpk_child[i] = GetChild_Key(input);
	}
}

//获取解密的密钥
void AES::get_child_key(uint8_t** input)
{
	uint8_t** temp = new uint8_t * [4];
	init_two_dim_array(temp, row_num, 2 * col_num);
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			temp[i][j+col_num] = input[i][j];
		}
	}
	for (int i = 0; i < row_num; i++)
	{
		for (int j = col_num-1; j >=0; j--)
		{
			temp[i][j] = temp[i][j + col_num - 1] ^ temp[i][j+col_num];
		}
	}
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			input[i][j] = temp[i][j];
		}
	}
	delete_two_dim_array(temp);
}

//密钥加
void AES::Key_Add(uint8_t **output, uint8_t **input1, uint8_t **input2,int flag)
{	
	if (flag == 1)
		Get_Child_Key(input2);
	else
		get_child_key(input2);
	for (int i = 0; i < row_num; i++)
	{
		/*Xor_4bits(output[i], input1[i], input2[i]);*/
		for(int j=0;j<col_num;j++)
		{
			output[i][j] = input1[i][j] ^ input2[i][j];
		}
	}
	assign_two_dim_array(input1, output);
}

//初始化明文或者密钥
void AES::init_mpt_or_mpk(uint8_t *mp,int mp_length,int flag)
{
	if(flag==1)
	cout << "请输入需要加密的明文，按字节输入(最大十六字节)：";
	else
	{
		cout << "请输入密钥，按字节输入(最大十六字节)：";
	}
	//string ch;
	//getline(cin, ch);
	uint8_t temp;
	int i;
	for(int i=0;i<mp_length; i++)
	{
		
		cin >> temp;
		while (temp != '*'&& i < mp_length)
		{
			mp[i++] = temp;
			cin >> temp;
		}
		if(temp=='*')
		{
			real_mpt_length = i;
			i = mp_length + 1;
		}
		
	}
	cout << "开始填充" << endl;
	for (int j = real_mpt_length; j < mp_length; j++)
	{
		mp[j] = j;
	}
}

//s盒替换
void AES::s_permutation(uint8_t **output, uint8_t **input)
{
	for(int i=0;i<row_num;i++)
	{
		for(int j=0;j< col_num;j++)
		{
			uint8_t temp = input[i][j];
			output[i][j] = s_s[temp>>4][temp&0x0f];
		}
	}
}

//逆s盒替换
void AES::de_s_permutation(uint8_t** output, uint8_t** input)
{
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			uint8_t temp = input[i][j];
			output[i][j] = de_s_s[temp >> 4][temp & 0x0f];;
		}
	}
}

//行移位
void AES::en_row_permutation(uint8_t** output, uint8_t** input)
{
	for(int i=0;i<row_num;i++)
	{
		for(int j=0;j< col_num;j++)
		{
			output[i][j] = input[i][(j-i+col_num)%col_num];
		}
	}
}

void AES::de_row_permutation(uint8_t** output, uint8_t** input)
{
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			output[i][j] = input[i][(j+i + col_num) % col_num];
		}
	}
	assign_two_dim_array(input, output);
}

char AES::GaloisMultiplication(unsigned char Num_L, unsigned char Num_R)
{
	//定义变量
	unsigned char Result = 0;       //伽罗瓦域内乘法计算的结果

	while (Num_L)
	{
		//如果Num_L最低位是1就异或Num_R，相当于加上Num_R * 1
		if (Num_L & 0x01)
		{
			Result ^= Num_R;
		}

		//Num_L右移一位，相当于除以2
		Num_L = Num_L >> 1;

		//如果Num_R最高位为1
		if (Num_R & 0x80)
		{
			//左移一位相当于乘二
			Num_R = Num_R << 1;     //注：这里会丢失最高位，但是不用担心

			Num_R ^= 0x1B;  //计算伽罗瓦域内除法Num_R = Num_R / (x^8(刚好丢失最高位) + x^4 + x^3 + x^1 + 1)
		}
		else
		{
			//左移一位相当于乘二
			Num_R = Num_R << 1;
		}
	}
	return Result;
}

/**
 * 列混合
 */
void AES::en_col_mix(uint8_t **output,uint8_t**input)
{
	for(int i=0;i<row_num;i++)
	{
		for(int j=0;j<col_num;j++)
		{
			uint8_t temp = 0;
			for(int k=0;k<row_num;k++)
			{
				temp^=GaloisMultiplication(col_m[i][k],input[k][j]);
			}
			output[i][j] = temp;
		}
	}
}

void AES::de_col_mix(uint8_t** output, uint8_t** input)
{
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			uint8_t temp = 0;
			for (int k = 0; k < row_num; k++)
			{
				temp ^= GaloisMultiplication(de_col_m[i][k], input[k][j]);
			}
			output[i][j] = temp;
		}
	}

}

/**
 * 一维数组转成二维数组
 * 输出二维数组
 * 输入一维数组
 * 输入行数和列数
 */
void AES::one_to_two(uint8_t *output[4], uint8_t* input)
{
	for(int i=0;i<row_num;i++)
	{
		for(int j=0;j<col_num;j++)
		{
			output[i][j] = input[4*j+i];
		}
	}
}

/**
 * 二维数组转成一维数组
 * 输出一维数组
 * 输入行数和列数
 */
void AES::two_to_one(uint8_t* output, uint8_t **input)
{
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			output[4 * j + i] = input[i][j];
		}
	}
}

/**
 * 轮函数
 * 输出加密后的操作块
 * 输入需要加密的操作块和上一轮密钥
 * 输入行数列数
 */
void AES::rough_Function(uint8_t **output, uint8_t **input1, uint8_t **input2)
{
	uint8_t** mpt_temp = new uint8_t * [row_num];
	init_two_dim_array(mpt_temp,row_num,col_num);

	uint8_t** mpt_temp1 = new uint8_t * [row_num];
	init_two_dim_array(mpt_temp1, row_num, col_num);
	s_permutation(mpt_temp, input1);
	//print_two_dim_array("mpt", mpt_temp);

	en_row_permutation(mpt_temp1, mpt_temp);
	//print_two_dim_array("mpt", mpt_temp1);
	
	en_col_mix(mpt_temp, mpt_temp1);
	
	
	Key_Add(mpt_temp1,mpt_temp,input2,1);

	assign_two_dim_array(output, mpt_temp1);
	/*assign_two_dim_array(input1, mpt_temp1);*/

	delete_two_dim_array(mpt_temp);
	delete_two_dim_array(mpt_temp1);
}

/**
 * 轮函数
 * 输出加密后的操作块
 * 输入需要加密的操作块和上一轮密钥
 * 输入行数列数
 */
void AES::rough_function(uint8_t** output, uint8_t** input1, uint8_t** input2)
{
	uint8_t** mpt_temp = new uint8_t * [row_num];
	init_two_dim_array(mpt_temp, row_num, col_num);

	uint8_t** mpt_temp1 = new uint8_t * [row_num];
	init_two_dim_array(mpt_temp1, row_num, col_num);

	de_s_permutation(mpt_temp, input1);
	Key_Add(mpt_temp1, mpt_temp, input2, 0);
	
	//print_two_dim_array("mpk", input2);

	de_col_mix(mpt_temp, mpt_temp1);
	de_row_permutation(mpt_temp1, mpt_temp);

	assign_two_dim_array(output, mpt_temp1);
	assign_two_dim_array(input1, mpt_temp1);

	delete_two_dim_array(mpt_temp);
	delete_two_dim_array(mpt_temp1);
}

/**
 * 初始化二维数组
 * 输入需要初始化的数组及行数列数
 */
void AES::init_two_dim_array(uint8_t **input,int row_num,int col_num)
{
	for (int i = 0; i < row_num; i++)
	{
		input[i] = new uint8_t[col_num];
	}
	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < col_num; j++)
		{
			input[i][j] = 0x00;
		}
	}
}

/**
 * 删除二位数组
 *输入需要删除的数组及行数
 */
void AES::delete_two_dim_array(uint8_t** input)
{
	for(int i=0;i<row_num;i++)
	{
		delete []input[i];
	}
	delete[]input;
}


//打印二维数组
//输入需要打印的数组，行数，列数
void AES::print_two_dim_array(string name,uint8_t **input)
{
	cout <<name<<"二维矩阵的值为：" << endl;
	for(int i=0;i<row_num;i++)
	{
		for(int j = 0; j < col_num;j++)
		{
			printf("%02x ", input[i][j]);
		}
		cout << endl;
	}
}

void AES::print_mpt()
{
	cout << "明文为：";
	for(int i=0;i<real_mpt_length;i++)
	{
		//printf("%02x", mpt[i]);
		cout << mpt[i];
	}
	cout << endl;
}

void AES::print_en_mpt()
{
	cout << "密文为：";
	for (int i = 0; i <mpt_length; i++)
	{
		//printf("%02x", en_mpt[i]);
		cout << en_mpt[i];
	}
	cout << endl;
}

void AES::print_de_mpt()
{
	cout << "解密后为：";
	for (int i = 0; i < real_mpt_length; i++)
	{
		//printf("%02x", de_mpt[i]);
		cout << de_mpt[i];
	}
	cout << endl;
}


//输出一维数组
void AES::print_ch(string name,const uint8_t *ch)
{
	cout << name << "的值为：";
	for(int i=0;i<mpt_length;i++)
	{
		printf("%02x", ch[i]);
	}
	cout << endl;
}

//二维数组赋值
void AES::assign_two_dim_array(uint8_t **output,uint8_t **input)
{
	for(int i=0;i<row_num;i++)
	{
		for(int j=0;j<col_num;j++)
		{
			output[i][j] = input[i][j];
		}
	}
}

//加密
void AES::en_cryption()
{
	/*cout << "请输入明文长度";
	cin >> mpt_length;
	cout << "请输入密钥长度";
	cin >> mpk_length;*/

	mpt_length = 16;
	mpk_length = 16;
	cout << "加密程序开始" << endl;
	mpt = new uint8_t[mpt_length];
	mpk = new uint8_t[mpk_length];
	init_mpt_or_mpk(mpt, mpt_length,1);
	init_mpt_or_mpk(mpk, mpk_length,0);
	
	print_mpt();
	print_ch("mpt", mpt);
	col_num = mpk_length / row_num;
	uint8_t** mpt_temp = new uint8_t * [row_num];
	init_two_dim_array(mpt_temp, row_num, col_num);

	uint8_t** mpt_temp0 = new uint8_t * [row_num];
	init_two_dim_array(mpt_temp0, row_num, col_num);

	uint8_t** mpk_temp = new uint8_t * [row_num];
	init_two_dim_array(mpk_temp, row_num, col_num);

	one_to_two(mpt_temp, mpt);
	one_to_two(mpk_temp, mpk);

	Key_Add(mpt_temp0, mpt_temp, mpk_temp,1);
	

	for(int i=0;i<9;i++)
	{
		rough_Function(mpt_temp, mpt_temp0, mpk_temp);
		assign_two_dim_array(mpt_temp0, mpt_temp);
	}

	s_permutation(mpt_temp0, mpt_temp);

	en_row_permutation(mpt_temp, mpt_temp0);

	

	Key_Add(mpt_temp0, mpt_temp, mpk_temp,1);

	
	en_mpk = new uint8_t*[row_num];
	init_two_dim_array(en_mpk,row_num,col_num);
	assign_two_dim_array(en_mpk, mpk_temp);

	Get_Child_Key(en_mpk);
	en_mpt = new uint8_t[mpt_length];
	two_to_one(en_mpt, mpt_temp0);

	delete_two_dim_array(mpt_temp);
	delete_two_dim_array(mpt_temp0);
	delete_two_dim_array(mpk_temp);
}

void AES::de_cryption()
{
	uint8_t** mpt_temp = new uint8_t * [row_num];
	init_two_dim_array(mpt_temp, row_num, col_num);

	uint8_t** mpt_temp0 = new uint8_t * [row_num];
	init_two_dim_array(mpt_temp0, row_num, col_num);

	uint8_t** mpk_temp = new uint8_t * [row_num];
	init_two_dim_array(mpk_temp, row_num, col_num);

	one_to_two(mpt_temp, en_mpt);
	assign_two_dim_array(mpk_temp, en_mpk);


	Key_Add(mpt_temp0, mpt_temp, mpk_temp,0);
	
	de_row_permutation(mpt_temp, mpt_temp0);
	

	for (int i = 0; i < 9; i++)
	{
		rough_function(mpt_temp0, mpt_temp, mpk_temp);
		assign_two_dim_array(mpt_temp, mpt_temp0);
	}

	
	de_s_permutation(mpt_temp0,mpt_temp);
	
	Key_Add(mpt_temp, mpt_temp0,mpk_temp, 0);

	de_mpt = new uint8_t[mpt_length];
	two_to_one(de_mpt, mpt_temp0);

	delete_two_dim_array(mpt_temp);
	delete_two_dim_array(mpt_temp0);
	delete_two_dim_array(mpk_temp);
}


void AES::test01()
{
	en_cryption();
	//print_en_mpt();
	/*print_ch("加密后mpt", en_mpt);*/
	cout << endl;
	de_cryption();
	print_de_mpt();
	/*print_ch("解密后mpt", de_mpt);*/

	cout << endl;
	delete[]mpt;
	delete[]mpk;
	delete[]en_mpt;
	delete[]de_mpt;
	delete_two_dim_array(en_mpk);
}
