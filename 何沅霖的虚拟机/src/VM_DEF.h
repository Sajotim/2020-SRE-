#include <iostream>
#include <string>
/*�ඨ�� */
enum {
    HEL = 0,
    PUT,
    MOV,
    ADD,
    SUB,
    DIV,
    MUL,
    XOR,
    OR,
    AND,
    NOT,
    JEP,
    JNEP,
    JGP,
    JGEP,
    JLP,
    JLEP,
    JMP,
    OUT,
    inst_c
}; //ָ�

enum {
    D_OUT ,
    D_IN ,
}; //������Դ��ʶ

enum {
    axi = 0,
    bxi,
    cxi,
    dxi,
    exi,
    fxi,
    gxi,
    hxi,
    reg_c
}; //�Ĵ���(д������)

enum {
    _char,
    _digit
}; //���������ʽ��־

enum {
    RUN = 1,
    DEBUG,
    _reg,
    _inst,
    _out,
}; //���������б�־

double reg[reg_c]; //ģ��Ĵ���

int memory[INT8_MAX]; //ģ���ڴ�

class VM_BOX {
private:
    int m_address;                      //ģ���ڴ��ַ
    int flag;                           //���б�־
    int r_address;                      //�Ĵ�����ַ
    int debug;                          //���Ա�־
    std::string inst_order, reg_order;  //ָ��˳��,��������
public:
    void d_info(int op,int index = -1);
    void initialization(int f_debug);
    void load_to_memory();
    void boot();
    void instruct(int op);
};