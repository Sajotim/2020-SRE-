#pragma once
using namespace std;
int program[16];
/*
int decToBin(int dec) { 
	int result = 0, temp = dec, j = 1;	
	while (temp) { 
		result = result + j * (temp % 2);		
		temp = temp / 2;		
		j = j * 10; 
	}	
	return result; 
}
*/
//INSTRUCTION TABLE
const int LOAD_A  =  0b0010;  //  2
const int LOAD_B  =  0b0001;  //  1
const int STORE_A =  0b0100;  //  4
const int ADD     =  0b1000;  //  8
const int SUB     =  0b1100;  // 12
const int MULTI   =  0b1110;  // 14
const int DEVIDE  =  0b1111;  // 15

//ָ��Ĵ�����ǰ��λΪ�����룬����λΪRAM/register��ַ
struct instruction_register {
	int opcode;
	int address;
};

//�����Ĵ���
typedef enum {
	A,B,C,D,E,
	NUM_OF_RGT
}RGT;
/*  
 *  �޸ļĴ�����ֵ
 *  int RGT[NUM_OF_RGT];
 *  RGT[A] = 2;
 */

class VirtualMachine {
private:
	int RAM[16];                      //�ڴ�
	instruction_register IR;          //ָ��Ĵ���
	int instruction_address_register; //ָ���ַ�Ĵ���
	int ALU;
public:
	void Fetch(int *a);               //��ȡָ��
	void Decode_Execute();            //ָ������&&ִ��ָ��
	void printf();                    //��ӡָ��
};
void VirtualMachine::Fetch(int *a) {
	cout << "\n��ʼ����ָ��" << endl;
	
	for (int i = 0; i < 16 ; i++) {
		
		RAM[i] = a[i]; 
	}

	cout << "�������\n" << endl;
}
void VirtualMachine::printf() {
	cout << "��ӡ�������ָ��" << endl;
	for (int i = 0; i < 16 && RAM[i] != 0; i++) {
		if (i < 10) cout << "0" << i << "\t";
		else cout << i << "\t";
		cout << RAM[i] / 1000 << RAM[i] / 100 % 10 << RAM[i] % 100 / 10 << RAM[i] % 100 % 10 << endl;
	}
	cout << "��ӡ���\n" << endl;
}

void VirtualMachine::Decode_Execute() {
	cout << "��ʼ����" << endl;
	int RGT[NUM_OF_RGT];
	for (int i = 0; i < 16 && RAM[i] != 0; i++) {
		IR.opcode = RAM[i]/100;
		IR.address = RAM[i] % 100;
		int register_former = IR.address / 10;
		int register_latter = IR.address % 10;
//		cout << IR.opcode << "\t" << IR.address<<"\t";
		switch (IR.opcode)
		{
		case(LOAD_A):
			RGT[A] = RAM[IR.address];
			cout << RGT[A] <<endl;
			break;
		case(LOAD_B):
			RGT[B] = RAM[IR.address];
			cout << RGT[B] << endl;
			break;
		case(STORE_A):
			RAM[IR.address] = RGT[A];
			cout << RAM[IR.address] <<endl;
			break;
		case(ADD):    	
			RGT[register_latter] += RGT[register_former];
			cout << RGT[A] << endl;
			break;
		case(SUB):
			RGT[register_latter] -= RGT[register_former];
			cout << RGT[B] << endl;
			break;
		case(MULTI):
			RGT[register_latter] *= RGT[register_former];
			cout << RGT[B] << endl;
			break;
		case(DEVIDE):
			RGT[register_latter] /= RGT[register_former];
			cout << RGT[B] << endl;
			break;
		default:
			break;
		}
	}
	cout << "���н���" << endl;
}
int* InputData() {
	cout << "ָ�����exit�Խ���" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> program[i];
		if (!cin.good())   //�ж��Ƿ���������Ϊint���������˳�ѭ��
			break;
	}
	return program;
}