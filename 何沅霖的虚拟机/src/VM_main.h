#include "program.h"
/*��ʵ�� */
void VM_BOX::d_info(int op,int index){
    std::string R[reg_c] = {"axi","bxi","cxi","dxi","exi","fxi","gxi","hxi"};
    std::string I[inst_c] = {"HEL","PUT","MOV","ADD","SUB","DIV","MUL","XOR","OR","AND","NOT","JEP","JNEP","JGP","JGEP","JLP","JLEP","JMP","OUT"};
    if (op == _reg){
        reg_order += std::to_string((int)reg[index]);
        reg_order += "���뵽";
        reg_order += R[index];
        reg_order += ",";
    }
    else if (op == _inst) {
        if (index != HEL) {
            inst_order += I[index];
            inst_order += "->";
        }
        else
            inst_order += I[index];
    }
    else if (op == _out && debug) {
        std::cout << "ģ��ָ�����˳��:" << inst_order << std::endl;
        std::cout << "��������:" << reg_order << std::endl;
    }
} //debug��Ϣ

void VM_BOX::initialization(int f_debug){
    m_address = 0;
    flag = 1;
    r_address = 0;
    debug = f_debug;
} //��ʼ�������

void VM_BOX::load_to_memory(){
    std::copy(std::begin(program),std::end(program),std::begin(memory));
} //���س���ģ���ڴ�

void VM_BOX::boot(){
    while (flag){
        instruct(memory[m_address++]);
    }
} //����ָ�����

void VM_BOX::instruct(int op){
    switch (op) {
        case HEL: {
            d_info(_inst, HEL);
            flag = 0;
            std::cout << "���" << std::endl;
            d_info(_out);
            break;
        }
        case PUT: {
            d_info(_inst, PUT);
            if (memory[m_address] == D_OUT) {
                if (memory[m_address + 1] == _char) {
                    char t;
                    std::cin >> t;
                    reg[r_address] = t;
                    d_info(_reg, r_address);
                    ++r_address;
                }
                else if (memory[m_address + 1] == _digit){
                    int t;
                    std::cin >> t;
                    reg[r_address] = t;
                    d_info(_reg, r_address);
                    ++r_address;
                }
            }
            if (memory[m_address] == D_IN) {
                reg[r_address] = memory[m_address + 1];
                d_info(_reg, r_address);
                ++r_address;
            }
            m_address += 2;
            break;
        }
        case MOV:{
            d_info(_inst, MOV);
            reg[memory[m_address]] = reg[memory[m_address]] + reg[memory[m_address+1]];
            d_info(_reg,memory[m_address]);
            reg[memory[m_address+1]] = reg[memory[m_address]] - reg[memory[m_address+1]];
            d_info(_reg,memory[m_address+1]);
            reg[memory[m_address]] = reg[memory[m_address]] - reg[memory[m_address]+1];
            d_info(_reg,memory[m_address]);
            m_address += 2;
            break;
        }
        case ADD:{
            d_info(_inst, ADD);
            reg[axi] = reg[axi] + reg[bxi];
            d_info(_reg,axi);
            break;
        }
        case SUB:{
            d_info(_inst, SUB);
            reg[axi] = reg[axi] - reg[bxi];
            d_info(_reg,axi);
            break;
        }
        case DIV:{
            d_info(_inst, DIV);
            if (reg[bxi]){
                reg[axi] = reg[axi] / reg[bxi];
                d_info(_reg,axi);
                break;
            }
            else{
                std::cout << "��������Ϊ0";
                instruct(HEL);
                break;
            }
        }
        case MUL:{
            d_info(_inst, MUL);
            reg[axi] = reg[axi] * reg[bxi];
            d_info(_reg,axi);
            break;
        }
        case XOR:{
            d_info(_inst, XOR);
            reg[axi] = (int)reg[axi] ^ (int)reg[bxi];
            d_info(_reg,axi);
            break;
        }
        case OR:{
            d_info(_inst, OR);
            reg[axi] = (int)reg[axi] | (int)reg[bxi];
            d_info(_reg,axi);
            break;
        }
        case AND:{
            d_info(_inst, AND);
            reg[axi] = (int)reg[axi] & (int)reg[bxi];
            d_info(_reg,axi);
            break;
        }
        case NOT:{
            d_info(_inst, NOT);
            reg[axi] = ~(int)reg[axi];
            d_info(_reg,axi);
            break;
        }
        case JEP:{
            d_info(_inst, JEP);
            if (memory[m_address+1] == reg[memory[m_address]]) {
                m_address += 3;
                break;
            }
            else {
                m_address = m_address + memory[m_address+2] + 1;
                break;
            }
        }
        case JNEP:{
            d_info(_inst, JNEP);
            if (memory[m_address+1] != reg[memory[m_address]]) {
                m_address += 3;
                break;
            }
            else {
                m_address = m_address + memory[m_address+2] + 1;
                break;
            }
        }
        case JGP:{
            d_info(_inst, JGP);
            if ( reg[memory[m_address]] > memory[m_address+1])  {
                m_address += 3;
                break;
            }
            else {
                m_address = m_address + memory[m_address+2] + 1;
                break;
            }
        }
        case JGEP:{
            d_info(_inst, JGEP);
            if (reg[memory[m_address]] >= memory[m_address+1]) {
                m_address += 3;
                break;
            }
            else {
                m_address = m_address + memory[m_address+2] + 1;
                break;
            }
        }
        case JLP:{
            d_info(_inst, JLP);
            if (reg[memory[m_address]] < memory[m_address+1]) {
                m_address += 3;
                break;
            }
            else {
                m_address = m_address + memory[m_address+2] + 1;
                break;
            }
        }
        case JLEP:{
            d_info(_inst, JLEP);
            if (reg[memory[m_address]] <= memory[m_address+1]) {
                m_address += 3;
                break;
            }
            else {
                m_address = m_address + memory[m_address+2] + 1;
                break;
            }
        }
        case JMP:{
            d_info(_inst, JMP);
            if (memory[m_address] > 0)
                m_address += memory[m_address] - 1;
            if (memory[m_address] < 0)
                m_address += memory[m_address] + 1;
            break;
        }
        case OUT:{
            d_info(_inst, OUT);
            if (memory[m_address+1] == _char)
                std::cout << (char)reg[memory[m_address]] << std::endl;
            if (memory[m_address+1] == _digit)
                std::cout << reg[memory[m_address]] << std::endl;
            break;
        }
    }
} // ִ��ָ��