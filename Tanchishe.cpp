#include"Tanchishe.hpp"
using namespace std;
#undef UNICODE
#undef _UNICODE
//��ʼ��Ϸ
void start()
{
    UI_init();
}

//UIģ��
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
//���ư�ť
void drawButton(LPBTN button)
{
	setfillcolor(button->color);//���õ�ǰ�豸�����ɫ
	fillrectangle(button->x, button->y, button->xx, button->yy);
	setbkmode(TRANSPARENT); //����͸������
	setlinecolor(RGB(0,0,0));//���û�����ɫ
    settextstyle(30, 0, "����"); //����ͼ���ı���ǰ���塢�ַ���С
	outtextxy(button->x + 15, button->y + 10, button->buttonstr);//���ͼ���ı��ַ���
}
//����Ƿ��ڰ�ť��
int isInButton(LPBTN button, MOUSEMSG m)
{
	if (button->x <= m.x && button->xx >= m.x && button->y <= m.y && button->yy >= m.y)
	{
		return 1;
	}
	return 0;
}
//չʾ����ͼ
void ShowBackground()
{
	initgraph(800, 600);
	IMAGE picture;
	loadimage(&picture, "background.jpg", 800, 600);
	putimage(0, 0, &picture);
}
//չʾ������Ϸ
void ShowSolitairegame()
{
	cout << "������Ϸ" << endl;

	HWND window;
	while (1)
	{

		window = single_snake();
       
		
		break;
	}
    ShowBackground();
}
//չʾ˫����Ϸ
void ShowDoublegame()
{
	cout << "˫����Ϸ" << endl;

	HWND window;
	while (1)
	{

		window = double_snake();

		break;
	}
    ShowBackground();
}
//չʾ��Ϸ����
void ShowIntroduction1()
{
    MOUSEMSG m;
    LPBTN backkey = createButton(590, 530, 750, 580, RGB(152, 98, 89), "�Ҽ�����", ShowSolitairegame);

    initgraph(800, 600);
    IMAGE pictureintro;
    loadimage(&pictureintro, "Introduction1.jpg", 800, 600);
    putimage(0, 0, &pictureintro);

    while (1)
    {
        BeginBatchDraw();
        drawButton(backkey);
        m = GetMouseMsg();
        if (m.uMsg == WM_RBUTTONDOWN)//�Ҽ�����
        {
            EndBatchDraw();
            break;
        }
        else EndBatchDraw();
    }
    ShowBackground();
}
//չʾ�Ŷӽ���
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
		if (m.uMsg == WM_RBUTTONDOWN)//�Ҽ�����
		{
			EndBatchDraw();
			break;
		}
		else EndBatchDraw();
	}
	ShowBackground();
}
//չʾ�˳���Ϸ
void ShowClose() { exit(0); }
/*---------�������ǰ������ɫ------------*/
//�������ж�
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
	//���û��ư�ť����
	LPBTN solitairegame = createButton(470, 240, 620, 290, RGB(152, 98, 89), "������Ϸ", ShowSolitairegame);
	LPBTN doublegame = createButton(470, 310, 620, 360, RGB(152, 98, 89), "˫����Ϸ", ShowDoublegame);
    LPBTN introduction1 = createButton(470, 380, 620, 430, RGB(152, 98, 89), "��Ϸ����", ShowIntroduction1);
	LPBTN introduction = createButton(470, 450, 620, 500, RGB(152, 98, 89), "�Ŷӽ���", ShowIntroduction);
	LPBTN close = createButton(470, 520, 620, 570, RGB(152, 98, 89), "�˳���Ϸ", ShowClose);
	ShowBackground();

	MOUSEMSG m;
	while (1)
	{
		BeginBatchDraw();//������ͼ��������Ļ��˸

/*-----------���ư�ť��ʽ----------*/
		drawButton(solitairegame);
		drawButton(doublegame);
        drawButton(introduction1);
		drawButton(introduction);
		drawButton(close);

		m = GetMouseMsg();//��ȡ�����Ϣ
/*---------�������ǰ�������ɫ------------*/
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

//����ʳ��
Snake::Snake(int ID, int COLOR)//Snake�๹�캯��
{

    id = ID;
    color = COLOR;
    num = 3;
    score = 0;
    position = new Pt;

}
bool Snake::Eat_Another(Snake& another) //�ж����Ƿ�ײ������һ����
{
    Pt* p = another.position->next;//��һ���ߵ�ͷ���
    int i = another.num;//��һ���ߵĳ���
    while (i--)//��ͷ��β����
    {
        if (position->next->point.x == p->point.x && position->next->point.y == p->point.y)//�����ߵ�ͷ�����Ƿ������һ���ߵ�ÿһ�ڽ���������
        {
            return true;
            break;
        }
        else
        {
            p = p->next;//���û�������͵���һ���ڵ�
        }
    }
    return false;
}
void Snake::Snake_Dir_change(dir newdirection) //�ı��ߵ��ƶ�����
{
     direction = newdirection;//�ı䷽��
}
void Snake::Snake_draw() //�ߵĻ��ƺ��� �������Ա�
{
   
    setlinecolor(BLACK);//���ñ�����ɫΪ��
    setfillcolor(color);//���������ɫΪ��
    int i = num;
    Pt* p = position->next;
    while (p!=NULL)//while (i--)
    {
        fillrectangle(p->point.x - size / 2, p->point.y - size / 2, p->point.x + size / 2, p->point.y + size / 2);//point1���������Ͻǵ� point2���������½ǵ�
        p = p->next;
    }
}
bool Snake::Eat_Myself() //�ж��Ƿ�Ե����Լ�
{
    Pt* p = position->next->next;//���ߵ�ͷ���
    int i = num - 1;
    while (p!=NULL)//��ͷ��β����
    {
        if (position->next->point.x == p->point.x && position->next->point.y == p->point.y)//�����ߵ�ͷ�����Ƿ������һ���ߵ�ÿһ�ڽ���������
        {
            return true;
            break;
        }
        else
        {
            p = p->next;//���û�������͵���һ���ڵ�
        }
    }
    return false;
}
Point* Snake::Snake_head() //��ȡ��ͷPointָ��
{
    return &(position->next->point);
}

void Snake::Snake_Init(int x,int y)//��ʼ��
{
    Pt *Head, *node1, *node2;
    Head = new Pt;
    node1 = new Pt;
    node2 = new Pt;
    position->next = Head;//��ͷָ��ָ��ͷ�ڵ�
    Head->next = node1;//ͷ�ڵ����ָ��ڵ�1
    Head->pre = NULL;//ͷ�ڵ���ǰָ���
    node1->next = node2;//�ڵ�1���ָ��ڵ�2
    node1->pre = Head;//�ڵ�1��ǰָ��ͷ�ڵ�
    node2->next = NULL;//�ڵ�2���ָ���
    node2->pre = node1;//�ڵ�2��ǰָ��ڵ�1
    Head->point.x = x;//��ͷ��x��y����
    Head->point.y = y;
    node1->point.x = x-size;//����1��x��y����
    node1->point.y = y;
    node2->point.x = x-2*size;//����2��x��y����
    node2->point.y = y;

}
void Snake::Snake_move()
{
    
    Pt* p = position->next->next;
    int prex = position->next->point.x, prey = position->next->point.y;
    int px, py;
    while (p != NULL)
    {
        px = p->point.x;//���±��ڵ�λ��
        py = p->point.y;
        p->point.x = prex;//���ڵ�λ���Ƶ���һ�ڵ�
        p->point.y = prey;
        prex = px;//����ǰһ���ڵ�λ��Ϊ���ڵ�λ��
        prey = py;
        p = p->next;//���ڵ����
    }
    switch (direction)//ͷ�ڵ���ݷ����ƶ�    ע�������ֱ�ע�͵Ĵ���������ܷ񴩹�ǽ�������Ҫ ����Knock_Wall�������ʹ��
    {
    case UP:
        position->next->point.y = position->next->point.y - 10;
        /*if (position->next->point.y < 5)//��ͷ���ϵ���Ե���������·�
            position->next->point.y = window_height-5;*/
        break;  //������һ��
    case DOWN:
        position->next->point.y = position->next->point.y + 10;
        /*if (position->next->point.y > window_height - 5)//��ͷ���µ���Ե���������Ϸ�
            position->next->point.y = 0+5;*/
        break;  //������һ��
    case LEFT:
        position->next->point.x = position->next->point.x - 10;
        /*if (position->next->point.x < 5)//��ͷ���󵽱�Ե���������ҷ�
            position->next->point.x = window_width-5;*/
        break;  //������һ��
    case RIGHT:
        position->next->point.x = position->next->point.x + 10;
        /*if (position->next->point.x > window_width - 5)//��ͷ���ҵ���Ե����������
            position->next->point.x = 5;*/
        break;  //������һ��
    default:break;
    }
}
void Snake::Snake_Add(int n) //ͨ�����Ա�ķ�ʽ�洢������Ϣ
{
    while (n--)//�����n�ڣ�ÿ��һ��
    {
        Pt* node = new Pt;
        node->point.x = position->next->point.x;//�¼ӽڵ������Ϊԭͷ�ڵ�����
        node->point.y = position->next->point.y;
        node->next = position->next->next;//�½ڵ�nextָ��ͷ�ڵ��ֱ�Ӻ��
        node->pre = position->next;//�½ڵ�preָ��ͷ�ڵ�
        position->next->next->pre = node;//ԭͷ�ڵ��ֱ�Ӻ����ǰָ���½ڵ�
        position->next->next = node;//ͷ�ڵ����ָ���½ڵ�
        switch (direction)//ͷ�ڵ���ݷ����ƶ�
        {
        case UP:
            position->next->point.y = position->next->point.y - 10;
            if (position->next->point.y < 0)//��ͷ���ϵ���Ե���������·�
                position->next->point.y = window_height;
            break;  //������һ��
        case DOWN:
            position->next->point.y = position->next->point.y + 10;
            if (position->next->point.y > window_height)//��ͷ���µ���Ե���������Ϸ�
                position->next->point.y = 0;
            break;  //������һ��
        case LEFT:
            position->next->point.x = position->next->point.x - 10;
            if (position->next->point.x < 0)//��ͷ���󵽱�Ե���������ҷ�
                position->next->point.x = window_width;
            break;  //������һ��
        case RIGHT:
            position->next->point.x = position->next->point.x + 10;
            if (position->next->point.x > window_width)//��ͷ���ҵ���Ե����������
                position->next->point.x = 0;
            break;  //������һ��
        default:break;
        }
        num++;
    }
}
int Snake::Get_score()//��ȡ�÷�
{
    return score;
}
void Snake::Score_add(int i)//�ӷ�
{
    score += i;
}

bool Snake::Knock_wall()//�ж��Ƿ�ײǽ
{
    switch (direction)//ͷ�ڵ���ݷ����ƶ�
    {
    case UP:
        if (position->next->point.y < 0)//��ͷ���ϵ���Ե
            return true;
        break;  
    case DOWN:
        if (position->next->point.y > window_height)//��ͷ���µ���Ե
            return true;
        break;  
    case LEFT:
        if (position->next->point.x < 0)//��ͷ���󵽱�Ե
            return true;
        break;  
    case RIGHT:
        if (position->next->point.x > window_width)//��ͷ���ҵ���Ե
            return true;
        break;  
    default:break;
    }
    return false;
}

Food::Food() //Food�๹�캯��
{
    BuffRandom();
    ColorRandom();
    Food_draw();
    Food_pos_random();
}
void Food::ColorRandom() //Food���������ɫ
{
    food_color = RGB(rand() % 256, rand() % 256, rand() % 256);
}
void Food::Food_pos_random() //Food�����������
{
    int x, y;//(x,y)Ϊʳ����������
    x = rand() % 780 + 5;
    y = rand() % 591 + 5;
    
    food_pt.x = x;
    food_pt.y = y;
}
void Food::BuffRandom() {  //Food�������buff
    int j=0;//��������
    srand((unsigned)time(NULL));

    j = rand() % 5 + 1;  // ����ʵ�ʵ������
    if (j == 1)  //j�ӡ�1,5���в�������������ٸ���25%��Ӧ����1��2����25%��Ӧ�ӳ���3,4ռ50%������buff
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
void Food::Food_draw() { //Food���ƺ���

    setlinecolor(BLACK);//���ñ�����ɫΪ��
    setfillcolor(food_color);//���������ɫΪ��
    fillellipse(food_pt.x, food_pt.y, food_pt.x+ size, food_pt.y + size);//��Ӧ����Բ���������ĸ������λ�� ���Ͷ���int

}
bool Food::EatorNot(Snake& snake) { //�ж����Ƿ�Ե���ʳ�� ������buff����������Ӧ
    int distance, dx, dy;
    Point *head = snake.Snake_head();
    dx = food_pt.x - head->x;
    dy = food_pt.y - head->y;
    distance = dx * dx + dy * dy;
    if (distance < 100) {  //�Ե���ʳ��
        if (buff_info == Lengthen) {   //��ʳ���buff������
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
        //����ʳ��buff----------��Ҫ��д
        //Food_draw();//��������һ��ʳ��
        //BuffRandom();//����ʳ��buff
        return true;
    }
    else { return false; }
}
int Food::Get_delay() //��ȡ��ǰˢ�¼��
{
    return delay;
}
void Food::Change_delay(int i) // �ı�ˢ�¼��
{
    delay = i;
}
buff Food::Get_buff() { //��ȡbuff����
    return buff_info;
}
int Food::Get_Score()  // ��ȡ��ǰ�Ե�ʳ��ļӷֶ��
{
    return score;
}
void Food::Change_Score(int i) //�ı�Ե�ʳ��ļӷֶ��
{
    score = i;
}

//����ģ��
void keycontrol_Snake(Snake& s1)
{
    //ʹ��win32API��ȡ������Ϣ

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
    //ʹ��win32API��ȡ������Ϣ ��1�Ŀ���
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
    //��2�Ŀ���
    if (GetAsyncKeyState(VK_UP) && s2.Get_direction() != DOWN)
    {//��
        s2.Snake_Dir_change(UP);
    }
    if (GetAsyncKeyState(VK_DOWN) && s2.Get_direction() != UP)
    {//��
        s2.Snake_Dir_change(DOWN);
    }
    if (GetAsyncKeyState(VK_LEFT) && s2.Get_direction() != RIGHT)
    {//��
        s2.Snake_Dir_change(LEFT);
    }
    if (GetAsyncKeyState(VK_RIGHT) && s2.Get_direction() != LEFT)
    {//��
        s2.Snake_Dir_change(RIGHT);
    }

}


//----------�Ʒְ�----------
//����Ϊ����ģʽ��˫��ģʽ
void Scoreboard(Snake s1)
{
    char temp[50] = "";
    sprintf_s(temp, "������%d  ", s1.Get_score());//�Ѻ���Ķ�����д�뵽temp������
    settextcolor(RGB(0, 0, 0));
    setbkmode(TRANSPARENT);
    settextstyle(30, 15, "΢���ź�");
    outtextxy(40, 40, temp);
}
void Scoreboard(Snake s1,Snake s2)
{
    char temp[50] = "";
    char temp2[50] = "";
    sprintf_s(temp, "1��С�ߵķ�����%d  ", s1.Get_score());//�Ѻ���Ķ�����д�뵽temp������
    sprintf_s(temp2, "2��С�ߵķ�����%d  ", s2.Get_score());//�Ѻ���Ķ�����д�뵽temp������
    settextcolor(RGB(0, 0, 0));
    setbkmode(TRANSPARENT);
    settextstyle(30, 15, "΢���ź�");
    outtextxy(40, 40, temp);
    outtextxy(40, 80, temp2);
}


//----------�ߺ�ʳ��Ļ��ƺ���----------
//����Ϊ����ģʽ��˫��ģʽ
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
        sprintf_s(temp, "��ǰʳ��Buff����Buff  ");//�Ѻ���Ķ�����д�뵽temp������
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "΢���ź�");
        outtextxy(250, 40, temp);
        break;
        }
    case EXTRA_SCORE:
        {
        sprintf_s(temp, "��ǰʳ��Buff����������  ");//�Ѻ���Ķ�����д�뵽temp������
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "΢���ź�");
        outtextxy(250, 40, temp);
        break;
        }
    case Lengthen:
    {
        sprintf_s(temp, "��ǰʳ��Buff����ø���  ");//�Ѻ���Ķ�����д�뵽temp������
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "΢���ź�");
        outtextxy(250, 40, temp);
        break;
    }
    case SPEED_UP:
    {
        
        sprintf_s(temp, "��ǰʳ��Buff������  ");//�Ѻ���Ķ�����д�뵽temp������
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "΢���ź�");
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
        sprintf_s(temp, "��ǰʳ��Buff����Buff  ");//�Ѻ���Ķ�����д�뵽temp������
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "΢���ź�");
        outtextxy(400, 40, temp);
        break;
    }
    case EXTRA_SCORE:
    {
        sprintf_s(temp, "��ǰʳ��Buff����������  ");//�Ѻ���Ķ�����д�뵽temp������
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "΢���ź�");
        outtextxy(400, 40, temp);
        break;
    }
    case Lengthen:
    {
        sprintf_s(temp, "��ǰʳ��Buff����ø���  ");//�Ѻ���Ķ�����д�뵽temp������
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "΢���ź�");
        outtextxy(400, 40, temp);
        break;
    }
    case SPEED_UP:
    {

        sprintf_s(temp, "��ǰʳ��Buff������  ");//�Ѻ���Ķ�����д�뵽temp������
        settextcolor(RGB(0, 0, 0));
        setbkmode(TRANSPARENT);
        settextstyle(30, 15, "΢���ź�");
        outtextxy(400, 40, temp);
        break;

    }
    }
}


//----------��Ϸ��������----------
//����Ϊ����ģʽ��˫��ģʽ

//��ȡ�����ش��ھ����������Ӧ�Ĵ������

HWND single_snake()
{
    HWND window;
    window = initgraph(window_width, window_height, 0);
    Snake s1(1,RGB(113,147,130));
    Food f;
    s1.Snake_Init(405,405);
    DWORD t1, t2;
    t1 = t2 = GetTickCount();
    BeginBatchDraw();//����������ͼ�ķ�ʽ Ҳ����˵ ֱ������ͼ�������˲Ż�һ����ȫ��չʾ����
 
    while (1)
    {
       
        if (t2 - t1 > f.Get_delay())
        {
            s1.Snake_move();
            t1 = t2;
        }
        t2 = GetTickCount();//����ˢ�¼��
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
            //��Ϸ������ʾ
            LPBTN lose = createButton(200, 275, 600, 325, RGB(152, 98, 89), "ײ��ǽ������Ϸ����~~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(lose);
                MOUSEMSG m;
                m = GetMouseMsg();//��ȡ�����Ϣ
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
            //��Ϸ������ʾ
            LPBTN lose = createButton(200, 275, 600, 325, RGB(152, 98, 89), "�Ե��Լ�������Ϸ����~~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(lose);
                MOUSEMSG m;
                m = GetMouseMsg();//��ȡ�����Ϣ
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
        FlushBatchDraw();//չʾ������ͼ�Ľ��
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
    BeginBatchDraw();//����������ͼ�ķ�ʽ Ҳ����˵ ֱ������ͼ�������˲Ż�һ����ȫ��չʾ����
    while (1)
    {
        if (t2 - t1 > f.Get_delay())
        {
            s1.Snake_move();
            s2.Snake_move();
            t1 = t2;
        }
        t2 = GetTickCount();//����ˢ�¼��
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
            //��Ϸ������ʾ
            LPBTN lose = createButton(200, 275, 600, 325, RGB(152, 98, 89), "С��ײ��ǽ������Ϸ����~~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(lose);
                MOUSEMSG m;
                m = GetMouseMsg();//��ȡ�����Ϣ
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
            //��Ϸ������ʾ
            LPBTN lose = createButton(200, 275, 600, 325, RGB(152, 98, 89), "С��ײ��ǽ������Ϸ����~~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(lose);
                MOUSEMSG m;
                m = GetMouseMsg();//��ȡ�����Ϣ
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
        if (s1.Eat_Myself())//S1���� ��Ϸ������ʾ
        {
            LPBTN S11win = createButton(200, 275, 600, 325, RGB(152, 98, 89), "С��ҧ�����Լ���С��ʤ��~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(S11win);
                MOUSEMSG m;
                m = GetMouseMsg();//��ȡ�����Ϣ
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
            //S2���� ��Ϸ������ʾ
            LPBTN S22win = createButton(200, 275, 600, 325, RGB(152, 98, 89), "С��ҧ�����Լ���С��ʤ��~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(S22win);
                MOUSEMSG m;
                m = GetMouseMsg();//��ȡ�����Ϣ
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
        FlushBatchDraw();//չʾ������ͼ�Ľ��
        if (s1.Eat_Another(s2))
        {
            //S1 ����
            LPBTN S12win = createButton(200, 275, 600, 325, RGB(152, 98, 89), "С��ײ����С�ȣ�С��ʤ��~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(S12win);
                MOUSEMSG m;
                m = GetMouseMsg();//��ȡ�����Ϣ
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
            //S2 ����

            LPBTN S21win = createButton(200, 275, 600, 325, RGB(152, 98, 89), "С��ײ����С�̣�С��ʤ��~", ShowBackground);
            while (1)
            {
                BeginBatchDraw();
                drawButton(S21win);
                MOUSEMSG m;
                m = GetMouseMsg();//��ȡ�����Ϣ
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

