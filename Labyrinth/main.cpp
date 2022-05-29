#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<graphics.h>
//#include<unistd.h>
#undef UNICODE
#undef _UNICODE

//*******************************************************************
//         本项目中注释内容，为视觉识别地图所需代码，在本项目不作展示
//         关于视觉识别地图，具体内容请在  VisualMaze  文件夹中的项目中体验
/*#include<opencv2\opencv.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\core.hpp>
#include<string.h>
#include <opencv2/core/utils/logger.hpp>*/
//*******************************************************************
using namespace std;
//using namespace cv;

/*struct countourRect
{
	Rect rec;
	countourRect* next = NULL;
	int val;
	Mat roi;
};*/

//countourRect* Visual(int row, int col, int** maze, string map = "./Templates/try1.png");
struct node //节点结构体 里面有行列和方向
{
	int row;
	int col;
	int dir;
}; 

template<typename T>
struct stacknode //栈结点
{
	T my_node;
	stacknode<T>* next;
}; 

//链式栈的类
template<typename T>
class Linkstack
{
	public:
		Linkstack();
		~Linkstack();
		bool Push(const T& e); //入栈函数
		bool Pop(T& e); //出栈函数
		bool Gettop(T& e);
		bool showdata();
		int Listlength();
		bool Inempty();
		stacknode<T>* getp();
	private:
		stacknode<T>* top;	//指向栈顶的指针
		int length;
};

//构造函数 
template<typename T>
Linkstack<T>::Linkstack()
{
	top=NULL;
	length=0;
}

//析构函数 
template<typename T>
Linkstack<T>::~Linkstack()
{
	T a;
	a.row=0; //a是node类型？？？？？ 这个析构在干什么？？？？
	a.col=0;
	a.dir=0;
	while(Pop(a)==true){} //不断反复调用出栈函数 直到为true
	delete top;
}

template<typename T> //拿到栈顶指针
stacknode<T>* Linkstack<T>::getp()
{
	return top;
}

//入栈 
template<typename T>
bool Linkstack<T>::Push(const T& e)
{
	stacknode<T>* node1= new stacknode<T>;
	node1->my_node.row=e.row;
	node1->my_node.col=e.col;
	node1->my_node.dir=e.dir;
	node1->next=top; //这里的top是之前的栈顶节点
	top=node1; //栈顶从新插入的节点开始
	length++;
	return true; 
}

//出栈 
template<typename T>
bool Linkstack<T>::Pop(T& e)
{ 
	if(Inempty()==true)
		return false; //如果已经是 空 了，返回true
	stacknode<T>* node1= top;
	top=top->next;
	e.row=node1->my_node.row;
	e.col=node1->my_node.col;
	e.dir=node1->my_node.dir;
	length--;
	delete node1; //?这不是new出来的 不用delete啊
	return true; 
}

//是否为空 
template<typename T>
bool Linkstack<T>::Inempty()
{
	if(top==NULL)
		return true; //如果top为空，返回true
	return false;
}

//取长度
template<typename T>
int Linkstack<T>::Listlength()
{
	return length;
}

int minstep = 1 << 30;
int length=0;
int Flag = 0;
int** tonglu = NULL;

struct snode {//这个结构体只在DFS算法中使用
	snode* qian = NULL, * next[8] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
	int x, y;
	int movefromflag;//它是由前面的往哪个方向来的
	int movetoflag=-1;//它应当往哪个方向去？
};

// Deep First Search 深度优先搜索 ***********   创新点
int DFS(int x, int y, int step,int** maze,int row,int col,int move[8][2],int** flags,snode* p) 
{
	if (x >= row+1 || y >= col+1 || x < 1 || y < 1) return -1;
	if (x == row && y == col) //(row, col)是目标地点？
	{
		if (step < minstep) minstep = step;
		return step;
	}

	if (maze[x][y] == 1) return -1;
	int totalstep = -1;
	int min_step_of_this_point = -1;
	for (int i = 0; i < 8; i++) 
	{
		if (x + move[i][1] >= row+1 || y + move[i][0] >= col+1 || x + move[i][1] < 1 || y + move[i][0] < 1) continue;//判断边界 ？？？
		if (maze[x + move[i][1]][y + move[i][0]] == 0 && !flags[x + move[i][1]][y + move[i][0]]) 
		{
			flags[x + move[i][1]][y + move[i][0]] = 1;
			p->next[i] = new snode;
			p->next[i]->qian = p;
			p = p->next[i];
			p->x = x + move[i][1];
			p->y = y + move[i][0];
			p->movefromflag = i;
			totalstep = DFS(x + move[i][1], y + move[i][0], step + 1,maze,row,col,move,flags,p);//递归

			if (totalstep > minstep||totalstep == -1) //如果已经超过最小step了，直接退回上个节点，以保证最小
			{
				p = p->qian;
				delete p->next[i];
			}
			else 
			{
				snode* temp = p;
				p = p->qian;
				p->movetoflag = temp->movefromflag;
				min_step_of_this_point = totalstep;
			}
			flags[x + move[i][1]][y + move[i][0]] = 0;//回溯,避免它走它来时的路
		}

	}
	return min_step_of_this_point;

}

//按钮结构体(用在画图上)
typedef struct Button
{
	int x;
	int y;
	int xx;
	int yy;
	COLORREF color;
	char* buttonstr;
	void (*f)(int** maze, int l, int c, snode* p);
}*LPBTN;

//按钮封装
LPBTN createButton(int x, int y, int xx, int yy, COLORREF color, const char* buttonstr, void (*f)(int** maze, int l, int c, snode* p))
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
	setlinecolor(RGB(130, 73, 71));//设置画线颜色
	settextstyle(25, 0, "黑体"); //设置图形文本当前字体、字符大小
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

//鼠标是否点击按钮
void buttonAction(LPBTN button, MOUSEMSG m,int **maze, int l, int c, snode* p)
{
	if (isInButton(button, m))
	{
		button->color = RGB(198, 150, 125);
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			closegraph();
			button->f(maze, l, c, p);
		}
	}
	else button->color = RGB(152, 98, 89);
}

//展示背景图片
void ShowBackground()
{
	initgraph(800, 600);
	IMAGE picture;
	loadimage(&picture, "background.jpg", 800, 600);
	putimage(0, 0, &picture);
}

//输出交互界面文字，共三种重载
void ShowText(int x,int y,const char* str)
{
	char temp[50] = "";
	sprintf_s(temp, str);
	settextcolor(RGB(0, 0, 0));
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, "微软雅黑");
	outtextxy(x, y, temp);
}

void ShowText(int x, int y, int i)
{
	char temp[50] = "";
	sprintf_s(temp, "%d",i);
	settextcolor(RGB(0, 0, 0));
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, "微软雅黑");
	outtextxy(x, y, temp);
}

void ShowText(int x, int y, int i, int j)
{
	char temp[50] = "";
	sprintf_s(temp, "%d, %d", i,j);
	settextcolor(RGB(0, 0, 0));
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, "微软雅黑");
	outtextxy(x, y, temp);
}

//绘制地图
void DrawMap(int** maze, int l, int c, snode* p);//绘制原始地图
void DrawMethodNULL(int** maze, int l, int c, snode* p)//用于函数指针正常使用的空函数
{}

void DrawMethod1(int** maze, int l, int c, snode* p)//绘制第一种解决方案地图
{
	COLORREF color;
	LPBTN nosolution = createButton(50 * c + 100, 50, 50 * c + 250, 100, RGB(152, 99, 89), "无合适路径", DrawMethodNULL);//创建按钮
	LPBTN solution = createButton(50 * c + 100, 50, 50 * c + 250, 100, RGB(152, 99, 89), "有合适路径", DrawMethodNULL);
	int i = 0, j = 0;
	char temp[50] = "";
	char temp1[50] = "";
	snode* head;

	head = p;

	initgraph(50 * c + 300, 50 * l + 100);
	setbkcolor(RGB(243, 228, 176));
	cleardevice();
	setlinecolor(RGB(130, 73, 71));
	for (i = 0; i < l; i++)//绘制地图
	{
		for (j = 0; j < c; j++)
		{
			if (maze[i][j] == 0 || maze[i][j] == 2) color = RGB(243, 228, 176);
			else color = RGB(152, 99, 89);
			setfillcolor(color);
			fillrectangle(50 * j + 50, 50 * i + 50, 50 * j + 100, 50 * i + 100);
		}
	}
	for (i = 0; i < minstep; i++)//输出通路矩阵
		{
			if (p->movetoflag == -1)
			{
				drawButton(nosolution);
				break;
			}
			else
			{
				drawButton(solution);
				sprintf_s(temp, "步数：%d  ", minstep);
				settextcolor(RGB(0, 0, 0));
				setbkmode(TRANSPARENT);
				settextstyle(25, 0, "微软雅黑");
				outtextxy(50 * c + 100, 120, temp);

				sprintf_s(temp1, "(%d,%d,%d)",p->x,p->y,p->movetoflag);
				settextcolor(RGB(0, 0, 0));
				setbkmode(TRANSPARENT);
				settextstyle(25, 0, "微软雅黑");
				outtextxy(50 * c + 100, 155+35*i, temp1);

				setfillcolor(RGB(174,187,143));
				fillrectangle(50 * p->y + 50, 50 * p->x + 50, 50 * p->y + 100, 50 * p->x + 100);
			}
			p = p->next[p->movetoflag];
		}
	fillrectangle(50 * c - 50, 50 *l - 50, 50 * c, 50 * l);
	p = head;
	while (1) //左键返回主地图界面
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			break;
		}
	}
	DrawMap(maze, l, c, p);
}

void DrawMethod2(int** maze, int l, int c, snode* p)//绘制第二种解决方案地图
{
	COLORREF color;
	LPBTN nosolution = createButton(50 * c + 100, 50, 50 * c + 250, 100, RGB(152, 99, 89), "无合适路径", DrawMethodNULL);
	LPBTN solution = createButton(50 * c + 100, 50, 50 * c + 250, 100, RGB(152, 99, 89), "有合适路径", DrawMethodNULL);
	int i = 0, j = 0;
	char temp[50] = "";
	char temp1[50] = "";

	initgraph(50 * c + 300, 50 * l + 100);
	setbkcolor(RGB(243, 228, 176));
	cleardevice();
	setlinecolor(RGB(130, 73, 71));
	if (Flag)
	{
		drawButton(solution);
		for (i = 0; i < length; i++)
		{
				sprintf_s(temp, "步数：%d  ", length);
				settextcolor(RGB(0, 0, 0));
				setbkmode(TRANSPARENT);
				settextstyle(25, 0, "微软雅黑");
				outtextxy(50 * c + 100, 120, temp);

				sprintf_s(temp1, "(%d,%d,%d)", tonglu[i][0], tonglu[i][1], tonglu[i][2]);
				settextcolor(RGB(0, 0, 0));
				setbkmode(TRANSPARENT);
				settextstyle(25, 0, "微软雅黑");
				outtextxy(50 * c + 100, 155 + 35 * i, temp1);
		}
	}
	else drawButton(nosolution);
	for (i = 0; i < l; i++)
	{
		for (j = 0; j < c; j++)
		{
			if (maze[i][j] == 0) color = RGB(243, 228, 176);
			else if (maze[i][j] == 2) color = RGB(220, 151, 112);
			else color = RGB(152, 99, 89);
			setfillcolor(color);
			fillrectangle(50 * j + 50, 50 * i + 50, 50 * j + 100, 50 * i + 100);
		}
	}

	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			break;
		}
	}
	DrawMap(maze, l, c, p);
}

void ShowClose(int** maze, int l, int c, snode* p) { exit(0); }

void DrawMap(int** maze, int l, int c, snode* p)//绘制主地图
{
	COLORREF color;
	int i = 0, j = 0;
	int flag = 0;

	initgraph(50 * c + 300, 50 * l + 100);
	setbkcolor(RGB(243, 228, 176));
	cleardevice();
	setlinecolor(RGB(130, 73, 71));

	for (i = 0; i < l; i++)
	{
		for (j = 0; j < c; j++)
		{
			if (maze[i][j] == 0 || maze[i][j] == 2) color = RGB(243, 228, 176);
			else color = RGB(152, 99, 89);
			setfillcolor(color);
			fillrectangle(50 * j + 50, 50 * i + 50, 50 * j + 100, 50 * i + 100);
		}
	}
	
	LPBTN method1 = createButton(50*c+100,50, 50*c+250, 100, RGB(152, 99, 89),"路径优先法", DrawMethod1);
	LPBTN method2 = createButton(50*c+100,120, 50*c+250, 170, RGB(152, 99, 89), "速度优先法", DrawMethod2);
	LPBTN close = createButton(50 * c + 100, 190, 50 * c + 250, 240, RGB(152, 99, 89), "退出", ShowClose);
	
	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		//BeginBatchDraw();
		drawButton(method1);
		drawButton(method2);
		drawButton(close);
		for (i = 0; i < l; i++)
		{
			for (j = 0; j < c; j++)
			{
				if (maze[i][j] == 0 || maze[i][j] == 2) color = RGB(243, 228, 176);
				else color = RGB(152, 99, 89);
				setfillcolor(color);
				fillrectangle(50 * j + 50, 50 * i + 50, 50 * j + 100, 50 * i + 100);
			}
		}
		buttonAction(method1, m, maze, l, c, p);
		buttonAction(method2, m, maze, l, c, p);
		buttonAction(close, m, maze, l, c, p);
		/*if (m.uMsg == WM_LBUTTONDOWN)
		{
			DrawMethod2(maze, l, c, p);
			flag = 1;
		}
		if (m.uMsg == WM_RBUTTONDOWN)
		{
			DrawMethod1(maze, l, c, p);
			flag = 1;
		}
		if (flag) break;*/
		//EndBatchDraw();
	}
}

int main()
{
	ShowBackground();

	//utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);
	string s = "./Templates/try2.png";
	//countourRect* firstimgblock=NULL;
	node mynode;
	Linkstack<node> stack;
	int move[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};// 下 右下 右 右上 上 左上 左 左下
	int hang_num=3,lie_num=3;//行数和列数 

	cout << "输入行数和列数" << endl;
	ShowText(100,200, "输入行数和列数");
	
	cin>>hang_num>>lie_num;
	ShowText(550, 200, hang_num, lie_num);

	
	int k,xuanze,rdoor;
	int i,j;
	int x_weiz,y_weiz; //x_weiz为行，y_weiz为列
	int **maze=new int*[hang_num+2];
	int** flags = new int* [hang_num + 2];
	for (i = 0; i < hang_num + 2; i++)
	{
		maze[i] = new int[lie_num + 2];
		flags[i] = new int[lie_num + 2];
		for (j = 0; j < lie_num + 2; j++) flags[i][j] = 0;
	}
		
	
	
	cout<<"选择地图：0为自定义地图；1为随机地图。"<<endl; 
	ShowText(100, 250, "选择地图：0为自定义地图；1为随机地图。");
	cin>>xuanze;
	ShowText(550, 250, xuanze);
	if(xuanze==0)
	{
		cout<<"输入地图数据"<<endl; 
		ShowText(100, 300, "输入地图数据");
		for(i=0;i<hang_num+2;i++)//自定义地图
		{
			for(j=0;j<lie_num+2;j++)
			{
				if(i==0||i==hang_num+1||j==0||j==lie_num+1)
					maze[i][j]=1;
				else
				{
					cin>>k;
					if(k==0||k==1)
						maze[i][j]=k;
					else
					{
						maze[i][j]=rand()%2;
					}
				}
			}
		}
		//maze[hang_num][lie_num] = 0;
	}
	else if(xuanze ==1)
	{
		for(i=0;i<hang_num+2;i++)//随机地图
		{
			for(j=0;j<lie_num+2;j++)
			{
				if(i==0||i==hang_num+1||j==0||j==lie_num+1)
					maze[i][j]=1;
				else
				{
					maze[i][j]=rand()%2;//生成0/1随机数
				}
			}
		} 
		maze[hang_num][lie_num] = 0;
	}
	else if (xuanze == 2)
	{
		
		//firstimgblock = Visual(hang_num, lie_num, maze, "./Templates/try2.png");
	}
	cout<<"设置入口位置：值为1-"<<hang_num<<endl;  // 入口位置指的是 第一列第几行作为入口 此时，无论它在地图中是0还是1 都可以作为入口 认为它是0
	ShowText(100, 350, "设置入口位置");
	cin>>rdoor;//入口的行
	ShowText(550, 350, rdoor);

	ShowText(100, 400, "输入任意数字并回车程序继续");
	char sleep;
	cin >> sleep;

	maze[rdoor][1]=0;
	snode *p,*head;
	p = new snode;
	p->x = rdoor;
	p->y = 1;
	head = p;
	flags[rdoor][1] = 1;
	DFS(rdoor, 1, 0, maze, hang_num, lie_num, move, flags, p);//调用DFS算法求解迷宫 并将结果存在 head指针所引导的树中  
	                                                         //即“路径优先法” ，指的是所需步数最少的通路
	cout << minstep << endl;
	p = head;
	cout << "(" << p->x << "," << p->y << "," << p->movetoflag << ")" << endl;
	for (i = 1; i<minstep; i++)
	{
		if (p->movetoflag == -1)
		{
			cout << "走不通呀" << endl;
			break;
		}
		p = p->next[p->movetoflag];

		cout << "(" << p->x << "," << p->y << "," << p->movetoflag << ")" << endl;
		
	}
	/*if (xuanze == 2)
	{
		Mat img;
		img = imread(s);
		snode* former, latter;
		p = head;
	
		countourRect *imgp=firstimgblock,*imgq = firstimgblock;
		for (i = 0; i < minstep; i++)
		{
			for (int n = 0; n < (p->x - 1) * lie_num + p->y - 1; n++) imgp = imgp->next;
			for (int n = 0; n < (p->next[p->movetoflag]->x - 1) * lie_num + p->next[p->movetoflag]->y - 1; n++) imgq = imgq->next;
			line(img, Point(imgp->rec.x + imgp->rec.width, imgp->rec.y + imgp->rec.height), Point(imgq->rec.x + imgq->rec.width, imgq->rec.y + imgq->rec.height),
				Scalar(0, 0, 255), 3);
		}
		imshow("Image", img);
		waitKey(0);

	}*/

	mynode.row=rdoor;//存第一个即入口位置
	mynode.col=1;
	mynode.dir=0;
	stack.Push(mynode);//进栈
	maze[hang_num][lie_num]=0;//默认右下角为出口 
	for(i=0;i<hang_num+2;i++)//显示地图
	{
		for(j=0;j<lie_num+2;j++)
		{
			cout<<maze[i][j];
		}
		cout<<endl;
	}
	int flag=0;
	maze[rdoor][1]=2;//走过的地方变2
	while(!stack.Inempty()&&!flag)    //*************************************** 速度优先法求解 ： 速度优先法指的是 程序运行速度最快
	{
		stack.Pop(mynode);//取位置，如果碰到死胡同，根据之前的信息退回
		i=mynode.row;
		j=mynode.col;
		k=mynode.dir;
		while(k<8)//每个方向都试一遍，都不行就开始后退
		{
			x_weiz=i+move[k][1];//试探前进
			y_weiz=j+move[k][0];
			if (x_weiz == hang_num && y_weiz == lie_num)//到达终点
			{
				maze[x_weiz][y_weiz] = 2;
				mynode.row = x_weiz;
				mynode.col = y_weiz;
				mynode.dir = k;
				stack.Push(mynode);//存入终点的信息
				flag = 1;//到达终点标志
				Flag = 1;
				break;
			}
			if(maze[x_weiz][y_weiz]==0)//试探到0就走进去
			{
				maze[x_weiz][y_weiz]=2;//走过变2，防止退回时再走
				mynode.row=x_weiz;//更新当前信息
				mynode.col=y_weiz;
				mynode.dir=k;
				i=mynode.row;//更新当前位置
				j=mynode.col;
				k=0;//方向重新到第一个方向
				stack.Push(mynode);//存当前信息
			}
			else//没试探到0就换下一个方向
			{
				k = k + 1;
			}
		}
	}
	if(flag)
	{
		cout<<"迷宫走通<_<"<<endl;
		for(i=1;i<hang_num+1;i++)
		{
			for(j=1;j<lie_num+1;j++)
			{
				cout<<maze[i][j];
			}
			cout<<endl;
		}
		//int length;
		length = stack.Listlength();
		cout << "步数:" << length << endl;
		cout << "通路" << endl;
		//int **tonglu=new int *[length];
		tonglu = new int* [length];
		for (i = 0;i < length;i++)
			tonglu[i] = new int[3];
		stack.Pop(mynode);
		int dir1;
		dir1 = mynode.dir;//dir1存下走到下一个坐标所用的方向
		for (i = 0;i < length-1;i++)
		{
			stack.Pop(mynode);
			tonglu[length - 1 - i][0] = mynode.row;//坐标是当前坐标，也就是走到下一个坐标前的坐标
			tonglu[length - 1 - i][1] = mynode.col;
			tonglu[length - 1 - i][2] = dir1;
			dir1 = mynode.dir;
		}
		tonglu[0][0] = rdoor;//设好的起点 
		tonglu[0][1] = 1;
		tonglu[0][2] = dir1;//走到下一点的方向
		for (i = 0;i < length;i++)
		{
			cout << "(";
			for (j = 0;j < 2;j++)
			{
				cout << tonglu[i][j] << ",";
			}
			cout << tonglu[i][2] << ")" << endl;
		}
	}
	else
	{
		cout<<"死胡同，走不通>_<"<<endl;
	}



	DrawMap(maze, hang_num + 2, lie_num + 2, head);
}



/*
countourRect* Visual(int row, int col, int** maze, string map )
{
	int i, j;
	Mat template0 = imread("./Templates/0.png", 0);
	Mat template1 = imread("./Templates/1.png", 0);
	threshold(template0, template0, 127, 255, THRESH_BINARY_INV);
	threshold(template1, template1, 127, 255, THRESH_BINARY_INV);
	resize(template0, template0, Size(300, 600));
	resize(template1, template1, Size(300, 600));
	vector<KeyPoint> kpt0, kpt1;
	Mat descriptors0, descriptors1;
	Mat try1 = imread(map);
	Mat img;
	cvtColor(try1, img, COLOR_BGR2GRAY);
	threshold(img, img, 127, 255, THRESH_BINARY_INV);
	/*imshow("0", template0);
	imshow("1", template1);
	imshow("img", img);
	waitKey(0);*/
/*
	Ptr<SiftFeatureDetector> sift = SiftFeatureDetector::create();
	sift->detect(template0, kpt0);
	sift->compute(template0, kpt0, descriptors0);
	sift->detect(template1, kpt1);
	sift->compute(template1, kpt1, descriptors1);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
	//cout << contours.size() << endl;
	countourRect* rechead;
	countourRect* newrect = new countourRect;
	rechead = newrect;//头节点没有东西
	for (i = 0; i < contours.size(); i++)
	{
		drawContours(try1, contours, i, Scalar(255, 0, 0), 4);
		Rect rec = boundingRect(contours[i]);
		rectangle(try1, rec, Scalar(0, 0, 255), 4);

	}

	Ptr<DescriptorMatcher>matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
	for (i = row - 1; i >= 0; i--)
	{
		for (j = 0; j < col; j++)
		{
			Rect rec = boundingRect(contours[i * col + j]);
			newrect->next = new countourRect;
			newrect = newrect->next;
			newrect->rec = rec;
			newrect->roi = img(rec);



			Mat result0, result1;
			resize(newrect->roi, newrect->roi, Size(300, 600));
			matchTemplate(newrect->roi, template0, result0, TM_CCOEFF_NORMED);
			matchTemplate(newrect->roi, template1, result1, TM_CCOEFF_NORMED);
			double maxval0 = -1, maxval1 = -1;
			Point maxLoc0, maxLoc1;
			minMaxLoc(result0, NULL, &maxval0, NULL, &maxLoc0, Mat());
			minMaxLoc(result1, NULL, &maxval1, NULL, &maxLoc1, Mat());
			if (maxval0 > maxval1)
			{
				//cout << "0" << endl;
				newrect->val = 0;
			}
			else
			{
				//cout << "1" << endl;
				newrect->val = 1;
			}

		}
		countourRect* p = rechead, * q;
		for (j = 0; j < (row-1 - i) * col; j++) p = p->next;

		for (j = 0; j < col; j++)
		{
			p = p->next;
			q = p;
			for (int k = j; k < col; k++)
			{
				if (q->rec.x < p->rec.x)
				{
					int temp = p->val;
					Rect temprec = p->rec;
					p->val = q->val;
					p->rec = q->rec;
					q->val = temp;
					q->rec = temprec;
				}
				q = q->next;
			}
		}
		p = rechead;
		for (j = 0; j < (row-1 - i) * col; j++) p = p->next;
		for (j = 0; j < col; j++)
		{
			p = p->next;
			//cout << "第" << 3 - i << "行," << "第" << j << "列:" << p->val << endl;
			stringstream s;
			s << (row -1 - i) * col + j;
			putText(try1, s.str(), Point(p->rec.x, p->rec.y), FONT_HERSHEY_PLAIN, 3, Scalar(0, 255, 0), 3);
		}

	}
	imshow("try1", try1);
	waitKey(0);
	for (i = 0; i < row + 2; i++)//自定义地图
	{
		for (j = 0; j < col + 2; j++)
		{
			if (i == 0 || i == row + 1 || j == 0 || j == col + 1)
				maze[i][j] = 1;
			else
			{
				countourRect* p;
				p = rechead;
				for (int n = 0; n < (i - 1) * col + j - 1; n++) p = p->next;
				int k = p->next->val;
				if (k == 0 || k == 1)
				{
					maze[i][j] = k;
				}
				else
				{
					maze[i][j] = rand() % 2;
				}
			}
		}
	}
	return rechead;
}*/