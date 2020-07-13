#include <iostream>
#include <string.h>
#include "vm.h"
using namespace std;

bool VM::Empty() const{
	if (STA_top == -1) {
		return true;
	}
	return false;
}
int VM::Size() const{
	return STA_top + 1;
}
int VM::Push(int x){
		cout << "Push " << x << endl;
	if (STA_top > MAX_DEPTH){
		cout << "ջSTA��" << endl;
		return false;
	}
	else{
		STA_top++;
		data_STA[STA_top] = x;
		return true;
	}
}
int VM::Top(){
	if (Empty()){
		cout << "ջSTA�Ĵ�СΪ";
		return 0;
	}
	return data_STA[STA_top];
}
int VM::Pop(){
	if (STA_top >= 0){
		int top_val = data_STA[STA_top];
		data_STA[STA_top] = 0;
		STA_top--;
		if (mode == 2){
			cout << "POP\t\t" << top_val << endl;
		}
		return top_val;
	}
	else{
		cout << "ջSTA�Ĵ�СΪ";
		return 0;
	}

}
void VM::Plus(){
	Register = Pop();
		cout << "Plus\t" << Top() << "\t" << Register << endl;
	Push(Register + Pop());
}
void VM::Min(){
	Register = Pop();
		cout << "Min\t" << Top() << "\t" << Register << endl;
	Push(Register - Pop());
}
void VM::Mul(){
	Register = Pop();
		cout << "Mul\t" << Top() << "\t" << Register << endl;
	Push(Pop() * Register);
}
bool VM::Div(){
	Register = Pop();
	if (Register == 0){
		cout << "��������Ϊ��!" << endl;
		Pop();
		return false;
	}
	else{
			cout << "Div\t" << Top() << "\t" << Register << endl;
		Push(Register / Pop());
		return true;
	}
}
bool VM::Mod(){
	Register = Pop();
	if (Register == 0){
		cout << "��������Ϊ�㣡" << endl;
		Pop();
		return false;
	}
	else{
			cout << "Mod\t" << Top() << "\t" << Register << endl;
		Push(Pop() % Register);
		return true;
	}
}
// ��ȡ
void VM::Read()   {
	if (STA_top == -1){
		cout << "STA����Ϊ��" << endl;
	}
	else {
		cout << "No.\tVal" << endl;
		for (int i = STA_top; i >= 0; i--) {
			cout << i << "\t" << data_STA[i] << endl;
		}
	}
}

// ִ�г���
void VM::Execute()   {
	cout << "�����������" << endl;
	while (STA_top != -1) Pop();     // ���ջ
	cout << "��ѡ����Ҫ�Ĺ��� " << endl
		<< "0. �����" << endl
		<< "1. �����" << endl
		<< "2. �����" << endl
		<< "3. �����" << endl
		<< "4. ��������" << endl
		<< "5. ����" << endl
		<< "6. �˳�" << endl;
	cin >> mode;
	while (mode < 0 || mode > 6){
		cout << "��δ��ɣ�" << endl;
		cin >> mode;
		while (true){
			switch (mode){
			case 0:
				Plus();
				break;
			case 1:
				Min();
				break;
			case 2:
				Mul();
				break;
			case 3:
				Div();
				break;
			case 4:
				Mod();
				break;
			case 5:
				Reboot();
				break;
			case 6:
				Exit();
				break;
			}
		}
	}
}
void VM::Reboot(){
	cout << "������..." << endl;
	Execute();
}
void VM::Exit(){
	cout << "�˳���..." << endl;
	exit(0);
}
void VM::Reset(){
	data.clear();
	instruct.clear();
	while (!stk.empty())
	{
		stk.pop();
	}
	data.resize(101);

	cout << endl << "�������λ" << endl;
}