#include "VM_main.h"

void show(){
    std::cout<<"__     ____  __ ____   _____  __\n"
               "\\ \\   / /  \\/  | __ ) / _ \\ \\/ /\n"
               " \\ \\ / /| |\\/| |  _ \\| | | \\  / \n"
               "  \\ V / | |  | | |_) | |_| /  \\ \n"
               "   \\_/  |_|  |_|____/ \\___/_/\\_\\\n"
               "       1.����        2����   \n";

}

void run(int f_debug = false){
    VM_BOX start,*BOX;
    BOX = &start;
    BOX ->initialization(f_debug);
    BOX ->load_to_memory();
    BOX ->boot();
}

void choice(int cp){
    if (cp == RUN){
        std::cout << "�������У������������(֧�����򣬰�λ��򣬰�λ�룬��λ������)" << std::endl;;
        run();
    }
    else if (cp == DEBUG){
        std::cout << "�������У������������" << std::endl << "������Ϻ�����ָ�����˳�����������" << std::endl;;
        run(true);
    }
    else {
        std::cout << "�������" <<std::endl;
    }
}

int main() {
    show();
    int cp;
    std::cout << "��ѡ����:";
    std::cin >> cp;
    choice(cp);
    system("pause");
    return 0;
}
