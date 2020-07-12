#include <cstdio>
#include <cstdlib>
#include <cstring>
#define R(s) Register[s&15]
#define M(s) Memory[trans_b2d(Register[s&15])&127]

using namespace std;

/*
����csappд��
ʵ���˶����Ʋ����������㣬ȡģ��λ����
ʵ�����������ƺ���������
Ȼ��������ֻ����cmp��test���ã���Ϊ����������������������
û�뵽��д��ô�������ȫ��һ���ļ�����
û��ô���ԣ�Ҳû������������bug��ܶ�
*/
struct Instruction
{
    int code, s1, s2;
    char t_s1, t_s2;
};

bool  Memory[128][32];          //�ڴ�
bool Register[16][32];          //�Ĵ���
Instruction Program[100];       //����
int IP;                         //ָ��Ĵ���
bool CF;                        //��λ��־(û�õ�)
bool ZF;                        //���־
bool SF;                        //���ű�־
bool OF;                        //�����־

const int ret = 0;              //�������
const int print = 1;            //��ӡʮ����ֵ
const int bprint = 2;           //��ӡ������ֵ
const int cmp = 3;              //�Ƚ�����������
const int jmp = 4;              //ֱ����ת
const int je = 5;               //���/����ת
const int jne = 6;              //�����/������ת
const int jg = 7;               //������ת
const int jge = 8;              //���ڵ�����ת
const int jl = 9;               //С����ת
const int jle = 10;             //С�ڵ�����ת
const int cmov = 11;            //ֱ�Ӵ���
const int cmove = 12;           //���/�㴫��
const int cmovne = 13;          //�����/���㴫��
const int cmovg = 14;           //���ڴ���
const int cmovge = 15;          //���ڵ��ڴ���
const int cmovl = 16;           //С�ڴ���
const int cmovle = 17;          //С�ڵ��ڴ���
const int neg = 18;             //ȡ��
const int add = 19;             //��
const int isub = 20;            //��
const int imul = 21;            //��
const int idiv = 22;            //������
const int mod = 23;             //ȡģ
const int bxor = 24;            //��λ���
const int bor = 25;             //��λ��
const int band = 26;            //��λ��
const int test = 27;            //��������������
const int sal = 28;             //����
const int sar = 29;             //��������
const int shr = 30;             //�߼�����

//�ֵ�������ƥ��ָ��
const int maxn = 5e5;
int trie[maxn][26], tot = 0;
int op[maxn];

void insert(char *a, int num)
{
    int root = 0, id, len = strlen(a);
    for(int i = 0; i < len; i++)
    {
        id = a[i]-'a';
        if(trie[root][id]==0) trie[root][id] = ++tot;
        root = trie[root][id];
    }
    op[root] = num;
}

int search(char *a)
{
    int root = 0, id, len = strlen(a);
    for(int i = 0; i < len; i++)
    {
        id = a[i]-'a';
        if(trie[root][id]==0) return -1;
        root = trie[root][id];
    }
    return op[root];
}

void trie_init()
{
    memset(trie, 0, sizeof(trie));
    memset(op, -1, sizeof(op));
    tot = 0;
    insert("ret", 0);
    insert("print", 1);
    insert("bprint", 2);
    insert("cmp", 3);
    insert("jmp", 4);
    insert("je", 5);
    insert("jne", 6);
    insert("jg", 7);
    insert("jge", 8);
    insert("jl", 9);
    insert("jle", 10);
    insert("cmov", 11);
    insert("cmove", 12);
    insert("cmovne",  13);
    insert("cmovg", 14);
    insert("cmovge", 15);
    insert("cmovl", 16);
    insert("cmovle", 17);
    insert("neg", 18);
    insert("add", 19);
    insert("isub", 20);
    insert("imul", 21);
    insert("idiv", 22);
    insert("mod", 23);
    insert("bxor", 24);
    insert("bor", 25);
    insert("band", 26);
    insert("test", 27);
    insert("sal", 28);
    insert("sar", 29);
    insert("shr", 30);
}
//�ֵ���ʵ�ֲ��ֽ���

//������ʮ����ת��
void trans_d2b(bool *p, int x)
{
    for(int i = 0; i < 32; i++) p[i] = x & (1<<i);
}

int trans_b2d(bool *p)
{
    int x = 0;
    for(int i = 0; i < 32; i++)
    {
        if(p[i]) x |= (1<<i);
    }
    return x;
}
//����ת�����ֽ���


void set_ccode_add(int a, int b)//����������
{
    int t = a+b;
    CF = (unsigned)t<(unsigned)a;
    ZF = t==0;
    SF = t<0;
    OF = ((a<0)==(b<0)) && ((t<0) != (a<0));
}

//ָ��ʵ��
void Print(int s, char t)
{
    if(t=='$') printf("%d\n", s);
    else if(t=='%') printf("%d\n", trans_b2d(R(s)));
    else if(t=='&') printf("%d\n", trans_b2d(M(s)));
}

void Bprint(int s, char t)
{
    if(t=='$') for(int i = 31; i >= 0; i--) printf("%d", s&(1<<i)?1:0);
    else if(t=='%') for(int i = 31; i >= 0; i--) printf("%d", R(s)[i]);
    else if(t=='&') for(int i = 31; i >= 0; i--) printf("%d", M(s)[i]);
    printf("\n");
}

void Cmp(int s1, int s2, char t_s1, char t_s2)
{
    int x, y;
    if(t_s1=='$') x = s1;
    else if(t_s1=='%') x = trans_b2d(R(s1));
    else if(t_s1=='&') x = trans_b2d(M(s1));
    if(t_s2=='$') y = s2;
    else if(t_s2=='%') y = trans_b2d(R(s2));
    else if(t_s1=='&') y = trans_b2d(M(s2));
    set_ccode_add(x, -y);
}

void Jmp(int x)
{

    if(x<0||x>=100||Program[x].code==-1)
    {
        printf("�������תλ��\n");
        exit(0);
    }
    IP = x-1;
}

void Je(int x){if(ZF) Jmp(x);}
void Jne(int x){if(!ZF) Jmp(x);}
void Jg(int x){if(!(SF!=OF)&&(!ZF)) Jmp(x);}
void Jge(int x){if(!(SF!=OF)) Jmp(x);}
void Jl(int x){if(SF!=OF) Jmp(x);}
void Jle(int x){if((SF!=OF)||ZF) Jmp(x);}

void Cmov(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻��Ϊ������\n");
        exit(0);
    }
    int x;
    if(t_s=='$') x = s;
    else if(t_s=='%') x = trans_b2d(R(s));
    else if(t_s=='&') x = trans_b2d(M(s));
    if(t_r=='%') trans_d2b(R(r), x);
    else if(t_r=='&') trans_d2b(M(r), x);
}

void Cmove(int s, int r, char t_s, char t_r){if(ZF) Cmov(s, r, t_s, t_r);}
void Cmovne(int s, int r, char t_s, char t_r){if(!ZF) Cmov(s, r, t_s, t_r);}
void Cmovg(int s, int r, char t_s, char t_r){if(!(SF!=OF)&&(!ZF)) Cmov(s, r, t_s, t_r);}
void Cmovge(int s, int r, char t_s, char t_r){if(!(SF!=OF)) Cmov(s, r, t_s, t_r);}
void Cmovl(int s, int r, char t_s, char t_r){if(SF!=OF) Cmov(s, r, t_s, t_r);}
void Cmovle(int s, int r, char t_s, char t_r){if((SF!=OF)||ZF) Cmov(s, r, t_s, t_r);}

void Inc(int s)
{
    int x = 1;
    for(int i = 0; i < 32; i++)
    {
        x += R(s)[i];
        R(s)[i] = x%2;
        x /= 2;
    }
}

void Neg(int s, int t)
{
    if(t=='%')
    {
        for(int i = 0; i < 32; i++) R(s)[i] = !R(s)[i];
        Inc(s);
    }
    else if(t=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
}

void Add(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = s;
        else if(t_s=='%') x = trans_b2d(R(s));
        else if(t_s=='&') x = trans_b2d(M(s));
        int temp = 0;
        for(int i = 0; i < 32; i++)
        {
            temp += (x&(1<<i)?1:0) + R(r)[i];
            R(r)[i] = temp%2;
            temp /= 2;
        }
    }
}

void Isub(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = -s;
        else if(t_s=='%') x = -trans_b2d(R(s));
        else if(t_s=='&') x = -trans_b2d(M(s));
        int temp = 0;
        for(int i = 0; i < 32; i++)
        {
            temp += (x&(1<<i)?1:0) + R(r)[i];
            R(r)[i] = temp%2;
            temp /= 2;
        }
    }
}

void Imul(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = s;
        else if(t_s=='%') x = trans_b2d(R(s));
        else if(t_s=='&') x = trans_b2d(M(s));
        int temp = 0;
        for(int i = 0; i < 32; i++)
        {
            if(R(r)[i]) temp += x<<i;
        }
        trans_d2b(R(r), temp);
    }
}

void Idiv(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = s;
        else if(t_s=='%') x = trans_b2d(R(s));
        else if(t_s=='&') x = trans_b2d(M(s));
        if(x==0)
        {
            printf("��������Ϊ0\n");
            exit(0);
        }
        int y = trans_b2d(R(r));
        int temp = y>=0?0:-1;
        for(int i = 31; i >= 0; i--)
        {
            temp = temp<<1;
            temp |= (y&(1<<31))?1:0;
            y = y<<1;
            int flag = temp&(1<<31);
            int re = temp;
            if(flag == (x&(1<<31))) temp = temp-x;
            else temp = temp+x;
            if((flag==(temp&(1<<31))) || (temp==0)) y |= 1;
            else
            {
                y &= -2;
                temp = re;
            }
        }
        trans_d2b(R(r), R(r)[31]==((x&(1<<31))?1:0)?y:-y);
    }
}

void Mod(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = s;
        else if(t_s=='%') x = trans_b2d(R(s));
        else if(t_s=='&') x = trans_b2d(M(s));
        if(x==0)
        {
            printf("��������Ϊ0\n");
            exit(0);
        }
        int y = trans_b2d(R(r));
        int temp = y>=0?0:-1;
        for(int i = 31; i >= 0; i--)
        {
            temp = temp<<1;
            temp |= (y&(1<<31))?1:0;
            y = y<<1;
            int flag = temp&(1<<31);
            int re = temp;
            if(flag == (x&(1<<31))) temp = temp-x;
            else temp = temp+x;
            if((flag==(temp&(1<<31))) || (temp==0)) y |= 1;
            else
            {
                y &= -2;
                temp = re;
            }
        }
        trans_d2b(R(r), temp);
    }
}

void Bxor(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = s;
        else if(t_s=='%') x = trans_b2d(R(s));
        else if(t_s=='&') x = trans_b2d(M(s));
        int y = trans_b2d(R(r));
        trans_d2b(R(r), x^y);
    }
}

void Bor(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = s;
        else if(t_s=='%') x = trans_b2d(R(s));
        else if(t_s=='&') x = trans_b2d(M(s));
        int y = trans_b2d(R(r));
        trans_d2b(R(r), x|y);
    }
}

void Band(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = s;
        else if(t_s=='%') x = trans_b2d(R(s));
        else if(t_s=='&') x = trans_b2d(M(s));
        int y = trans_b2d(R(r));
        trans_d2b(R(r), x&y);
    }
}

void Test(int s1, int s2, char t_s1, char t_s2)
{
    int x, y;
    if(t_s1=='$') x = s1;
    else if(t_s1=='%') x = trans_b2d(R(s1));
    else if(t_s1=='&') x = trans_b2d(M(s1));
    if(t_s2=='$') y = s2;
    else if(t_s2=='%') y = trans_b2d(R(s2));
    else if(t_s2=='&') y = trans_b2d(M(s2));
    ZF = (x&y)==0;
    CF=0;SF=0;OF=0;
}

void Sal(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = s;
        else if(t_s=='%') x = trans_b2d(R(s));
        else if(t_s=='&') x = trans_b2d(M(s));
        for(int i = 31; i >= 0; i--)
        {
            if(i-x>=0) R(r)[i] = R(r)[i-x];
            else R(r)[i] = 0;
        }
    }
}

void Sar(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = s;
        else if(t_s=='%') x = trans_b2d(R(s));
        else if(t_s=='&') x = trans_b2d(M(s));
        for(int i = 0; i < 32; i++)
        {
            if(i+x<32) R(r)[i] = R(r)[i+x];
            else R(r)[i] = R(r)[31];
        }
    }
}

void Shr(int s, int r, char t_s, char t_r)
{
    if(t_r=='$')
    {
        printf("Ŀ�겻����������\n");
        exit(0);
    }
    else if(t_r=='&')
    {
        printf("��������ֻ���ڼĴ����Ͻ���\n");
        exit(0);
    }
    else if(t_r=='%')
    {
        int x;
        if(t_s=='$') x = s;
        else if(t_s=='%') x = trans_b2d(R(s));
        else if(t_s=='&') x = trans_b2d(M(s));
        for(int i = 0; i < 32; i++)
        {
            if(i+x<32) R(r)[i] = R(r)[i+x];
            else R(r)[i] = 0;
        }
    }
}
//ָ��ʵ�ֲ��ֽ���


int Load()//ָ�����
{
    char p[10];
    scanf("%s", p);
    int code = search(p);
    int s, s1, s2;
    char t, t1, t2;
    switch(code)
    {
    case 1:
    case 2:
    case 18:
        while((t=getchar())==' ');
        if(t!='$'&&t!='%'&&t!='&') code = -1;
        scanf("%d", &s);
        Program[IP].s1 = s;
        Program[IP].t_s1 = t;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        scanf("%d", &s);
        Program[IP].s1 = s;
        break;
    case 3:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
        while((t1=getchar())==' ');
        if(t1!='$'&&t1!='%'&&t1!='&') code = -1;
        scanf("%d", &s1);
        while((t2=getchar())==' ');
        if(t1!='$'&&t1!='%'&&t1!='&') code = -1;
        scanf("%d", &s2);
        Program[IP].s1=s1;Program[IP].s2=s2;
        Program[IP].t_s1=t1;Program[IP].t_s2=t2;
        break;
    }
    Program[IP].code = code;
    return code;
}

int Run()//ָ��ִ��
{
    int code = Program[IP].code;
    int s, s1, s2;
    char t, t1, t2;
    switch(code)
    {
    case ret:
        printf("%02d\t%02d\n", IP, code);
        break;
    case print:
        s = Program[IP].s1;
        t = Program[IP].t_s1;
        printf("%02d\t%02d\t%c%d\n", IP, code, t, s);
        Print(s, t);
        break;
    case bprint:
        s = Program[IP].s1;
        t = Program[IP].t_s1;
        printf("%02d\t%02d\t%c%d\n", IP, code, t, s);
        Bprint(s, t);
        break;
    case jmp:
        s = Program[IP].s1;
        printf("%02d\t%02d\t%d\n", IP, code, s);
        Jmp(s);
        break;
    case je:
        s = Program[IP].s1;
        printf("%02d\t%02d\t%d\n", IP, code, s);
        Je(s);
        break;
    case jne:
        s = Program[IP].s1;
        printf("%02d\t%02d\t%d\n", IP, code, s);
        Jne(s);
        break;
    case jg:
        s = Program[IP].s1;
        printf("%02d\t%02d\t%d\n", IP, code, s);
        Jg(s);
        break;
    case jge:
        s = Program[IP].s1;
        printf("%02d\t%02d\t%d\n", IP, code, s);
        Jge(s);
        break;
    case jl:
        s = Program[IP].s1;
        printf("%02d\t%02d\t%d\n", IP, code, s);
        Jl(s);
        break;
    case jle:
        s = Program[IP].s1;
        printf("%02d\t%02d\t%d\n", IP, code, s);
        Jle(s);
        break;
    case neg:
        s = Program[IP].s1;
        t = Program[IP].t_s1;
        printf("%02d\t%02d\t%c%d\n", IP, code, t, s);
        Neg(s, t);
        break;
    case cmp:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Cmp(s1, s2, t1, t2);
        break;
    case cmov:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Cmov(s1, s2, t1, t2);
        break;
    case cmove:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Cmove(s1, s2, t1, t2);
        break;
    case cmovne:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Cmovne(s1, s2, t1, t2);
        break;
    case cmovg:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Cmovg(s1, s2, t1, t2);
        break;
    case cmovge:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Cmovge(s1, s2, t1, t2);
        break;
    case cmovl:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Cmovl(s1, s2, t1, t2);
        break;
    case cmovle:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Cmovle(s1, s2, t1, t2);
        break;
    case add:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Add(s1, s2, t1, t2);
        break;
    case isub:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Isub(s1, s2, t1, t2);
        break;
    case imul:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Imul(s1, s2, t1, t2);
        break;
    case idiv:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Idiv(s1, s2, t1, t2);
        break;
    case mod:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Mod(s1, s2, t1, t2);
        break;
    case bxor:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Bxor(s1, s2, t1, t2);
        break;
    case bor:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Bor(s1, s2, t1, t2);
        break;
    case band:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Band(s1, s2, t1, t2);
        break;
    case test:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Test(s1, s2, t1, t2);
        break;
    case sal:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Sal(s1, s2, t1, t2);
        break;
    case sar:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Sar(s1, s2, t1, t2);
        break;
    case shr:
        s1 = Program[IP].s1, s2 = Program[IP].s2;
        t1 = Program[IP].t_s1, t2 = Program[IP].t_s2;
        printf("%02d\t%02d\t%c%d, %c%d\n", IP, code, t1, s1, t2, s2);
        Shr(s1, s2, t1, t2);
        break;
    }
    return code;
}

void VirtualMachine_init()//�������ʼ��
{
    memset(Memory, 0, sizeof(Memory));
    memset(Register, 0, sizeof(Register));
    for(int i = 0; i < 100; i++) Program[i].code = -1;
    IP=0;CF=0;ZF=0;SF=0;OF=0;
}

void VirtualMachine_run()//���������
{
    printf("��������ڳ�ʼ��......\n");
    trie_init();
    VirtualMachine_init();
    printf("�����������\n");
    int c = -1;
    while(c==-1)
    {
        printf("����0�鿴ָ��,����1ȫ�������ִ��,����2��ִ��\n");
        if(scanf("%d", &c)!=1) break;;
        if(c==0)
        {
            printf("$��ʾ������,%��ʾ�Ĵ���,&��ʾ�Ĵ����е�ַָ����ڴ�\n");
            printf("�Ĵ�����ΧΪ0-15,�ڴ淶ΧΪ0-127������Ϊ32λ,û��Խ����\n");
            printf("�Ĵ���0��ֵΪ0 �ڴ�0��ֵΪ42 print &0������ڴ�0��ֵ42\n");
            printf("ret      \n");
            printf("printf   S\n");
            printf("bprint   S\n");
            printf("cmp      S1 S2\n");
            printf("jmp      S\n");
            printf("je       S\n");
            printf("jne      S\n");
            printf("jg       S\n");
            printf("jge      S\n");
            printf("jl       S\n");
            printf("jle      S\n");
            printf("cmov     S R\n");
            printf("cmove    S R\n");
            printf("cmovne   S R\n");
            printf("cmovg    S R\n");
            printf("cmovge   S R\n");
            printf("cmovl    S R\n");
            printf("cmovle   S R\n");
            printf("neg      S\n");
            printf("add      S R\n");
            printf("isub     S R\n");
            printf("imul     S R\n");
            printf("idiv     S R\n");
            printf("mod      S R\n");
            printf("bxor     S R\n");
            printf("bor      S R\n");
            printf("band     S R\n");
            printf("test     S1 S2\n");
            printf("sal      S R\n");
            printf("sar      S R\n");
            printf("shr      S R\n");
        }
        else if(c==1)
        {
            printf("��ʼ����\n");
            int flag;
            while(1)
            {
                flag = Load();
                if(flag == 0) break;
                if(flag == -1) printf("�������\n");
                else IP++;
            }
            IP = 0;
            while(1)
            {
                int flag = Run();
                if(flag==0) break;
                IP++;
            }
        }
        else if(c==2)
        {
            printf("��ʼ����\n");
            int flag;
            while(1)
            {
                flag = Load();
                while(flag==-1)
                {
                    printf("�������\n");
                    flag = Load();
                }
                while(Program[IP].code!=-1)
                {
                    flag = Run();
                    IP++;
                }
                if(flag==0) break;
            }
        }
    }
    printf("������ѹر�\n");
}



int main()
{
    VirtualMachine_run();
    return 0;
}



