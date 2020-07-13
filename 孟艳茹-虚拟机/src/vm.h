#include <iostream>
#include<string>
#include <vector>
#include <stack>
using namespace std;
//ָ�
enum InstructCode
{
	Push,                // ��ջ
	Pop,                 // ��ջ
	Top,                 // ջ��
	Plus,                // ��
	Min,                 // ��
	Mul,                 // ��
	Div,                 // ��
	Mod,                 // ��
	Reboot,              // ����
	Exit                 // �˳�
};

// ��������
struct Data
{
	bool    isInt;
	int     intData;
	string  strData;

	Data() : isInt(true), intData(0) {}
	Data(int _n) : isInt(true), intData(_n) {}
	Data(const string& _s) : isInt(false), intData(0), strData(_s) {}
};

struct Instruct
{
	InstructCode insCode;
	int          operand;

	Instruct() {}
	Instruct(InstructCode _ic) : insCode(_ic) {}
	Instruct(InstructCode _ic, int _d) : insCode(_ic), operand(_d) {}
};

class VM  {
private:
	vector<Instruct> instruct;
	stack<Data> stk;
	vector<Data> data;

public:
	VM()
	{
		data.resize(101);
	}

	~VM()
	{
		Reset();
	}

	void Read();
	void Execute();
	void Reset();
	void Reboot();
	static void Exit();

private:
	static const int MAX_DEPTH = 1024;
	int mode, STA_top, Register{}, data_STA[MAX_DEPTH]{0};
	stack <int> program_STA;
	enum instr{
	Empty, Size, Push, Top, Pop, Add, Sub, Mul, Div, Mod, Reboot, Exit
	};
protected:
	bool Empty() const;
	int Size() const;
	int Push(int x);
	int Top();
	int Pop();
	void Plus();
	void Min();
	void Mul();
	bool Div();
	bool Mod();
	void Reboot();
	void Exit();
};