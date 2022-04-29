#include"Tanchishe.hpp"
using namespace std;
#undef UNICODE
#undef _UNICODE
//开始游戏
void start()
{
    UI_init();
}

//UI模块
LPBTN createButton(int x, int y, int xx, int yy, COLORREF color, const char* buttonstr, void (*f)())
{
	LPBTN button = new Button[1];
	button->x = x;
	button->y = y;
	button->xx = xx;
	button->yy = yy;
	button->color = color;
	button->buttonstr = new char(*buttonstr);
	strcpy_s(button->buttonstr, strlen(buttonstr) + 1, buttonstr);
	button->f = f;
	return button;
}
//绘制按钮
void drawButton(LPBTN button)
{
	setfillcolor(button->color);//设置当前设备填充颜色
	fillrectangle(button->x, button->y, button->xx, button->yy);
	setbkmode(TRANSPARENT); //设置透明背景
	setlinecolor(RGB(0,0,0));//设置画线颜色
    settextstyle(30, 0, "黑体"); //设置图形文本当前字体、字符大小
	outtextxy(button->x + 15, button->y + 10, button->buttonstr);//输出图形文本字符串
}
//鼠标是否在按钮上
int isInButton(LPBTN button, MOUSEMSG m)
{
	if (button->x <= m.x && button->xx >= m.x && button->y <= m.y && button->yy >= m.y)
	{
		return 1;
	}
	return 0;
}
//展示背景图
void ShowBackground()
{
	initgraph(800, 600);
	IMAGE picture;
	loadimage(&picture, "background.jpg", 800, 600);
	putimage(0, 0, &picture);
}
//展示单人游戏
void ShowSolitairegame()
{
	cout << "单人游戏" << endl;

	HWND window;
	while (1)
	{

		window = single_snake();
       
		
		break;
	}
    ShowBackground();
}
//展示双人游戏
void ShowDoublegame()
{
	cout << "双人游戏" << endl;

	HWND window;
	while (1)
	{

		window = double_snake();

		break;
	}
    ShowBackground();
}
//展示游戏介绍
void ShowIntroduction1()
{
    MOUSEMSG m;
    LPBTN backkey = createButton(590, 530, 750, 580, RGB(152, 98, 89), "右键返回", ShowSolitairegame);

    initgraph(800, 600);
    IMAGE pictureintro;
    loadimage(&pictureintro, "Introduction1.jpg", 800, 600);
    putimage(0, 0, &pictureintro);

    while (1)
    {
        BeginBatchDraw();
        drawButton(backkey);
        m = GetMouseMsg();
        if (m.uMsg == WM_RBUTTONDOWN)//右键返回
        {
            EndBatchDraw();
            break;
        }
        else EndBatchDraw();
    }
    ShowBackground();
}
//展示团队介绍
void ShowIntroduction()
{
	MOUSEMSG m;

	initgraph(800, 600);
	IMAGE pictureintro;
	loadimage(&pictureintro, "Introduction0419.jpg", 800, 600);
	putimage(0, 0, &pictureintro);
	while (1)
	{
		BeginBatchDraw();
		m = GetMouseMsg();
		if (m.uMsg == WM_RBUTTONDOWN)//右键返回
		{
			EndBatchDraw();
			break;
		}
		else EndBatchDraw();
	}
	ShowBackground();
}
//展示退出游戏
void ShowClose() { exit(0); }
/*---------鼠标作用前呈现颜色------------*/
//鼠标左键判断
void buttonAction(LPBTN button, MOUSEMSG m)
{
	if (isInButton(button, m))
	{
		button->color = RGB(198, 150, 125);
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			closegraph();
			button->f();
		}
	}
	else button->color = RGB(152, 98, 89);
}

int UI_init()
{
	//调用绘制按钮函数
	LPBTN solitairegame = createButton(470, 240, 620, 290, RGB(152, 98, 89), "单人游戏", ShowSolitairegame);
	LPBTN doublegame = createButton(470, 310, 620, 360, RGB(152, 98, 89), "双人游戏", ShowDoublegame);
    LPBTN introduction1 = createButton(470, 380, 620, 430, RGB(152, 98, 89), "游戏介绍", ShowIntroduction1);
	LPBTN introduction = createButton(470, 450, 620, 500, RGB(152, 98, 89), "团队介绍", ShowIntroduction);
	LPBTN close = createButton(470, 520, 620, 570, RGB(152, 98, 89), "退出游戏", ShowClose);
	ShowBackground();

	MOUSEMSG m;
	while (1)
	{
		BeginBatchDraw();//批量绘图，消除屏幕闪烁

/*-----------绘制按钮样式----------*/
		drawButton(solitairegame);
		drawButton(doublegame);
        drawButton(introduction1);
		drawButton(introduction);
		drawButton(close);

		m = GetMouseMsg();//获取鼠标信息
/*---------鼠标作用前面呈现颜色------------*/
		buttonAction(solitairegame, m);
		buttonAction(doublegame, m);
        buttonAction(introduction1, m);
		buttonAction(introduction, m);
		buttonAction(close, m);

		EndBatchDraw();
	}
	closegraph();
	return 0;
}

//蛇与食物
Snake::Snake(int ID, int COLOR)//Snake类构造函数
{

    id = ID;
    color = COLOR;
    num = 3;
    score = 0;
    position = new Pt;

}
bool Snake::Eat_Another(Snake& another) //判断蛇是否撞到了另一条蛇
{
    Pt* p = another.position->next;//另一条蛇的头结点
    int i = another.num;//另一条蛇的长度
    while (i--)//从头到尾遍历
    {
        if (position->next->point.x == p->point.x && position->next->point.y == p->point.y)//本条蛇的头坐标是否会与另一条蛇的每一节结点坐标相等
        {
            return true;
            break;
        }
        else
        {
            p = p->next;//如果没有相碰就到下一个节点
        }
    }
    return false;
}
void Snake::Snake_Dir_change(dir newdirection) //改变蛇的移动方向
{
     direction = newdirection;//改变方向
}
void Snake::Snake_draw() //蛇的绘制函数 遍历线性表
{
   
    setlinecolor(BLACK);//设置边线颜色为黑
    setfillcolor(color);//设置填充颜色为黄
    int i = num;
    Pt* p = position->next;
    while (p!=NULL)//while (i--)
    {
        fillrectangle(p->point.x - size / 2, p->point.y - size / 2, p->point.x + size / 2, p->point.y + size / 2);//point1：矩形左上角点 point2：矩形右下角点
        p = p->next;
    }
}
bool Snake::Eat_Myself() //判断是否吃到了自己
{
    Pt* p = position->next->next;//该蛇的头结点
    int i = num - 1;
    while (p!=NULL)//从头到尾遍历
    {
        if (position->next->point.x == p->point.x && position->next->point.y == p->point.y)//本条蛇的头坐标是否会与另一条蛇的每一节结点坐标相等
        {
            return true;
            break;
        }
        else
        {
            p = p->next;//如果没有相碰就到下一个节点
        }
    }
    return false;
}
Point* Snake::Snake_head() //获取蛇头Point指针
{
    return &(position->next->point);
}

void Snake::Snake_Init(int x,int y)//初始化
{
    Pt *Head, *node1, *node2;
    Head = new Pt;
    node1 = new Pt;
    node2 = new Pt;
    position->next = Head;//蛇头指针指向头节点
    Head->next = node1;//头节点向后指向节点1
    Head->pre = NULL;//头节点向前指向空
    node1->next = node2;//节点1向后指向节点2
    node1->pre = Head;//节点1向前指向头节点
    node2->next = NULL;//节点2向后指向空
    node2->pre = node1;//节点2向前指向节点1
    Head->point.x = x;//蛇头的x，y坐标
    Head->point.y = y;
    node1->point.x = x-size;//蛇身1的x，y坐标
    node1->point.y = y;
    node2->point.x = x-2*size;//蛇身2的x，y坐标
    node2->point.y = y;

}
void Snake::Snake_move()
{
    
    Pt* p = position->next->next;
    int prex = position->next->point.x, prey = position->next->point.y;
    int px, py;
    while (p != NULL)
    {
        px = p->point.x;//存下本节点位置
        py = p->point.y;
        p->point.x = prex;//本节点位置移到上一节点
        p->point.y = prey;
        prex = px;//更新前一个节点位置为本节点位置
        prey = py;
        p = p->next;//本节点后移
    }
    switch (direction)//头节点根据方向移动    注：本部分被注释的代码代表了能否穿过墙，如果需要 请与Knock_Wall函数配合使用
    {
    case UP:
        position->next->point.y = position->next->point.y - 10;
        /*if (position->next->point.y < 5)//蛇头向上到边缘出现在最下方
            position->next->point.y = window_height-5;*/
        break;  //向上走一步
    case DOWN:
        position->next->point.y = position->next->point.y + 10;
        /*if (position->next->point.y > window_height - 5)//蛇头向下到边缘出现在最上方
            position->next->point.y = 0+5;*/
        break;  //向下走一步
    case LEFT:
        position->next->point.x = position->next->point.x - 10;
        /*if (position->next->point.x < 5)//蛇头向左到边缘出现在最右方
            position->next->point.x = window_width-5;*/
        break;  //向左走一步
    case RIGHT:
        position->next->point.x = position->next->point.x + 10;
        /*if (position->next->point.x > window_width - 5)//蛇头向右到边缘出现在最左方
            position->next->point.x = 5;*/
        break;  //向右走一步
    default:break;
    }
}
void Snake::Snake_Add(int n) //通过线性表的方式存储蛇身信息
{
    while (n--)//蛇身加n节，每次一节
    {
        Pt* node = new Pt;
        node->point.x = position->next->point.x;//新加节点的坐标为原头节点坐标
        node->point.y = position->next->point.y;
        node->next = position->next->next;//新节点next指向头节点的直接后继
        node->pre = position->next;//新节点pre指向头节点
        position->next->next->pre = node;//原头节点的直接后继向前指向新节点
        position->next->next = node;//头节点向后指向新节点
        switch (direction)//头节点根据方向移动
        {
        case UP:
            position->next->point.y = position->next->point.y - 10;
            if (position->next->point.y < 0)//蛇头向上到边缘出现在最下方
                position->next->point.y = window_height;
            break;  //向上走一步
        case DOWN:
            position->next->point.y = position->next->point.y + 10;
            if (position->next->point.y > window_height)//蛇头向下到边缘出现在最上方
                position->next->point.y = 0;
            break;  //向下走一步
        case LEFT:
            position->next->point.x = position->next->point.x - 10;
            if (position->next->point.x < 0)//蛇头向左到边缘出现在最右方
                position->next->point.x = window_width;
            break;  //向左走一步
        case RIGHT:
            position->next->point.x = position->next->point.x + 10;
            if (position->next->point.x > window_width)//蛇头向右到边缘出现在最左方
                position->next->point.x = 0;
            break;  //向右走一步
        default:break;
        }
        num++;
    }
}
int Snake::Get_score()//获取得分
{
    return score;
}
void Snake::Score_add(int i)//加分
{
    score += i;
}

bool Snake::Knock_wall()//判断是否撞墙
{
    switch (direction)//头节点根据方向移动
    {
    case UP:
        if (position->next->point.y < 0)//蛇头向上到边缘
            return true;
        break;  
    case DOWN:
        if (position->next->point.y > window_height)//蛇头向下到边缘
            return true;
        break;  
    case LEFT:
        if (position->next->point.x < 0)//蛇头向左到边缘
            return true;
        break;  
    case RIGHT:
        if (position->next->point.x > window_width)//蛇头向右到边缘
            return true;
        break;  
    default:break;
    }
    return false;
}

Food::Food() //Food类构造函数
{
    BuffRandom();
    ColorRandom();
    Food_draw();
    Food_pos_random();
}
void Food::ColorRandom() //Food随机生成颜色
{
    food_color = RGB(rand() % 256, rand() % 256, rand() % 256);
}
void Food::Food_pos_random() //Food随机生成坐标
{
    int x, y;//(x,y)为食物中心坐标
    x = rand() % 780 + 5;
    y = rand() % 591 + 5;
    
    food_pt.x = x;
    food_pt.y = y;
}
void Food::BuffRandom() {  //Food随机生成buff
    int j=0;//设置种子
    srand((unsigned)time(NULL));

    j = rand() % 5 + 1;  // 生成实际的随机数
    if (j == 1)  //j从【1,5】中产生随机数，加速概率25%对应数字1；2概率25%对应加长；3,4占50%代表无buff
    {
        buff_info = EXTRA_SCORE;
    }
    else if (j == 2)
    {
        buff_info = Lengthen;
    }
    else if (j==3 || j ==4)
    {
        buff_info = NO;
    }
    else if (j == 5)
    {
        buff_info = SPEED_UP;
    }
}
void Food::Food_draw() { //Food绘制函数

    setlinecolor(BLACK);//设置边线颜色为黑
    setfillcolor(food_color);//设置填充颜色为黄
    fillellipse(food_pt.x, food_pt.y, food_pt.x+ size, food_pt.y + size);//对应了椭圆上下左右四个顶点的位置 类型都是int

}
bool Food::EatorNot(Snake& snake) { //判断蛇是否吃到了食物 并依据buff类型做出反应
    int distance, dx, dy;
    Point *head = snake.Snake_head();
    dx = food_pt.x - head->x;
    dy = food_pt.y - head->y;
    distance = dx * dx + dy * dy;
    if (distance < 100) {  //吃到了食物
        if (buff_info == Lengthen) {   //将食物的buff赋予蛇
            snake.Snake_Add(3);
            score = 10;
            delay = 100;
        }
        else if (buff_info == NO) {
            
            score = 10;
            delay = 100;
        }
        else if (buff_info == EXTRA_SCORE)
        {
            score = 20;
            delay = 100;
        }
        else if (buff_info == SPEED_UP)
        {
            score = 10;
            delay = 50;
        }
        //加速食物buff----------需要填写
        //Food_draw();//重新生成一个食物
        //BuffRandom();//生成食物buff
        return true;
    }
    else { return false; }
}
int Food::Get_delay() //获取当前刷新间隔
{
    return delay;
}
void Food::Change_delay(int i) // 改变刷新间隔
{
    delay = i;
}
buff Food::Get_buff() { //获取buff类型
    return buff_info;
}
int Food::Get_Score()  // 获取当前吃到食物的加分额度
{
    return score;
}
void Food::Change_Score(int i) //改变吃到食物的加分额度
{
    score = i;
}

//控制模块
void keycontrol_Snake(Snake& s1)
{
    //使用win32API获取键盘信息

    if (GetAsyncKeyState('W') && s1.Get_direction() != DOWN)
    {//W
        s1.Snake_Dir_change(UP);
    }
    if (GetAsyncKeyState('S') && s1.Get_direction() != UP)
    {//S
        s1.Snake_Dir_change(DOWN);
    }
    if (GetAsyncKeyState('A') && s1.Get_direction() != RIGHT)
    {//A
        s1.Snake_Dir_change(LEFT);
    }
    if (GetAsyncKeyState('D') && s1.Get_direction() != LEFT)
    {//D
        s1.Snake_Dir_change(RIGHT);
    }
}

void keycontrol_Snake(Snake& s1, Snake& s2)
{
    //使用win32API获取键盘信息 蛇1的控制
    if (GetAsyncKeyState(0x57) && s1.Get_direction() != DOWN)
    {//W
        s1.Snake_Dir_change(UP);
    }
    if (GetAsyncKeyState(0x53) && s1.Get_direction() != UP)
    {//S
        s1.Snake_Dir_change(DOWN);
    }
    if (GetAsyncKeyState(0x41) && s1.Get_direction() != RIGHT)
    {//A
        s1.Snake_Dir_change(LEFT);
    }
    if (GetAsyncKeyState(0x44) && s1.Get_direction() != LEFT)
    {//D
        s1.Snake_Dir_change(RIGHT);
    }
    //蛇2的控制
    if (GetAsyncKeyState(VK_UP) && s2.Get_direction() != DOWN)
    {//上
        s2.Snake_Dir_change(UP);
    }
    if (GetAsyncKeyState(VK_DOWN) && s2.Get_direction() != UP)
    {//下
        s2.Snake_Dir_change(DOWN);
    }
    if (GetAsyncKeyState(VK_LEFT) && s2.Get_direction() != RIGHT)
    {//左
        s2.Snake_Dir_change(LEFT);
    }
    if (GetAsyncKeyState(VK_RIGHT) && s2.Get_direction() != LEFT)
    {//右
        s2.Snake_Dir_change(RIGHT);
    }

}


//----------计分板----------
//重载为单人模式和双人模式
void Scoreboard(Snake s1)
{
    char temp[50] = "";
    sprintf_s(temp, "分数：%d  ", s1.Get_score());//把后面的东西都写入到temp数组中
    settextcolor(RGB(0, 0, 0));
    setbkmode(TRANSPARENT);
    settextstyle(30, 15, "微软雅黑");
    outtextxy(40, 40, temp);
}
void Scoreboard(Snake s1,Snake s2)
{
    char temp[50] = "";
    char temp2[50] = "";
    sprintf_s(temp, "1号小蛇的分数：%d  ", s1.Get_score());//把后面的东西都写入到temp数组中
    sprintf_s(temp2, "2号小蛇的分数：%d  ", s2.Get_score());//把后面的东西都写入到temp数组中
    settextcolor(RGB(0, 0, 0));
    setbkmode(TRANSPARENT);
    settextstyle(30, 15, "微软雅黑");
    outtextxy(40, 40, temp);
    outtextxy(40, 80, temp2);
}


//----------蛇和食物的绘制函数----------
//重载为单人模式和双人模式
void draw_snake(Snake s1, Food f)
{
    setbkcolor(RGB(243,228,176));
    cleardevice();
    s1.Snake_draw();
    f.Food_draw();
    Scoreboard(s1);
    char temp[50] = "";
    switch (f.Get_buff())
    {
    case NO: 
        {
        sprintf_s(temp, "当前食物Buff：无Buff  ");//把后面的东西都写入到temp数组中
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "微软雅黑");
        outtextxy(250, 40, temp);
        break;
        }
    case EXTRA_SCORE:
        {
        sprintf_s(temp, "当前食物Buff：两倍分数  ");//把后面的东西都写入到temp数组中
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "微软雅黑");
        outtextxy(250, 40, temp);
        break;
        }
    case Lengthen:
    {
        sprintf_s(temp, "当前食物Buff：变得更长  ");//把后面的东西都写入到temp数组中
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "微软雅黑");
        outtextxy(250, 40, temp);
        break;
    }
    case SPEED_UP:
    {
        
        sprintf_s(temp, "当前食物Buff：加速  ");//把后面的东西都写入到temp数组中
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "微软雅黑");
        outtextxy(250, 40, temp);
        break;
        
    }
    }
    
    
}
void draw_snake(Snake s1, Snake s2, Food f)
{
    setbkcolor(RGB(243, 228, 176));
    cleardevice();
    s1.Snake_draw();
    s2.Snake_draw();
    f.Food_draw();
    Scoreboard(s1, s2);
    char temp[50] = "";
    switch (f.Get_buff())
    {
    case NO:
    {
        sprintf_s(temp, "当前食物Buff：无Buff  ");//把后面的东西都写入到temp数组中
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "微软雅黑");
        outtextxy(400, 40, temp);
        break;
    }
    case EXTRA_SCORE:
    {
        sprintf_s(temp, "当前食物Buff：两倍分数  ");//把后面的东西都写入到temp数组中
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "微软雅黑");
        outtextxy(400, 40, temp);
        break;
    }
    case Lengthen:
    {
        sprintf_s(temp, "当前食物Buff：变得更长  ");//把后面的东西都写入到temp数组中
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "微软雅黑");
        outtextxy(400, 40, temp);
        break;
    }
    case SPEED_UP:
    {

        sprintf_s(temp, "当前食物Buff：加速  ");//把后面的东西都写入到temp数组中
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "微软雅黑");
        outtextxy(400, 40, temp);
        break;

    }
    }
}


//----------游戏启动函数----------
//重载为单人模式和双人模式

//获取并返回窗口句柄，便于相应的窗体操作

HWND single_snake()
{
    HWND window;
    window = initgraph(window_width, window_height, 0);
    Snake s1(1,RGB(113,147,130));
    Food f;
    s1.Snake_Init(405,405);
    DWORD t1, t2;
    t1 = t2 = GetTickCount();
    BeginBatchDraw();//采用批量绘图的方式 也就是说 直到所有图都画完了才会一次性全都展示出来
 
    while (1)
    {
       
        if (t2 - t1 > f.Get_delay())
        {
            s1.Snake_move();
            t1 = t2;
        }
        t2 = GetTickCount();//设置刷新间隔
        draw_snake(s1, f);
        if (f.EatorNot(s1))
        {
            s1.Snake_Add(1);
            s1.Score_add(f.Get_Score());
            f.ColorRandom();
            f.Food_pos_random();
            f.BuffRandom();
            
            
        }
        if (s1.Knock_wall())
        {
            //游戏结束提示
            LPBTN lose = createButton(200, 275, 600, 325, RGB(152, 98, 89), "撞到墙啦，游戏结束~~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(lose);
                MOUSEMSG m;
                m = GetMouseMsg();//获取鼠标信息
                if (isInButton(lose, m))
                {
                    lose->color = RGB(198, 150, 125);
                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        closegraph();
                        break;
                    }
                }
                else lose->color = RGB(152, 98, 89);
                EndBatchDraw();
            }
            return window;
            break;
        }
        if (s1.Eat_Myself())
        {
            //游戏结束提示
            LPBTN lose = createButton(200, 275, 600, 325, RGB(152, 98, 89), "吃到自己啦，游戏结束~~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(lose);
                MOUSEMSG m;
                m = GetMouseMsg();//获取鼠标信息
                if (isInButton(lose, m))
                {
                    lose->color = RGB(198, 150, 125);
                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        closegraph();
                        break;
                    }
                }
                else lose->color = RGB(152, 98, 89);
                EndBatchDraw();
            }
            return window;
            break;
        }
        FlushBatchDraw();//展示批量绘图的结果
        keycontrol_Snake(s1);
    }
    getchar();
}


HWND double_snake()
{

    HWND window;
    window = initgraph(window_width, window_height, 0);
    Snake s1(1, RGB(113,147,130)), s2(2,RGB(220,151,112));
    Food f;
    s1.Snake_Init(405,405);
    s2.Snake_Init(305,305);
    DWORD t1, t2;
    t1 = t2 = GetTickCount();
    BeginBatchDraw();//采用批量绘图的方式 也就是说 直到所有图都画完了才会一次性全都展示出来
    while (1)
    {
        if (t2 - t1 > f.Get_delay())
        {
            s1.Snake_move();
            s2.Snake_move();
            t1 = t2;
        }
        t2 = GetTickCount();//设置刷新间隔
        draw_snake(s1, s2, f);
        if (f.EatorNot(s1))
        {
            s1.Snake_Add(1);
            f.ColorRandom();
            f.Food_pos_random();
            f.BuffRandom();
            s1.Score_add(f.Get_Score());
        }
        else if (f.EatorNot(s2))
        {
            s2.Snake_Add(1);
            f.ColorRandom();
            f.Food_pos_random();
            f.BuffRandom();
            s2.Score_add(f.Get_Score());
        }
        if (s1.Knock_wall())
        {
            //游戏结束提示
            LPBTN lose = createButton(200, 275, 600, 325, RGB(152, 98, 89), "小绿撞到墙啦，游戏结束~~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(lose);
                MOUSEMSG m;
                m = GetMouseMsg();//获取鼠标信息
                if (isInButton(lose, m))
                {
                    lose->color = RGB(198, 150, 125);
                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        closegraph();
                        break;
                    }
                }
                else lose->color = RGB(152, 98, 89);
                EndBatchDraw();
            }
            return window;
            break;
        }
        if (s2.Knock_wall())
        {
            //游戏结束提示
            LPBTN lose = createButton(200, 275, 600, 325, RGB(152, 98, 89), "小橙撞到墙啦，游戏结束~~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(lose);
                MOUSEMSG m;
                m = GetMouseMsg();//获取鼠标信息
                if (isInButton(lose, m))
                {
                    lose->color = RGB(198, 150, 125);
                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        closegraph();
                        break;
                    }
                }
                else lose->color = RGB(152, 98, 89);
                EndBatchDraw();
            }
            return window;
            break;
        }
        if (s1.Eat_Myself())//S1输了 游戏结束提示
        {
            LPBTN S11win = createButton(200, 275, 600, 325, RGB(152, 98, 89), "小绿咬到了自己，小橙胜利~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(S11win);
                MOUSEMSG m;
                m = GetMouseMsg();//获取鼠标信息
                if (isInButton(S11win, m))
                {
                    S11win->color = RGB(198, 150, 125);
                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        closegraph();
                        break;
                    }
                }
                else S11win->color = RGB(152, 98, 89);
                EndBatchDraw();
            }
            break;
        }
        else if (s2.Eat_Myself())
        {
            //S2输了 游戏结束提示
            LPBTN S22win = createButton(200, 275, 600, 325, RGB(152, 98, 89), "小橙咬到了自己，小绿胜利~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(S22win);
                MOUSEMSG m;
                m = GetMouseMsg();//获取鼠标信息
                if (isInButton(S22win, m))
                {
                    S22win->color = RGB(198, 150, 125);
                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        closegraph();
                        break;
                    }
                }
                else S22win->color = RGB(152, 98, 89);
                EndBatchDraw();
            }
          
            break;
        }
        FlushBatchDraw();//展示批量绘图的结果
        if (s1.Eat_Another(s2))
        {
            //S1 输了
            LPBTN S12win = createButton(200, 275, 600, 325, RGB(152, 98, 89), "小绿撞上了小橙，小橙胜利~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(S12win);
                MOUSEMSG m;
                m = GetMouseMsg();//获取鼠标信息
                if (isInButton(S12win, m))
                {
                    S12win->color = RGB(198, 150, 125);
                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        closegraph();
                        break;
                    }
                }
                else S12win->color = RGB(152, 98, 89);
                EndBatchDraw();
            }
            break;
        }
        else if (s2.Eat_Another(s1))
        {
            //S2 输了

            LPBTN S21win = createButton(200, 275, 600, 325, RGB(152, 98, 89), "小橙撞上了小绿，小绿胜利~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(S21win);
                MOUSEMSG m;
                m = GetMouseMsg();//获取鼠标信息
                if (isInButton(S21win, m))
                {
                    S21win->color = RGB(198, 150, 125);
                    if (m.uMsg == WM_LBUTTONDOWN)
                    {
                        closegraph();
                        break;
                    }
                }
                else S21win->color = RGB(152, 98, 89);
                EndBatchDraw();
            }
            break;
        }
        
        keycontrol_Snake(s1, s2);
    }

    return window;
}

