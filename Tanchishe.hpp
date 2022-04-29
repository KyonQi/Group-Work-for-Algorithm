#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<string.h>
#include<iostream>
#include<Windows.h>

//UI模块
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
//按键封装
LPBTN createButton(int x, int y, int xx, int yy, COLORREF color, const char* buttonstr, void (*f)());
//绘制按钮
void drawButton(LPBTN button);
//鼠标是否在按钮上
int isInButton(LPBTN button, MOUSEMSG m);
//展示背景图
void ShowBackground();
//展示单人游戏
void ShowSolitairegame();
//展示双人游戏
void ShowDoublegame();
//展示介绍
void ShowIntroduction();
//展示退出游戏
void ShowClose();
/*---------鼠标作用前呈现颜色------------*/
//鼠标左键判断
void buttonAction(LPBTN button, MOUSEMSG m);

int UI_init();

//蛇与食物模块

#define window_height 600 
#define window_width 800 //窗口高度480 宽度640
enum dir //蛇的方向
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};
struct Point //坐标结构体
{
    int x;
    int y;
};
struct Pt //坐标线性表
{
    Point point;
    Pt* next;
    Pt* pre;
};
class Snake
{
private:
    int id; //说明蛇的序号 用在双人模式
    dir direction = RIGHT;//说明蛇的目前头的朝向
    int score;//当前得分
    int size = 10;//蛇的尺寸
    Pt* position;//蛇头的坐标指针，用于遍历整条蛇的信息
    int color = YELLOW;
    int num;//记录蛇的节数 初始时为3节
public:
    Snake(int ID, int COLOR );//构造函数 初始化相关信息
    void Snake_Init(int x,int y);//初始化函数 将节数、位置、朝向、得分初始化 
    void Snake_draw();//绘制蛇的全部节点
    Point* Snake_head();//返回蛇头坐标结构体指针
    void Snake_Add(int n);//蛇的身长加n 通过双向链表，在head节点的后继节点上增加新节点
    void Snake_move();//蛇的移动 蛇头朝着direction方向移动一格，后续的节点都移到各自前一节点的位置
    bool Eat_Myself();//判断是否头和自己的某个节点重合
    bool Eat_Another(Snake& another);//判断自己的头是否撞到另一条蛇的身体，是返回true 否返回False
    void Snake_Dir_change(dir newdirection);//更改蛇的移动方向参数
    int Get_score();//获得当前得分
    void Score_add(int i);//增加得分
    dir Get_direction() { return direction; }//取当前朝向
    bool Knock_wall();//判断是否撞墙
};

enum buff //buff类型 
{
    NO,//无buff
    EXTRA_SCORE,//两倍分数
    Lengthen,//增长3节
    SPEED_UP,//加速

};
class Food
{
private:
    Point food_pt;//食物坐标
    buff buff_info;//buff类型
    DWORD food_color;//食物颜色参数
    int size = 10;//食物的大小应当和蛇一样
    int delay = 100;//刷新间隔参数
    int score = 10;//吃到食物的加分
public:
    Food();//构造函数 初始化食物坐标、buff类型、颜色
    void ColorRandom();   //设定随机颜色 
    void BuffRandom();    //buff类型随机
    void Food_draw();     //绘制Food函数
    bool EatorNot(Snake& snake);//判断的是蛇头是否与食物坐标重合 是则返回true 否则false
    void Food_pos_random();     //随机生成食物位置
    int Get_delay();            //获取当前刷新间隔
    void Change_delay(int i);   //改变刷新间隔
    buff Get_buff();            //获取buff类型
    int Get_Score();            //获取当前加分值
    void Change_Score(int i);   //改变加分值
};
                            

//----------控制模块-----------
//键盘控制：
//单人模式： WASD移动
//双人模式： WASD移动 上下左右移动
void keycontrol_Snake(Snake& s1);  
void keycontrol_Snake(Snake& s1, Snake& s2);

//---------游戏内容-----------
//绘制模块，单人模式、双人模式重载
void draw_snake(Snake s1, Food f);
void draw_snake(Snake s1, Snake s2, Food f);
//启动模块：单人模式、双人模式重载
HWND single_snake();
HWND double_snake();
void start(); //开始游戏

