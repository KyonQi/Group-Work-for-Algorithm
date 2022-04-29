#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<string.h>
#include<iostream>
#include<Windows.h>

//UIģ��
typedef struct Button
{
	int x;
	int y;
	int xx;
	int yy;
	COLORREF color;
	char* buttonstr;
	void (*f)();
}*LPBTN;
//������װ
LPBTN createButton(int x, int y, int xx, int yy, COLORREF color, const char* buttonstr, void (*f)());
//���ư�ť
void drawButton(LPBTN button);
//����Ƿ��ڰ�ť��
int isInButton(LPBTN button, MOUSEMSG m);
//չʾ����ͼ
void ShowBackground();
//չʾ������Ϸ
void ShowSolitairegame();
//չʾ˫����Ϸ
void ShowDoublegame();
//չʾ����
void ShowIntroduction();
//չʾ�˳���Ϸ
void ShowClose();
/*---------�������ǰ������ɫ------------*/
//�������ж�
void buttonAction(LPBTN button, MOUSEMSG m);

int UI_init();

//����ʳ��ģ��

#define window_height 600 
#define window_width 800 //���ڸ߶�480 ���640
enum dir //�ߵķ���
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};
struct Point //����ṹ��
{
    int x;
    int y;
};
struct Pt //�������Ա�
{
    Point point;
    Pt* next;
    Pt* pre;
};
class Snake
{
private:
    int id; //˵���ߵ���� ����˫��ģʽ
    dir direction = RIGHT;//˵���ߵ�Ŀǰͷ�ĳ���
    int score;//��ǰ�÷�
    int size = 10;//�ߵĳߴ�
    Pt* position;//��ͷ������ָ�룬���ڱ��������ߵ���Ϣ
    int color = YELLOW;
    int num;//��¼�ߵĽ��� ��ʼʱΪ3��
public:
    Snake(int ID, int COLOR );//���캯�� ��ʼ�������Ϣ
    void Snake_Init(int x,int y);//��ʼ������ ��������λ�á����򡢵÷ֳ�ʼ�� 
    void Snake_draw();//�����ߵ�ȫ���ڵ�
    Point* Snake_head();//������ͷ����ṹ��ָ��
    void Snake_Add(int n);//�ߵ�����n ͨ��˫��������head�ڵ�ĺ�̽ڵ��������½ڵ�
    void Snake_move();//�ߵ��ƶ� ��ͷ����direction�����ƶ�һ�񣬺����Ľڵ㶼�Ƶ�����ǰһ�ڵ��λ��
    bool Eat_Myself();//�ж��Ƿ�ͷ���Լ���ĳ���ڵ��غ�
    bool Eat_Another(Snake& another);//�ж��Լ���ͷ�Ƿ�ײ����һ���ߵ����壬�Ƿ���true �񷵻�False
    void Snake_Dir_change(dir newdirection);//�����ߵ��ƶ��������
    int Get_score();//��õ�ǰ�÷�
    void Score_add(int i);//���ӵ÷�
    dir Get_direction() { return direction; }//ȡ��ǰ����
    bool Knock_wall();//�ж��Ƿ�ײǽ
};

enum buff //buff���� 
{
    NO,//��buff
    EXTRA_SCORE,//��������
    Lengthen,//����3��
    SPEED_UP,//����

};
class Food
{
private:
    Point food_pt;//ʳ������
    buff buff_info;//buff����
    DWORD food_color;//ʳ����ɫ����
    int size = 10;//ʳ��Ĵ�СӦ������һ��
    int delay = 100;//ˢ�¼������
    int score = 10;//�Ե�ʳ��ļӷ�
public:
    Food();//���캯�� ��ʼ��ʳ�����ꡢbuff���͡���ɫ
    void ColorRandom();   //�趨�����ɫ 
    void BuffRandom();    //buff�������
    void Food_draw();     //����Food����
    bool EatorNot(Snake& snake);//�жϵ�����ͷ�Ƿ���ʳ�������غ� ���򷵻�true ����false
    void Food_pos_random();     //�������ʳ��λ��
    int Get_delay();            //��ȡ��ǰˢ�¼��
    void Change_delay(int i);   //�ı�ˢ�¼��
    buff Get_buff();            //��ȡbuff����
    int Get_Score();            //��ȡ��ǰ�ӷ�ֵ
    void Change_Score(int i);   //�ı�ӷ�ֵ
};
                            

//----------����ģ��-----------
//���̿��ƣ�
//����ģʽ�� WASD�ƶ�
//˫��ģʽ�� WASD�ƶ� ���������ƶ�
void keycontrol_Snake(Snake& s1);  
void keycontrol_Snake(Snake& s1, Snake& s2);

//---------��Ϸ����-----------
//����ģ�飬����ģʽ��˫��ģʽ����
void draw_snake(Snake s1, Food f);
void draw_snake(Snake s1, Snake s2, Food f);
//����ģ�飺����ģʽ��˫��ģʽ����
HWND single_snake();
HWND double_snake();
void start(); //��ʼ��Ϸ

