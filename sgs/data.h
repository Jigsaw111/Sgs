#include<graphics.h>//easyX图形库
#include<stdlib.h>
#include<string>
#include<iostream>
#include<vector>
#include<time.h>
#include<windows.h>
#pragma comment(lib,"WinMM.Lib")//播放音乐用

using namespace std;

/***********************************************************/
//创建类

//玩家
struct player { vector<int>card; int judge; int hero; int hp; };
player wanjia[2];//player0 is user,player1 is computer.

//武将 
struct hero { int force; int skill; int gender; string name; IMAGE photo; int hp; };
hero wujiang[26];//25张标准武将
//force:1=魏 2=蜀 3=吴 0=群
//gender:1=男 0=女
vector<int>nowwujiang;//用来洗牌

//卡牌
struct card { int color; int type; string name; IMAGE photo; };
card kapai[109];//108张标准卡牌
vector<int>nowkapai;//用来洗牌
//color:1=红桃 2=方块 3=黑桃 4=梅花
//type:1=基本牌 2=非延时锦囊牌 3=装备牌 4=延时锦囊

//技能
struct skill { int type; string name; };
skill jineng[26];//偷懒，每个武将只有一个技能。
//type:1=锁定 0=主动

/**********************************************************/
//主界面

int login()//初始界面
{
	//主界面
	initgraph(1080, 720);
	IMAGE login;
	loadimage(&login, "photo//login.png", 1080, 720);
	putimage(0, 0, &login);
	//初始UI

	//PlaySound("sound//bgm.wav", NULL, SND_FILENAME | SND_ASYNC);
	//BGM,很吵，建议不要打开，放在这里只是为了证明我会播放BGM

	while (true)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.x > 280 && m.x < 730 && m.y>395 && m.y < 520 && m.uMsg == WM_LBUTTONDOWN)
		{
			break;
		}
		if (m.x > 290 && m.x < 730 && m.y>210 && m.y < 3366 && m.uMsg == WM_LBUTTONDOWN)
			MessageBox(NULL, "你在想屁吃", "你在想屁吃", MB_OK);
		//暂时没有能力搞联网对战，等我什么时候会了也许会搞一搞
	}
	//按钮检测
	return 0;
}

int choosehero()//选将
{
	IMAGE choose;
	loadimage(&choose, "photo//choose.png", 1080, 720);
	putimage(0, 0, &choose);
	//载入选将UI

	srand(time(0));
	for (int i = 0; i < 25; i++)
	{
		nowwujiang.push_back(rand() % 25 + 1);
		for (int j = 0; j < i; j++)
			if (nowwujiang[i] == nowwujiang[j])
			{
				nowwujiang.pop_back();
				i--;
				break;
			}
	}
	//洗武将牌

	int h1, h2, h3;
	h1 = nowwujiang[nowwujiang.size() - 1];
	nowwujiang.pop_back();
	h2 = nowwujiang[nowwujiang.size() - 1];
	nowwujiang.pop_back();
	h3 = nowwujiang[nowwujiang.size() - 1];
	nowwujiang.pop_back();
	putimage(160, 103, &wujiang[h1].photo);
	putimage(400, 103, &wujiang[h2].photo);
	putimage(640, 103, &wujiang[h3].photo);
	//发武将牌

	while (true)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.x > 160 && m.x < 400 && m.y>103 && m.y < 503 && m.uMsg == WM_LBUTTONDOWN) wanjia[0].hero = h1;
		if (m.x > 400 && m.x < 640 && m.y>103 && m.y < 503 && m.uMsg == WM_LBUTTONDOWN) wanjia[0].hero = h2;
		if (m.x > 640 && m.x < 880 && m.y>103 && m.y < 503 && m.uMsg == WM_LBUTTONDOWN) wanjia[0].hero = h3;
		if (m.x > 400 && m.x < 640 && m.y>550 && m.y < 620 && m.uMsg == WM_LBUTTONDOWN)
		{
			if (wanjia[0].hero > 0 && wanjia[0].hero < 26)
				break;
			else
				MessageBox(NULL, "你还未选择武将", "提示", MB_OK);
		}
	}
	//按钮检测

	wanjia[1].hero = nowwujiang[nowwujiang.size() - 1];
	nowwujiang.pop_back();
	//为AI选将

	wanjia[0].hp = wujiang[wanjia[0].hero].hp;
	wanjia[1].hp = wujiang[wanjia[1].hero].hp;
	//载入hp

	return 0;
}

int gameover()//结算
{
	IMAGE gameover;
	loadimage(&gameover, "photo//gameover.png", 1080, 720);
	putimage(0, 0, &gameover);
	//载入结算界面

	while (TRUE)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
			exit(0);
	}
}

/**********************************************************/
//必要模块

int flash()//重载UI，可能会导致界面一闪一闪，懒得找解决办法
{
	IMAGE gamestart;
	loadimage(&gamestart, "photo//gamestart.png", 1080, 720);
	putimage(0, 0, &gamestart);
	//界面UI

	putimage(0, 420, 180, 300, &wujiang[wanjia[0].hero].photo, 60, 0);
	putimage(895, 0, 180, 300, &wujiang[wanjia[1].hero].photo, 60, 100);
	//武将图片

	for (int i = 0; i < wanjia[0].card.size(); i++)
	{
		putimage(311 + 45 * i, 590, &kapai[wanjia[0].card[i]].photo);
	}
	//载入玩家手牌

	for (int i = 0; i < wanjia[1].card.size(); i++)
	{
		putimage(673 - 45 * i, 0, &kapai[0].photo);
	}
	//载入AI手牌

	char s1[2];
	sprintf(s1, "%d", wanjia[0].hp);
	outtextxy(217, 608, s1);
	sprintf(s1, "%c", '/');
	outtextxy(237, 608, s1);
	sprintf(s1, "%d", wujiang[wanjia[0].hero].hp);
	outtextxy(257, 608, s1);
	//玩家HP

	char s2[2];
	sprintf(s2, "%d", wanjia[1].hp);
	outtextxy(804, 110, s2);
	sprintf(s2, "%c", '/');
	outtextxy(824, 110, s2);
	sprintf(s2, "%d", wujiang[wanjia[1].hero].hp);
	outtextxy(844, 110, s2);
	//AIHP

	return 0;
}

int cardflash(int x)//选牌定制版重载UI
{
	IMAGE gamestart;
	loadimage(&gamestart, "photo//gamestart.png", 1080, 720);
	putimage(0, 0, &gamestart);
	//界面UI

	putimage(0, 420, 180, 300, &wujiang[wanjia[0].hero].photo, 60, 0);
	putimage(895, 0, 180, 300, &wujiang[wanjia[1].hero].photo, 60, 100);
	//武将图片

	for (int i = 0; i < wanjia[0].card.size(); i++)
	{
		if (i == x) continue;
		putimage(311 + 45 * i, 590, &kapai[wanjia[0].card[i]].photo);
	}
	//载入玩家手牌

	for (int i = 0; i < wanjia[1].card.size(); i++)
	{
		putimage(673 - 45 * i, 0, &kapai[0].photo);
	}
	//载入AI手牌

	char s1[2];
	sprintf(s1, "%d", wanjia[0].hp);
	outtextxy(217, 608, s1);
	sprintf(s1, "%c", '/');
	outtextxy(237, 608, s1);
	sprintf(s1, "%d", wujiang[wanjia[0].hero].hp);
	outtextxy(257, 608, s1);
	//玩家HP

	char s2[2];
	sprintf(s2, "%d", wanjia[1].hp);
	outtextxy(804, 110, s2);
	sprintf(s2, "%c", '/');
	outtextxy(824, 110, s2);
	sprintf(s2, "%d", wujiang[wanjia[1].hero].hp);
	outtextxy(844, 110, s2);
	//AIHP

	return 0;
}

int washcard()//洗牌
{
	srand(time(0));
	for (int i = 0; i < 108; i++)
	{
		nowkapai.push_back(rand() % 108 + 1);
		for (int j = 0; j < i; j++)
			if (nowkapai[i] == nowkapai[j])
			{
				nowkapai.pop_back();
				i--;
				break;
			}
	}
	return 0;
}

int getcard(int u = 0, int x = 2)//摸牌,默认为玩家摸两张
{
	for (int i = 0; i < x; i++)
	{
		if (!u)
			putimage(311 + 45 * wanjia[u].card.size(), 590, &kapai[nowkapai[nowkapai.size() - 1]].photo);
		else
			putimage(673 - 45 * wanjia[u].card.size(), 0,&kapai[0].photo);
		wanjia[u].card.push_back(nowkapai[nowkapai.size() - 1]);
		nowkapai.pop_back();
	}
	return 0;
}

int button()//按钮检测
{
	while (TRUE)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.x > 181 && m.x < 310 && m.y>418 && m.y < 478 && m.uMsg == WM_LBUTTONDOWN) return 1;//确定
		if (m.x > 181 && m.x < 310 && m.y>478 && m.y < 540 && m.uMsg == WM_LBUTTONDOWN) return 2;//取消
		if (m.x > 181 && m.x < 310 && m.y>540 && m.y < 595 && m.uMsg == WM_LBUTTONDOWN) return 3;//结束
	}
}

int choosecard()//选牌
{
	while (TRUE)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		for (int i = 0; i < wanjia[0].card.size(); i++)
			if (m.x > 311 + 45 * i && m.x < 356 + 45 * i && m.y>590 && m.y < 720 && m.uMsg == WM_LBUTTONDOWN)
			{
				cardflash(i);//重载UI
				putimage(311 + 45 * i, 560, &kapai[wanjia[0].card[i]].photo);
				return i;
			}
	}
}

int askfor( string s)//询问
{
	while (TRUE)
	{
		int xuanpai = choosecard();

		int tmp = button();
		if (tmp == 2)break;
		if (tmp == 1)
		{
			if (kapai[wanjia[0].card[xuanpai]].name==s)
			{
				wanjia[0].card.erase(wanjia[0].card.begin() + xuanpai);
				flash();
				return 1;
			}
			flash();
			return 0;
		}
	}
	return 0;
}

int answerfor(string s)//AI应对询问
{
	for (int i = 0; i < wanjia[1].card.size(); i++)
		if (kapai[wanjia[1].card[i]].name == s)
		{
			wanjia[1].card.erase(wanjia[1].card.begin() + i);
			return 1;
		}		
	return 0;
}

int dying(int u)//濒死
{
	if (wanjia[u].hp == 0)
		for (int i = 0; i < wanjia[u].card.size(); i++)
			if (kapai[wanjia[u].card[i]].name == "桃")
			{
				wanjia[u].hp++;
				wanjia[u].card.erase(wanjia[u].card.begin() + i);
				flash();
			}
	if (wanjia[u].hp == 0) gameover();
	return 0;
}

int judge()//判定（由于延时锦囊没写好，判定也没有用处）
{
	int result = kapai[nowkapai[nowkapai.size() - 1]].color;
	nowkapai.pop_back();
	return result;
}
/*******************************************************************************/
//卡牌类别判断

int jiben(string s)//基本牌
{
	if (s == "桃" && wanjia[0].hp < wujiang[wanjia[0].hero].hp)
	{
		wanjia[0].hp++;
		return 1;
	}
	if (s == "杀")
	{
		if (!answerfor("闪")) wanjia[1].hp--;
		return 1;
	}
	return 0;
}

int unjinnang(string s)//非延时锦囊牌
{
	if (answerfor("无懈可击")) return 0;

	if (s == "南蛮入侵")
	{	
		if (!answerfor("杀")) wanjia[1].hp--;
		return 1;
	}
	if (s == "万箭齐发" )
	{
		if (!answerfor("闪")) wanjia[1].hp--;
		return 1;
	}
	if (s == "过河拆桥" && wanjia[1].card.size())
	{
		wanjia[1].card.pop_back();
		flash();
		return 1;
	}
	if (s == "顺手牵羊" && wanjia[1].card.size())
	{
		wanjia[0].card.push_back(wanjia[1].card[wanjia[1].card.size() - 1]);
		wanjia[1].card.pop_back();
		flash();
		return 1;
	}
	if (s == "桃园结义")
	{
		if (wanjia[0].hp < wujiang[wanjia[0].hero].hp)wanjia[0].hp++;
		if (wanjia[1].hp < wujiang[wanjia[1].hero].hp)wanjia[1].hp++;
		flash();
		return 1;
	}
	if (s == "无中生有")
	{
		getcard();
		return 1;
	}
	return 0;
}

int zhuangbei(string s)//装备牌(目前无法使用）
{
	return 1;
}

int jinnang(string s)//延时锦囊（目前无法使用）
{
	return 1;
}

/*****************************************************************************/
//玩家

int prepare(int u = 0)//准备阶段
{
	if (wanjia[u].judge)
	{
		judge();
	}
	return 0;
}

int putcard()//出牌阶段
{
	if (jineng[wanjia[0].hero].name == "英姿") getcard(0, 3);//英姿额外摸一张牌
	else getcard(0, 2);//摸牌

	while(TRUE)
	{
		int xuanpai = choosecard();
		IMAGE photo = kapai[wanjia[0].card[xuanpai]].photo;

		if (!wanjia[0].card.size()) return 0;

		int tmp = button();
		if (tmp == 3)break;
		if (tmp == 2)flash();
		if(tmp==1)
		{
			int realput=0;
			switch (kapai[wanjia[0].card[xuanpai]].type)
			{
				case 1:realput=jiben(kapai[wanjia[0].card[xuanpai]].name); break;
				case 2:realput=unjinnang(kapai[wanjia[0].card[xuanpai]].name); break;
				case 3:realput=zhuangbei(kapai[wanjia[0].card[xuanpai]].name); break;
				case 4:realput=jinnang(kapai[wanjia[0].card[xuanpai]].name); break;
			}
			if (realput)
			{
				wanjia[0].card.erase(wanjia[0].card.begin() + xuanpai);
			}
			flash();
			if (realput) putimage(492, 300, &photo);
		}

		dying(1);//AI濒死
	}
	return 0;
}

int throwcard()//弃牌阶段
{
	while (wanjia[0].card.size() > wanjia[0].hp)
	{
		flash();
		wanjia[0].card.erase(wanjia[0].card.begin()+choosecard());
		flash();
	}

	if (jineng[wanjia[0].hero].name == "闭月") getcard(0, 1);//闭月额外摸一张牌

	return 0;
}
/*******************************************************************************/
//AI卡牌类别判断

int aijiben(string s)//基本牌
{
	if (s == "桃" && wanjia[1].hp < wujiang[wanjia[1].hero].hp)
	{
		wanjia[1].hp++;
		return 1;
	}
	if (s == "杀")
	{
		if (!askfor("闪")) wanjia[0].hp--;
		return 1;
	}
	return 0;
}

int aiunjinnang(string s)//非延时锦囊牌
{
	if (answerfor("无懈可击")) return 0;

	if (s == "南蛮入侵")
	{
		if (!askfor("杀")) wanjia[0].hp--;
		return 1;
	}
	if (s == "万箭齐发")
	{
		if (!askfor("闪")) wanjia[1].hp--;
		return 1;
	}
	if (s == "过河拆桥" && wanjia[1].card.size())
	{
		wanjia[1].card.pop_back();
		flash();
		return 1;
	}
	if (s == "顺手牵羊" && wanjia[0].card.size())
	{
		wanjia[1].card.push_back(wanjia[0].card[wanjia[0].card.size() - 1]);
		wanjia[0].card.pop_back();
		flash();
		return 1;
	}
	if (s == "桃园结义")
	{
		if (wanjia[0].hp < wujiang[wanjia[0].hero].hp)wanjia[0].hp++;
		if (wanjia[1].hp < wujiang[wanjia[1].hero].hp)wanjia[1].hp++;
		flash();
		return 1;
	}
	if (s == "无中生有")
	{
		getcard(1,2);
		return 1;
	}
	return 0;
}

int aizhuangbei(string s)//装备牌(目前无法使用）
{
	return 1;
}

int aijinnang(string s)//延时锦囊（目前无法使用）
{
	return 1;
}

/**********************************************************/
//AI

int aiputcard()//AI出牌阶段（有什么牌出什么牌）
{
	if (jineng[wanjia[1].hero].name == "英姿") getcard(1, 3);//英姿额外摸一张牌
	else getcard(1, 2);//摸牌

	for (int i = 0; i < wanjia[1].card.size(); i++)
	{
		int realput = 0;
		IMAGE photo = kapai[wanjia[1].card[i]].photo;
		putimage(492, 300, &photo);
		switch (kapai[wanjia[1].card[i]].type)
		{
		case 1:realput = aijiben(kapai[wanjia[1].card[i]].name); break;
		case 2:realput = aiunjinnang(kapai[wanjia[1].card[i]].name); break;
		case 3:realput = aizhuangbei(kapai[wanjia[1].card[i]].name); break;
		case 4:realput = aijinnang(kapai[wanjia[1].card[i]].name); break;
		}
		if (realput)
		{
			wanjia[1].card.erase(wanjia[1].card.begin() + i);
		}
		flash();

		dying(0);//玩家濒死
	}
	return 0;
}

int aithrowcard()//AI弃牌阶段（默认从最后一张手牌开始弃牌）
{
	while (wanjia[1].card.size() > wanjia[1].hp)
	{
		flash();

		wanjia[1].card.pop_back();

		flash();
	}

	if (jineng[wanjia[1].hero].name == "闭月") getcard(1, 1);//闭月额外摸一张牌

	return 0;
}

/**********************************************************/
//游戏持续进行

int round()//回合
{

	while (TRUE)//只要不出意外，回合无限进行
	{
		prepare(0);//准备阶段
		putcard();//出牌阶段
		throwcard();//弃牌阶段
		//玩家阶段

		prepare(1);//AI准备阶段
		aiputcard();//AI出牌阶段
		aithrowcard();//AI弃牌阶段
		//AI阶段
	}

	return 0;
}

int gamestart()//开局
{
	choosehero();//选将

	flash();//载入UI

	washcard();//洗牌

	getcard(0, 4);//玩家初始摸牌

	getcard(1, 4);//AI初始摸牌

	round();//回合正式开始

	return 0;
}

/**********************************************************/
//初始化

int init()//变量初始化，又臭又长，建议不要看
{
	//武将 
	wujiang[1].name = "曹操"; wujiang[1].force = 1; wujiang[1].gender = 1; wujiang[1].skill = 1; loadimage(&wujiang[1].photo, "photo//caocao.png", 240, 400); wujiang[1].hp = 4;//曹操 
	wujiang[2].name = "郭嘉"; wujiang[2].force = 1; wujiang[2].gender = 1; wujiang[2].skill = 2; loadimage(&wujiang[2].photo, "photo//guojia.png", 240, 400); wujiang[2].hp = 3;//郭嘉 
	wujiang[3].name = "司马懿"; wujiang[3].force = 1; wujiang[3].gender = 1; wujiang[3].skill = 3; loadimage(&wujiang[3].photo, "photo//simayi.png", 240, 400); wujiang[3].hp = 3;//司马懿 
	wujiang[4].name = "夏侯惇"; wujiang[4].force = 1; wujiang[4].gender = 1; wujiang[4].skill = 4; loadimage(&wujiang[4].photo, "photo//xiahoudun.png", 240, 400); wujiang[4].hp = 4;//夏侯敦 
	wujiang[5].name = "许诸"; wujiang[5].force = 1; wujiang[5].gender = 1; wujiang[5].skill = 5; loadimage(&wujiang[5].photo, "photo//xuzhu.png", 240, 400); wujiang[5].hp = 4;//许诸 
	wujiang[6].name = "张辽"; wujiang[6].force = 1; wujiang[6].gender = 1; wujiang[6].skill = 6; loadimage(&wujiang[6].photo, "photo//zhangliao.png", 240, 400); wujiang[6].hp = 4;//张辽 
	wujiang[7].name = "甄姬"; wujiang[7].force = 1; wujiang[7].gender = 0; wujiang[7].skill = 7; loadimage(&wujiang[7].photo, "photo//zhenji.png", 240, 400); wujiang[7].hp = 3;//甄姬 
	wujiang[8].name = "刘备"; wujiang[8].force = 2; wujiang[8].gender = 1; wujiang[8].skill = 8; loadimage(&wujiang[8].photo, "photo//liubei.png", 240, 400); wujiang[8].hp = 4;//刘备 
	wujiang[9].name = "关羽"; wujiang[9].force = 2; wujiang[9].gender = 1; wujiang[9].skill = 9; loadimage(&wujiang[9].photo, "photo//guanyu.png", 240, 400); wujiang[9].hp = 4;//关羽 
	wujiang[10].name = "黄月英"; wujiang[10].force = 2; wujiang[10].gender = 0; wujiang[10].skill = 10; loadimage(&wujiang[10].photo, "photo//huangyueying.png", 240, 400); wujiang[10].hp = 3;//黄月英 
	wujiang[11].name = "马超"; wujiang[11].force = 2; wujiang[11].gender = 1; wujiang[11].skill = 11; loadimage(&wujiang[11].photo, "photo//machao.png", 240, 400); wujiang[11].hp = 4;//马超 
	wujiang[12].name = "张飞"; wujiang[12].force = 2; wujiang[12].gender = 1; wujiang[12].skill = 12; loadimage(&wujiang[12].photo, "photo//zhangfei.png", 240, 400); wujiang[12].hp = 4;//张飞 
	wujiang[13].name = "赵云"; wujiang[13].force = 2; wujiang[13].gender = 1; wujiang[13].skill = 13; loadimage(&wujiang[13].photo, "photo//zhaoyun.png", 240, 400); wujiang[13].hp = 4;//赵云 
	wujiang[14].name = "诸葛亮"; wujiang[14].force = 2; wujiang[14].gender = 1; wujiang[14].skill = 14; loadimage(&wujiang[14].photo, "photo//zhugeliang.png", 240, 400); wujiang[14].hp = 3;//诸葛亮 
	wujiang[15].name = "孙权"; wujiang[15].force = 3; wujiang[15].gender = 1; wujiang[15].skill = 15; loadimage(&wujiang[15].photo, "photo//sunquan.png", 240, 400); wujiang[15].hp = 4;//孙权 
	wujiang[16].name = "大乔"; wujiang[16].force = 3; wujiang[16].gender = 0; wujiang[16].skill = 16; loadimage(&wujiang[16].photo, "photo//daqiao.png", 240, 400); wujiang[16].hp = 3;//大乔 
	wujiang[17].name = "甘宁"; wujiang[17].force = 3; wujiang[17].gender = 1; wujiang[17].skill = 17; loadimage(&wujiang[17].photo, "photo//ganning.png", 240, 400); wujiang[17].hp = 4;//甘宁 
	wujiang[18].name = "黄盖"; wujiang[18].force = 3; wujiang[18].gender = 1; wujiang[18].skill = 18; loadimage(&wujiang[18].photo, "photo//huanggai.png", 240, 400); wujiang[18].hp = 4;//黄盖 
	wujiang[19].name = "吕蒙"; wujiang[19].force = 3; wujiang[19].gender = 1; wujiang[19].skill = 19; loadimage(&wujiang[19].photo, "photo//lvmeng.png", 240, 400); wujiang[19].hp = 4;//吕蒙 
	wujiang[20].name = "陆逊"; wujiang[20].force = 3; wujiang[20].gender = 1; wujiang[20].skill = 20; loadimage(&wujiang[20].photo, "photo//luxun.png", 240, 400); wujiang[20].hp = 3;//陆逊 
	wujiang[21].name = "孙尚香"; wujiang[21].force = 3; wujiang[21].gender = 0; wujiang[21].skill = 21; loadimage(&wujiang[21].photo, "photo//sunshangxiang.png", 240, 400); wujiang[21].hp = 3;//孙尚香 
	wujiang[22].name = "周瑜"; wujiang[22].force = 3; wujiang[22].gender = 1; wujiang[22].skill = 22; loadimage(&wujiang[22].photo, "photo//zhouyu.png", 240, 400); wujiang[22].hp = 3;//周瑜 
	wujiang[23].name = "吕布"; wujiang[23].force = 0; wujiang[23].gender = 1; wujiang[23].skill = 23; loadimage(&wujiang[23].photo, "photo//lvbu.png", 240, 400); wujiang[23].hp = 4;//吕布 
	wujiang[24].name = "貂蝉"; wujiang[24].force = 0; wujiang[24].gender = 0; wujiang[24].skill = 24; loadimage(&wujiang[24].photo, "photo//diaochan.png", 240, 400); wujiang[24].hp = 3;//貂蝉 
	wujiang[25].name = "华佗"; wujiang[25].force = 0; wujiang[25].gender = 1; wujiang[25].skill = 25; loadimage(&wujiang[25].photo, "photo//huatuo.png", 240, 400); wujiang[25].hp = 3;//华佗 

	//卡牌 
	kapai[0].name = "背面"; kapai[0].color = 0; kapai[0].type = 0; loadimage(&kapai[0].photo, "photo//back.png", 93, 130);
	kapai[1].name = "桃园结义"; kapai[1].color = 1; kapai[1].type = 2; loadimage(&kapai[1].photo, "photo//1.png", 93, 130);
	kapai[2].name = "闪"; kapai[2].color = 1; kapai[2].type = 1; loadimage(&kapai[2].photo, "photo//2.png", 93, 130);
	kapai[3].name = "桃"; kapai[3].color = 1; kapai[3].type = 1; loadimage(&kapai[3].photo, "photo//3.png", 93, 130);
	kapai[4].name = "桃"; kapai[4].color = 1; kapai[4].type = 1; loadimage(&kapai[4].photo, "photo//4.png", 93, 130);
	kapai[5].name = "赤兔"; kapai[5].color = 1; kapai[5].type = 3; loadimage(&kapai[5].photo, "photo//5.png", 93, 130);
	kapai[6].name = "桃"; kapai[6].color = 1; kapai[6].type = 1; loadimage(&kapai[6].photo, "photo//6.png", 93, 130);
	kapai[7].name = "桃"; kapai[7].color = 1; kapai[7].type = 1; loadimage(&kapai[7].photo, "photo//7.png", 93, 130);
	kapai[8].name = "桃"; kapai[8].color = 1; kapai[8].type = 1; loadimage(&kapai[8].photo, "photo//8.png", 93, 130);
	kapai[9].name = "桃"; kapai[9].color = 1; kapai[9].type = 1; loadimage(&kapai[9].photo, "photo//9.png", 93, 130);
	kapai[10].name = "杀"; kapai[10].color = 1; kapai[10].type = 1; loadimage(&kapai[10].photo, "photo//10.png", 93, 130);
	kapai[11].name = "杀"; kapai[11].color = 1; kapai[11].type = 1; loadimage(&kapai[11].photo, "photo//11.png", 93, 130);
	kapai[12].name = "桃"; kapai[12].color = 1; kapai[12].type = 1; loadimage(&kapai[12].photo, "photo//12.png", 93, 130);
	kapai[13].name = "闪"; kapai[13].color = 1; kapai[13].type = 1; loadimage(&kapai[13].photo, "photo//13.png", 93, 130);
	kapai[14].name = "万箭齐发"; kapai[14].color = 1; kapai[14].type = 2; loadimage(&kapai[14].photo, "photo//14.png", 93, 130);
	kapai[15].name = "闪"; kapai[15].color = 1; kapai[15].type = 1; loadimage(&kapai[15].photo, "photo//15.png", 93, 130);
	kapai[16].name = "五谷丰登"; kapai[16].color = 1; kapai[16].type = 2; loadimage(&kapai[16].photo, "photo//16.png", 93, 130);
	kapai[17].name = "五谷丰登"; kapai[17].color = 1; kapai[17].type = 2; loadimage(&kapai[17].photo, "photo//17.png", 93, 130);
	kapai[18].name = "麒麟弓"; kapai[18].color = 1; kapai[18].type = 3; loadimage(&kapai[18].photo, "photo//18.png", 93, 130);
	kapai[19].name = "乐不思蜀"; kapai[19].color = 1; kapai[19].type = 4; loadimage(&kapai[19].photo, "photo//19.png", 93, 130);
	kapai[20].name = "无中生有"; kapai[20].color = 1; kapai[20].type = 2; loadimage(&kapai[20].photo, "photo//20.png", 93, 130);
	kapai[21].name = "无中生有"; kapai[21].color = 1; kapai[21].type = 2; loadimage(&kapai[21].photo, "photo//21.png", 93, 130);
	kapai[22].name = "无中生有"; kapai[22].color = 1; kapai[22].type = 2; loadimage(&kapai[22].photo, "photo//22.png", 93, 130);
	kapai[23].name = "杀"; kapai[23].color = 1; kapai[23].type = 1; loadimage(&kapai[23].photo, "photo//23.png", 93, 130);
	kapai[24].name = "无中生有"; kapai[24].color = 1; kapai[24].type = 2; loadimage(&kapai[24].photo, "photo//24.png", 93, 130);
	kapai[25].name = "过河拆桥"; kapai[25].color = 1; kapai[25].type = 2; loadimage(&kapai[25].photo, "photo//25.png", 93, 130);
	kapai[26].name = "的卢"; kapai[26].color = 1; kapai[26].type = 3; loadimage(&kapai[26].photo, "photo//26.png", 93, 130);
	kapai[27].name = "决斗"; kapai[27].color = 2; kapai[27].type = 2; loadimage(&kapai[27].photo, "photo//27.png", 93, 130);
	kapai[28].name = "闪"; kapai[28].color = 2; kapai[28].type = 1; loadimage(&kapai[28].photo, "photo//28.png", 93, 130);
	kapai[29].name = "闪"; kapai[29].color = 2; kapai[29].type = 1; loadimage(&kapai[29].photo, "photo//29.png", 93, 130);
	kapai[30].name = "闪"; kapai[30].color = 2; kapai[30].type = 1; loadimage(&kapai[30].photo, "photo//30.png", 93, 130);
	kapai[31].name = "闪"; kapai[31].color = 2; kapai[31].type = 1; loadimage(&kapai[31].photo, "photo//31.png", 93, 130);
	kapai[32].name = "闪"; kapai[32].color = 2; kapai[32].type = 1; loadimage(&kapai[32].photo, "photo//32.png", 93, 130);
	kapai[33].name = "闪"; kapai[33].color = 2; kapai[33].type = 1; loadimage(&kapai[33].photo, "photo//33.png", 93, 130);
	kapai[34].name = "闪"; kapai[34].color = 2; kapai[34].type = 1; loadimage(&kapai[34].photo, "photo//34.png", 93, 130);
	kapai[35].name = "闪"; kapai[35].color = 2; kapai[35].type = 1; loadimage(&kapai[35].photo, "photo//35.png", 93, 130);
	kapai[36].name = "闪"; kapai[36].color = 2; kapai[36].type = 1; loadimage(&kapai[36].photo, "photo//36.png", 93, 130);
	kapai[37].name = "闪"; kapai[37].color = 2; kapai[37].type = 1; loadimage(&kapai[37].photo, "photo//37.png", 93, 130);
	kapai[38].name = "桃"; kapai[38].color = 2; kapai[38].type = 1; loadimage(&kapai[38].photo, "photo//38.png", 93, 130);
	kapai[39].name = "杀"; kapai[39].color = 2; kapai[39].type = 1; loadimage(&kapai[39].photo, "photo//39.png", 93, 130);
	kapai[40].name = "诸葛连弩"; kapai[40].color = 2; kapai[40].type = 3; loadimage(&kapai[40].photo, "photo//40.png", 93, 130);
	kapai[41].name = "闪"; kapai[41].color = 2; kapai[41].type = 1; loadimage(&kapai[41].photo, "photo//41.png", 93, 130);
	kapai[42].name = "顺手牵羊"; kapai[42].color = 2; kapai[42].type = 2; loadimage(&kapai[42].photo, "photo//42.png", 93, 130);
	kapai[43].name = "顺手牵羊"; kapai[43].color = 2; kapai[43].type = 2; loadimage(&kapai[43].photo, "photo//43.png", 93, 130);
	kapai[44].name = "贯石斧"; kapai[44].color = 2; kapai[44].type = 3; loadimage(&kapai[44].photo, "photo//44.png", 93, 130);
	kapai[45].name = "杀"; kapai[45].color = 2; kapai[45].type = 1; loadimage(&kapai[45].photo, "photo//45.png", 93, 130);
	kapai[46].name = "杀"; kapai[46].color = 2; kapai[46].type = 1; loadimage(&kapai[46].photo, "photo//46.png", 93, 130);
	kapai[47].name = "杀"; kapai[47].color = 2; kapai[47].type = 1; loadimage(&kapai[47].photo, "photo//47.png", 93, 130);
	kapai[48].name = "杀"; kapai[48].color = 2; kapai[48].type = 1; loadimage(&kapai[48].photo, "photo//48.png", 93, 130);
	kapai[49].name = "杀"; kapai[49].color = 2; kapai[49].type = 1; loadimage(&kapai[49].photo, "photo//49.png", 93, 130);
	kapai[50].name = "闪"; kapai[50].color = 2; kapai[50].type = 1; loadimage(&kapai[50].photo, "photo//50.png", 93, 130);
	kapai[51].name = "无懈可击"; kapai[51].color = 2; kapai[51].type = 2; loadimage(&kapai[51].photo, "photo//51.png", 93, 130);
	kapai[52].name = "大宛"; kapai[52].color = 2; kapai[52].type = 3; loadimage(&kapai[52].photo, "photo//52.png", 93, 130);
	kapai[53].name = "闪电"; kapai[53].color = 3; kapai[53].type = 4; loadimage(&kapai[53].photo, "photo//53.png", 93, 130);
	kapai[54].name = "雌雄双股剑"; kapai[54].color = 3; kapai[54].type = 3; loadimage(&kapai[54].photo, "photo//54.png", 93, 130);
	kapai[55].name = "顺手牵羊"; kapai[55].color = 3; kapai[55].type = 2; loadimage(&kapai[55].photo, "photo//55.png", 93, 130);
	kapai[56].name = "顺手牵羊"; kapai[56].color = 3; kapai[56].type = 2; loadimage(&kapai[56].photo, "photo//56.png", 93, 130);
	kapai[57].name = "绝影"; kapai[57].color = 3; kapai[57].type = 3; loadimage(&kapai[57].photo, "photo//57.png", 93, 130);
	kapai[58].name = "乐不思蜀"; kapai[58].color = 3; kapai[58].type = 4; loadimage(&kapai[58].photo, "photo//58.png", 93, 130);
	kapai[59].name = "杀"; kapai[59].color = 3; kapai[59].type = 1; loadimage(&kapai[59].photo, "photo//59.png", 93, 130);
	kapai[60].name = "杀"; kapai[60].color = 3; kapai[60].type = 1; loadimage(&kapai[60].photo, "photo//60.png", 93, 130);
	kapai[61].name = "杀"; kapai[61].color = 3; kapai[61].type = 1; loadimage(&kapai[61].photo, "photo//61.png", 93, 130);
	kapai[62].name = "杀"; kapai[62].color = 3; kapai[62].type = 1; loadimage(&kapai[62].photo, "photo//62.png", 93, 130);
	kapai[63].name = "无懈可击"; kapai[63].color = 3; kapai[63].type = 2; loadimage(&kapai[63].photo, "photo//63.png", 93, 130);
	kapai[64].name = "过河拆桥"; kapai[64].color = 3; kapai[64].type = 2; loadimage(&kapai[64].photo, "photo//64.png", 93, 130);
	kapai[65].name = "紫骍"; kapai[65].color = 3; kapai[65].type = 3; loadimage(&kapai[65].photo, "photo//65.png", 93, 130);
	kapai[66].name = "决斗"; kapai[66].color = 3; kapai[66].type = 2; loadimage(&kapai[66].photo, "photo//66.png", 93, 130);
	kapai[67].name = "寒冰剑"; kapai[67].color = 3; kapai[67].type = 3; loadimage(&kapai[67].photo, "photo//67.png", 93, 130);
	kapai[68].name = "过河拆桥"; kapai[68].color = 3; kapai[68].type = 2; loadimage(&kapai[68].photo, "photo//68.png", 93, 130);
	kapai[69].name = "过河拆桥"; kapai[69].color = 3; kapai[69].type = 2; loadimage(&kapai[69].photo, "photo//69.png", 93, 130);
	kapai[70].name = "青龙偃月刀"; kapai[70].color = 3; kapai[70].type = 3; loadimage(&kapai[70].photo, "photo//70.png", 93, 130);
	kapai[71].name = "青釭剑"; kapai[71].color = 3; kapai[71].type = 3; loadimage(&kapai[71].photo, "photo//71.png", 93, 130);
	kapai[72].name = "南蛮入侵"; kapai[72].color = 3; kapai[72].type = 2; loadimage(&kapai[72].photo, "photo//72.png", 93, 130);
	kapai[73].name = "杀"; kapai[73].color = 3; kapai[73].type = 1; loadimage(&kapai[73].photo, "photo//73.png", 93, 130);
	kapai[74].name = "杀"; kapai[74].color = 3; kapai[74].type = 1; loadimage(&kapai[74].photo, "photo//74.png", 93, 130);
	kapai[75].name = "杀"; kapai[75].color = 3; kapai[75].type = 1; loadimage(&kapai[75].photo, "photo//75.png", 93, 130);
	kapai[76].name = "顺手牵羊"; kapai[76].color = 3; kapai[76].type = 2; loadimage(&kapai[76].photo, "photo//76.png", 93, 130);
	kapai[77].name = "丈八蛇矛"; kapai[77].color = 3; kapai[77].type = 3; loadimage(&kapai[77].photo, "photo//77.png", 93, 130);
	kapai[78].name = "南蛮入侵"; kapai[78].color = 3; kapai[78].type = 2; loadimage(&kapai[78].photo, "photo//78.png", 93, 130);
	kapai[79].name = "决斗"; kapai[79].color = 4; kapai[79].type = 2; loadimage(&kapai[79].photo, "photo//79.png", 93, 130);
	kapai[80].name = "杀"; kapai[80].color = 4; kapai[80].type = 1; loadimage(&kapai[80].photo, "photo//80.png", 93, 130);
	kapai[81].name = "杀"; kapai[81].color = 4; kapai[81].type = 1; loadimage(&kapai[81].photo, "photo//81.png", 93, 130);
	kapai[82].name = "杀"; kapai[82].color = 4; kapai[82].type = 1; loadimage(&kapai[82].photo, "photo//82.png", 93, 130);
	kapai[83].name = "杀"; kapai[83].color = 4; kapai[83].type = 1; loadimage(&kapai[83].photo, "photo//83.png", 93, 130);
	kapai[84].name = "杀"; kapai[84].color = 4; kapai[84].type = 1; loadimage(&kapai[84].photo, "photo//84.png", 93, 130);
	kapai[85].name = "杀"; kapai[85].color = 4; kapai[85].type = 1; loadimage(&kapai[85].photo, "photo//85.png", 93, 130);
	kapai[86].name = "杀"; kapai[86].color = 4; kapai[86].type = 1; loadimage(&kapai[86].photo, "photo//86.png", 93, 130);
	kapai[87].name = "杀"; kapai[87].color = 4; kapai[87].type = 1; loadimage(&kapai[87].photo, "photo//87.png", 93, 130);
	kapai[88].name = "杀"; kapai[88].color = 4; kapai[88].type = 1; loadimage(&kapai[88].photo, "photo//88.png", 93, 130);
	kapai[89].name = "杀"; kapai[89].color = 4; kapai[89].type = 1; loadimage(&kapai[89].photo, "photo//89.png", 93, 130);
	kapai[90].name = "无懈可击"; kapai[90].color = 4; kapai[90].type = 2; loadimage(&kapai[90].photo, "photo//90.png", 93, 130);
	kapai[91].name = "无懈可击"; kapai[91].color = 4; kapai[91].type = 2; loadimage(&kapai[91].photo, "photo//91.png", 93, 130);
	kapai[92].name = "诸葛连弩"; kapai[92].color = 4; kapai[92].type = 3; loadimage(&kapai[92].photo, "photo//92.png", 93, 130);
	kapai[93].name = "仁王盾"; kapai[93].color = 4; kapai[93].type = 3; loadimage(&kapai[93].photo, "photo//93.png", 93, 130);
	kapai[94].name = "过河拆桥"; kapai[94].color = 4; kapai[94].type = 2; loadimage(&kapai[94].photo, "photo//94.png", 93, 130);
	kapai[95].name = "过河拆桥"; kapai[95].color = 4; kapai[95].type = 2; loadimage(&kapai[95].photo, "photo//95.png", 93, 130);
	kapai[96].name = "诸葛连弩"; kapai[96].color = 4; kapai[96].type = 3; loadimage(&kapai[96].photo, "photo//96.png", 93, 130);
	kapai[97].name = "乐不思蜀"; kapai[97].color = 4; kapai[97].type = 4; loadimage(&kapai[97].photo, "photo//97.png", 93, 130);
	kapai[98].name = "南蛮入侵"; kapai[98].color = 4; kapai[98].type = 2; loadimage(&kapai[98].photo, "photo//98.png", 93, 130);
	kapai[99].name = "杀"; kapai[99].color = 4; kapai[99].type = 1; loadimage(&kapai[99].photo, "photo//99.png", 93, 130);
	kapai[100].name = "杀"; kapai[100].color = 4; kapai[100].type = 1; loadimage(&kapai[100].photo, "photo//100.png", 93, 130);
	kapai[101].name = "杀"; kapai[101].color = 4; kapai[101].type = 1; loadimage(&kapai[101].photo, "photo//101.png", 93, 130);
	kapai[102].name = "杀"; kapai[102].color = 4; kapai[102].type = 1; loadimage(&kapai[102].photo, "photo//102.png", 93, 130);
	kapai[103].name = "借刀杀人"; kapai[103].color = 4; kapai[103].type = 2; loadimage(&kapai[103].photo, "photo//103.png", 93, 130);
	kapai[104].name = "借刀杀人"; kapai[104].color = 4; kapai[104].type = 2; loadimage(&kapai[104].photo, "photo//104.png", 93, 130);
	kapai[105].name = "方天画戟"; kapai[105].color = 2; kapai[105].type = 3; loadimage(&kapai[105].photo, "photo//105.png", 93, 130);
	kapai[106].name = "八卦阵"; kapai[106].color = 4; kapai[106].type = 3; loadimage(&kapai[106].photo, "photo//106.png", 93, 130);
	kapai[107].name = "八卦阵"; kapai[107].color = 3; kapai[107].type = 3; loadimage(&kapai[107].photo, "photo//107.png", 93, 130);
	kapai[108].name = "闪电"; kapai[108].color = 1; kapai[108].type = 4; loadimage(&kapai[108].photo, "photo//108.png", 93, 130);

	//技能
	jineng[1].name = "奸雄"; jineng[1].type = 0;
	jineng[2].name = "天妒"; jineng[1].type = 0;
	jineng[3].name = "反馈"; jineng[1].type = 0;
	jineng[4].name = "刚烈"; jineng[1].type = 0;
	jineng[5].name = "裸衣"; jineng[1].type = 0;
	jineng[6].name = "突袭"; jineng[1].type = 0;
	jineng[7].name = "倾国"; jineng[1].type = 0;
	jineng[8].name = "仁望"; jineng[1].type = 0;
	jineng[9].name = "武圣"; jineng[1].type = 0;
	jineng[10].name = "集智"; jineng[1].type = 1;
	jineng[11].name = "铁骑"; jineng[1].type = 0;
	jineng[12].name = "咆哮"; jineng[1].type = 1;
	jineng[13].name = "龙胆"; jineng[1].type = 0;
	jineng[14].name = "空城"; jineng[1].type = 1;
	jineng[15].name = "制衡"; jineng[1].type = 0;
	jineng[16].name = "国色"; jineng[1].type = 0;
	jineng[17].name = "奇袭"; jineng[1].type = 0;
	jineng[18].name = "苦肉"; jineng[1].type = 0;
	jineng[19].name = "克己"; jineng[1].type = 0;
	jineng[20].name = "连营"; jineng[1].type = 1;
	jineng[21].name = "枭姬"; jineng[1].type = 0;
	jineng[22].name = "英姿"; jineng[1].type = 1;
	jineng[23].name = "无双"; jineng[1].type = 1;
	jineng[24].name = "闭月"; jineng[1].type = 1;
	jineng[25].name = "青囊"; jineng[1].type = 0;

	//玩家
	wanjia[0].judge = 0;
	wanjia[1].judge = 0;
	wanjia[0].hp = 0;
	wanjia[1].hp = 0;
	wanjia[0].hero = 0;
	wanjia[1].hero = 0;

	return 0;
}