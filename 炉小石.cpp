#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<conio.h>
#include<iostream>
#include<windows.h>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;
struct node{
	int cost,attack,health,death,treat,quality,time,hurt,special;
	int spell,level,beam;
};
FILE *data;
node deck[3][70],hand[3][20],nu,war[3][20],swap1;
node card[4],card1[40],card2[40];
int mode;//��Ϸģʽ
int dotask1,dotask2;//�������
int ac[20],chapter;//ͨ��ð��ģʽ��ð�չؿ�
int flag[3][70];
int hea[3];//Ӣ��Ѫ��
int num1,num2,hf1[20],hf2[20],tot1,tot2,fa1,fa2;//tot���ƿ⣬num�����ƣ�fa��ƣ��
int top1,top2;//�ƿⶥ
int mana1,mana2,tma1,tma2;//ˮ��������
int alive[3],maxhealth;
int fc1,fc2,over;//��������Ϸ����
int X,Y,ix,iy,jx,jy,xx,yy;
int hpcost[10]={0,2,4,0,1,2,1,2,2,5};//Ӣ�ۼ��ܷ���
int spcost[40]={0,2,2,2,2,0,6,0,1,3,0,2,2,2,2,3,1,3,5,2,0,3,1,2,2,1,6,2};
int win[10],lose[10],twin,tlose;
int hpuse1,hpuse2,maxhp2;//���غ��Ƿ�ʹ����Ӣ�ۼ���
char occu[10][40]={
"","�� ��","�� ѧ","Ӣ ��","�� ��","�� ѧ","�� ��","�� ��","�� ʷ","�� ��"
};
char name[50],name1[50]="ѧ ��";//��������
int clas;
int en_sum;
void HideCursor()//���ؿ���̨�Ĺ��
{
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
void search()//�������λ��
{
	LONG zx=-1;
	LONG zy=-1;
	POINT ptB={0,0};
	LPPOINT xy=&ptB;  //λ�ñ���
	GetCursorPos(xy);   //��ȡ��굱ǰλ��
	if ((zx!=xy->x) || (zy!=xy->y))
	{
		X=xy->x;
		Y=xy->y;
	}
}
void gotoxy(int y,int x)//�ı��������λ��
{
    CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;
    HANDLE    hConsoleOut;
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);
    csbiInfo.dwCursorPosition.X = x;
    csbiInfo.dwCursorPosition.Y = y;
    SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}
void swap(int &x,int &y)//����
{
	int t;
	t=x;x=y;y=t;
}
void change(int x,int y)//��ѧ������������ĸ���
{
	yy=(double)(x-ix)/(jx-ix)*100+0.5;
	xx=(double)(y-iy)/(jy-iy)*34+0.5;
}
void pause()//���ո������
{
	printf("���ո��������\n");
	char c1=0;
	while(c1!=' ') c1=getch();
}
void calibrate()//У׼��Ļ
{
	system("cls");
	printf("O  <-������ԲȦ������\n");
	while(1)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			ix=X,iy=Y;
			printf("(%d,%d)�ܺã�\n",ix,iy);
			break;
		}
	}
	pause();
	for(int i=1;i<=31;i++) printf("\n");
	for(int i=1;i<=70;i++) printf(" ");
	printf("        ������ԲȦ������->  O\n");
	while(1)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			jx=X,jy=Y;
			printf("(%d,%d)�ܺã�\n��Ҫ���ƶ���Ĵ����ˣ�����\n",jx,jy);
			break;
		}
	}
	pause();

}
void initialize()//��ʼ������
{
	printf("���ڼ��أ����Ժ󡭡���������\n");
	Sleep(100);
	srand(time(0));
	HideCursor();
	data=fopen("data.txt","r");
	if(data!=NULL)
	{
		for(int i=1;i<=9;i++) fscanf(data,"%d%d",&win[i],&lose[i]);
		fscanf(data,"%d%d",&twin,&tlose);
		for(int i=1;i<=9;i++) fscanf(data,"%d",&ac[i]);
		fscanf(data,"\n");
		fgets(name,9,data);
		if(strlen(name)==0) strcpy(name,name1);
		fclose(data);
	}
	if(strlen(name)==0) strcpy(name,name1);
	system("color 60");
	system("mode con cols=120 lines=40");
	printf("��������ɣ���������У׼ģʽ\n");
	pause();
	calibrate();
}
void clear()//������Ļ�������ս����
{
	system("cls");
	gotoxy(35,0);
	for(int i=1;i<=100;i++) printf("-");
	printf("/");
	for(int i=1;i<=35;i++) gotoxy(i-1,100),printf("|");
	gotoxy(0,0);
}
void create(int k)//�����ƿ�
{
	int t;
	for(int i=1;i<=(k==1?tot1:tot2);i++)
	{
		t=rand()%12;
		if(k==1 && mode==4 && clas==7 && dotask1>=6) t=0;
		if(k==2 && chapter==9) t=0;
		if(t<9)
		{
			t=rand()%100;
			if(mode==4 && clas==7 && dotask1>=6) t=0;
			if(k==1 && chapter==7) t=66;
			if(k==2 && chapter==9) t=0;
			if(t==0) deck[k][i].quality=4;
			else if(t<=5) deck[k][i].quality=3;
			else if(t>=80) deck[k][i].quality=2;
			else deck[k][i].quality=1;
			if(k==2 && chapter==8 && deck[k][i].quality==1) deck[k][i].quality=2;
			deck[k][i].attack=rand()%10+1;
			deck[k][i].health=rand()%10+1;
			deck[k][i].cost=rand()%10+1;
			deck[k][i].spell=0;
			if(k==1 && chapter==3) deck[k][i].cost=(rand()%5+1)*2;
			deck[k][i].time=1;
			while(deck[k][i].attack<deck[k][i].cost-4) deck[k][i].attack=rand()%10+1;
			while(deck[k][i].health<deck[k][i].cost-4) deck[k][i].health=rand()%10+1;
			if(deck[k][i].quality==4)
			{
				deck[k][i].time=rand()%3+2;
				if(deck[k][i].health<deck[k][i].cost+1) deck[k][i].health=deck[k][i].cost+1;
				if(deck[k][i].health>10) deck[k][i].health=10;
			}
			if(deck[k][i].quality==3)
			{
				deck[k][i].death=rand()%6+2;
				int t1=deck[k][i].attack,t2=deck[k][i].health;
				deck[k][i].attack=deck[k][i].health=(t1+t2+1)/2;
			}
			if(deck[k][i].quality==2)
			{
				deck[k][i].treat=rand()%5+1;
				if(deck[k][i].attack>deck[k][i].cost*2) deck[k][i].attack=deck[k][i].cost*2+1;
				if(deck[k][i].health>deck[k][i].cost*2) deck[k][i].health=deck[k][i].cost*2+1;
			}
			if(deck[k][i].quality==1)
			{
				while(deck[k][i].attack>deck[k][i].cost*2) deck[k][i].attack=rand()%10+1;
				while(deck[k][i].health>deck[k][i].cost*2) deck[k][i].health=rand()%10+1;
			}
			if(deck[k][i].attack<deck[k][i].cost-4) deck[k][i].attack=deck[k][i].cost-4;
			if(deck[k][i].health<deck[k][i].cost-4) deck[k][i].health=deck[k][i].cost-4;
			if(k==2 && chapter==4) deck[k][i].cost--;
		}
		else
		{
			t=rand()%100;
			if(k==1 && chapter==7) t=66;
			if(t<=20) deck[k][i].quality=3;
			else if(t>=70) deck[k][i].quality=2;
			else deck[k][i].quality=1;
			if(k==2 && chapter==8 && deck[k][i].quality==1) deck[k][i].quality=2;
			if(k==1) deck[k][i].cost=spcost[(clas-1)*3+deck[k][i].quality];
			else deck[k][i].cost=3;
			deck[k][i].spell=1;
		}
	}
}
void print1(int t)//�����ʼ����
{
	if(!deck[1][t].spell)
	{
		printf("\t�����:%d/%d-%d",deck[1][t].cost,deck[1][t].attack,deck[1][t].health);
		if(deck[1][t].quality==1) printf("(��ͨ)\n");
		if(deck[1][t].quality==2) printf("(ϡ��):ս��Ϊ���Ӣ�ۻָ� %d ������ֵ��\n",deck[1][t].treat);
		if(deck[1][t].quality==3) printf("(ʷʫ):����Եз�Ӣ����� %d ���˺���\n",deck[1][t].death);
		if(deck[1][t].quality==4) printf("(��˵):�����ŭ�� %d ��\n",deck[1][t].time);
	}
	else
	{
		printf("\t������:%d/",deck[1][t].cost);
		if(deck[1][t].quality==1)
		{
			if(clas==1) printf("(��ͨ)��ʹ������С��鼮�����+2+1");
			if(clas==2) printf("(��ͨ)���ٻ�һ��2-3�ġ�ֱ�ߡ�");
			if(clas==3) printf("(��ͨ): I only know this is a common cell card.");
			if(clas==4) printf("(��ͨ)�����ڱ��غϣ����һ�㷨��ˮ�����Ҳ��ܳ���10��");
			if(clas==5) printf("(��ͨ)����һ���ƣ�������غ�ʹ�ù�Ӣ�ۼ��ܣ����Ϊ��������");
			if(clas==6) printf("(��ͨ)���ٻ�һ����ϸ������һ����");
			if(clas==7) printf("(��ͨ)����2���ƣ���һ�Ŵ�˵����ϴ������ֵ��ƿ�");
			if(clas==8) printf("(��ͨ)����һ���ƣ�������Ʊ����ã����Զ��ͷ�");
			if(clas==9) printf("(��ͨ)��������ƿ��ｫ1�ŷ����Ƶĸ��������������");
			printf("\n");

		}
		if(deck[1][t].quality==2)
		{
			if(clas==1) printf("(ϡ��)����ÿ����һ�����鼮������һ����");
			if(clas==2) printf("(ϡ��)������������з���ˮ����Ȼ��ÿ����2�㷨��ˮ�����һ����");
			if(clas==3) printf("(ϡ��): I only know this is a rare cell card.");
			if(clas==4) printf("(ϡ��)����ÿ����һ�����������һ����");
			if(clas==5) printf("(ϡ��)��ʹ������ӵ�����ֵ��Ϊ1");
			if(clas==6) printf("(ϡ��)��ʹ�з����Ҳ���ӱ�Ϊ0-0");
			if(clas==7) printf("(ϡ��)��ʹ����������ƻ��-1/+1+1������ƿ�ÿ��һ���ƣ����Ƶķ���ֵ���ı�����1��");
			if(clas==8) printf("(ϡ��)�������ֵ�һ������ϴ������ƿ�");
			if(clas==9) printf("(ϡ��)���ٻ����ƿ���һ��6�������ϵ���ӵĸ���");
			printf("\n");
		}
		if(deck[1][t].quality==3)
		{
			if(clas==1) printf("(ʷʫ)��ʹ��������鼮�����������");
			if(clas==2) printf("(ʷʫ)������ķ���ˮ����Ϊ������2-2�ġ�ֱ�ߡ�");
			if(clas==3) printf("(ʷʫ): I only know this is an epic cell card.");
			if(clas==4) printf("(ʷʫ)���ݻ�������з���ˮ�����ٻ�һ������ֵ��С��6�Ĵ�˵��ӡ��⡱���ڸ����������ԭ��ķ���ˮ��");
			if(clas==5) printf("(ʷʫ)���Ƴ����й�������ͬ��������ֵ�����");
			if(clas==6) printf("(ʷʫ)��ʹ˫��Ӣ�۱�Ϊ30Ѫ");
			if(clas==7) printf("(ʷʫ)����Ķ��ֳ�2���ƣ�Ϊ���Ӣ�ۻָ�8������ֵ");
			if(clas==8) printf("(ʷʫ)��ʹ���������ӻ��+1+1������1���ƺ�������");
			if(clas==9) printf("(ʷʫ)���������������������Ƶĸ���ϴ������ƿ�");
			printf("\n");
		}
	}
}
void shuffle(int l,int r,int k)//ϴ��
{
	int totm=r-l+1;
	for(int i=1;i<=1000;i++)
	{
		int x=rand()%totm+l;
		int y=rand()%totm+l;
		node t;
		t=deck[k][x];
		deck[k][x]=deck[k][y];
		deck[k][y]=t;
	}
}
void begin()//��ʼ��Ϸ��ѡ����ʼ����
{
	//for(int i=1;i<=tot1;i++) flag[1][i]=flag[2][i]=1;
	clear();
	printf("\t���ְҵ�� %s",occu[clas]);
	printf("\n\tѡ����ʼ����\n");
	printf("\t����Ҫ�������Ƶĺ���\n");
	print1(1);
	printf("\n");
	print1(2);
	printf("\n");
	print1(3);
	printf("\n");
	printf("        ȷ�ϣ�");
	int w1=0,w2=0,w3=0,w4=0,ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			switch(xx)
			{
				case 3:gotoxy(3,0);
					   if(w1==0) printf("X"),w1=1;
					   else		 printf(" "),w1=0;
					   break;
				case 5:gotoxy(5,0);
					   if(w2==0) printf("X"),w2=1;
					   else		 printf(" "),w2=0;
					   break;
				case 7:gotoxy(7,0);
					   if(w3==0) printf("X"),w3=1;
					   else		 printf(" "),w3=0;
					   break;
				case 9:gotoxy(9,8);
					   printf("         ");
					   ow=0;
					   break;
			}
			Sleep(150);
		}
	}
	if(w1==1)
	{
		node t;
		t=deck[1][1];
		deck[1][1]=deck[1][4];
		deck[1][4]=t;
	}
	if(w2==1)
	{
		node t;
		t=deck[1][2];
		deck[1][2]=deck[1][5];
		deck[1][5]=t;
	}
	if(w3==1)
	{
		node t;
		t=deck[1][3];
		deck[1][3]=deck[1][6];
		deck[1][6]=t;
	}
	shuffle(4,tot1,1);
	gotoxy(10,0);
	printf("    ���ڣ����������ʼ����\n");
	print1(1);
	printf("\n");
	print1(2);
	printf("\n");
	print1(3);
	printf("\n");
	num1=0;
	hand[1][++num1]=deck[1][1];
	hand[1][++num1]=deck[1][2];
	hand[1][++num1]=deck[1][3];
	//flag[1][1]=0;
	//flag[1][2]=0;
	//flag[1][3]=0;
	tot1-=3;top1=4;
	if(chapter==5) num1=0;
	//your opponent
	w1=w2=w3=w4=0;
	if(deck[2][1].cost>4 || deck[2][1].spell && deck[2][1].quality==2) w1=1;
	if(deck[2][2].cost>4 || deck[2][2].spell && deck[2][2].quality==2) w2=1;
	if(deck[2][3].cost>4 || deck[2][3].spell && deck[2][3].quality==2) w3=1;
	if(deck[2][4].cost>4 || deck[2][4].spell && deck[2][4].quality==2) w4=1;
	if(w1==1)
	{
		node t;
		t=deck[2][1];
		deck[2][1]=deck[2][5];
		deck[2][5]=t;
	}
	if(w2==1)
	{
		node t;
		t=deck[2][2];
		deck[2][2]=deck[2][6];
		deck[2][6]=t;
	}
	if(w3==1)
	{
		node t;
		t=deck[2][3];
		deck[2][3]=deck[2][7];
		deck[2][7]=t;
	}
	if(w4==1)
	{
		node t;
		t=deck[2][4];
		deck[2][4]=deck[2][8];
		deck[2][8]=t;
	}
	shuffle(5,tot2,2);
	num2=0;
	hand[2][++num2]=deck[2][1];
	if(hand[2][num2].cost) hand[2][num2].cost--;
	hand[2][++num2]=deck[2][2];
	if(hand[2][num2].cost) hand[2][num2].cost--;
	hand[2][++num2]=deck[2][3];
	if(hand[2][num2].cost) hand[2][num2].cost--;
	hand[2][++num2]=deck[2][4];
	if(hand[2][num2].cost) hand[2][num2].cost--;
	//flag[2][1]=0;
	//flag[2][2]=0;
	//flag[2][3]=0;
	//flag[2][4]=0;
	tot2-=4;top2=5;
	pause();
}
void print2(int x,int y,int i)//����������
{
	gotoxy(x,y-1);
	printf("%2d ==",hand[1][i].cost);
	gotoxy(++x,y);
	printf("[");
	if(hand[1][i].special==1) printf("��]");
	else
	{
		if(hand[1][i].quality==1) printf("��]");
		if(hand[1][i].quality==2) printf("��]");
		if(hand[1][i].quality==3) printf("��]");
		if(hand[1][i].quality==4) printf("��]");
	}
	gotoxy(++x,y);
	if(!hand[1][i].spell)
	{
		if(hand[1][i].quality==2) printf("B:%d",hand[1][i].treat);
		if(hand[1][i].quality==3) printf("D:%d",hand[1][i].death);
		if(hand[1][i].quality==4) printf("A:%d",hand[1][i].time);
	}
	else if(clas==8 && hand[1][i].quality==3) printf("L:%d",hand[1][i].level+1);
	gotoxy(++x,y-1);
	if(!hand[1][i].spell) printf("%2d %2d",hand[1][i].attack,hand[1][i].health);
	else printf(" ���� ");
	gotoxy(++x,y);
	printf("====");
}
void p1()//�����������
{
	gotoxy(30,0);
	for(int i=1;i<=99;i++) printf(" ");
	gotoxy(31,0);
	for(int i=1;i<=99;i++)printf(" ");
	gotoxy(32,0);
	for(int i=1;i<=99;i++)printf(" ");
	gotoxy(33,0);
	for(int i=1;i<=99;i++)printf(" ");
	gotoxy(34,0);
	for(int i=1;i<=99;i++)printf(" ");
	int position=5;
	for(int i=1;i<=num1;i++) print2(30,position,i),position+=9;
}
void p2()//���¶�������
{
	int position=10;
	for(int i=1;i<=num2;i++)
	{
		gotoxy(0,position);
		printf("[====]");
		gotoxy(1,position);
		printf("[����]");
		gotoxy(2,position);
		printf("[====]");
		position+=8;
	}
}
void p3(int k)//����ս��
{
	int px[3]={0,19,11};
	gotoxy(px[k],0);
	for(int i=1;i<=90;i++) printf(" ");
	gotoxy(px[k]+1,0);
	for(int i=1;i<=90;i++) printf(" ");
	gotoxy(px[k]+2,0);
	for(int i=1;i<=90;i++) printf(" ");
	float mid=alive[k]/2.0;
	for(int i=1;i<=alive[k];i++)
	{
		float dis=42+(i-mid)*10;
		gotoxy(px[k],dis);
		printf("/ == \\");
		gotoxy(px[k]+1,dis);
		printf("||  ||");
		gotoxy(px[k]+2,dis-1);
		if(war[k][i].quality<3) printf("%2d   %2d",war[k][i].attack,war[k][i].health);
		if(war[k][i].quality==3) printf("%2d D%d %d",war[k][i].attack,war[k][i].death,war[k][i].health);
		if(war[k][i].quality==4) printf("%2d A%d %d",war[k][i].attack,war[k][i].time,war[k][i].health);
		if(war[k][i].special==1) gotoxy(px[k]+1,dis+2),printf("��");
		if(war[k][i].special==2) gotoxy(px[k]+1,dis+2),printf("��");
		if(war[k][i].special==4) gotoxy(px[k]+1,dis+2),printf("��");
		if(war[k][i].special==6) gotoxy(px[k]+1,dis+2),printf("��");
		if(war[k][i].special==14)gotoxy(px[k]+1,dis+2),printf("��");
	}
}
void p4()//����Ӣ��Ѫ��
{
	gotoxy(7,48);
	printf("%3d",hea[2]);
	gotoxy(27,48);
	printf("%3d",hea[1]);
}
void p5(int i)//��ʷ��¼
{
	int x=0,y=107;
	gotoxy(x,y-1);
	printf("         ");
	gotoxy(x+1,y-1);
	printf("         ");
	gotoxy(x+2,y-1);
	printf("         ");
	gotoxy(x+3,y-1);
	printf("         ");
	gotoxy(x+4,y-1);
	printf("         ");
	gotoxy(x+5,y-1);
	printf("         ");
	gotoxy(x,y-1);
	printf("��ʷ��¼��");
	x++;
	gotoxy(x,y-1);
	printf("%2d ==",hand[2][i].cost);
	gotoxy(++x,y);
	printf("[");
	if(hand[2][i].quality==1) printf("��]");
	if(hand[2][i].quality==2) printf("��]");
	if(hand[2][i].quality==3) printf("��]");
	if(hand[2][i].quality==4) printf("��]");
	gotoxy(++x,y);
	if(!hand[2][i].spell)
	{
		if(hand[2][i].quality==2) printf("B:%d",hand[2][i].treat);
		if(hand[2][i].quality==3) printf("D:%d",hand[2][i].death);
		if(hand[2][i].quality==4) printf("A:%d",hand[2][i].time);
	}
	else
	{
		if(hand[2][i].quality==1) printf(" �� ");
		if(hand[2][i].quality==2) printf(" �� ");
		if(hand[2][i].quality==3) printf(" �� ");
	}
	gotoxy(++x,y-1);
	if(!hand[2][i].spell) printf("%2d %2d",hand[2][i].attack,hand[2][i].health);
	else printf(" ���� ");
	gotoxy(++x,y);
	printf("====");
}
void p5_1(int t)//��ʷ��¼��Ӣ�ۼ���
{
	int x=0,y=107;
	gotoxy(x,y-1);
	printf("         ");
	gotoxy(x+1,y-1);
	printf("         ");
	gotoxy(x+2,y-1);
	printf("         ");
	gotoxy(x+3,y-1);
	printf("         ");
	gotoxy(x+4,y-1);
	printf("         ");
	gotoxy(x+5,y-1);
	printf("         ");
	gotoxy(x,y-1);
	printf("��ʷ��¼��");
	x++;
	gotoxy(x,y);
	printf("2 ==");
	gotoxy(++x,y);
	printf("[HP]");
	gotoxy(++x,y-1);
	printf("%2d %2d",t,t);
	gotoxy(++x,y);
	printf("====");
}
void p6(int t,int k)//����
{
	int x=7,y=107;
	gotoxy(x,y-1);
	printf("         ");
	gotoxy(x+1,y-1);
	printf("         ");
	gotoxy(x+2,y-1);
	printf("         ");
	gotoxy(x+3,y-1);
	printf("         ");
	gotoxy(x+4,y-1);
	printf("         ");
	gotoxy(x+5,y-1);
	printf("         ");
	if(t==0) return;
	gotoxy(x,y-1);
	printf(" ���ƣ�");
	x++;
	gotoxy(x,y-1);
	printf("%2d ==",deck[k][t].cost);
	gotoxy(++x,y);
	printf("[");
	if(deck[k][t].quality==1) printf("��]");
	if(deck[k][t].quality==2) printf("��]");
	if(deck[k][t].quality==3) printf("��]");
	if(deck[k][t].quality==4) printf("��]");
	gotoxy(++x,y);
	if(!deck[k][t].spell)
	{
		if(deck[k][t].quality==2) printf("B:%d",deck[k][t].treat);
		if(deck[k][t].quality==3) printf("D:%d",deck[k][t].death);
		if(deck[k][t].quality==4) printf("A:%d",deck[k][t].time);
	}
	else if(k==2)
	{
		if(deck[k][t].quality==1) printf(" �� ");
		if(deck[k][t].quality==2) printf(" �� ");
		if(deck[k][t].quality==3) printf(" �� ");
	}
	gotoxy(++x,y-1);
	if(!deck[k][t].spell) printf("%2d %2d",deck[k][t].attack,deck[k][t].health);
	else printf(" ���� ");
	gotoxy(++x,y);
	printf("====");
}
void p7()//���·���ˮ��
{
	gotoxy(26,83);
	printf("%2d/%2d",mana1,tma1);
	gotoxy(4,11);
	printf("%2d/%2d",mana2,tma2);
}
void p8()//�����������
{
	gotoxy(3,45);
	printf("%d/6",dotask2);
	gotoxy(23,44);
	if(clas==1) printf("%2d/1 ",dotask1);
	if(clas==2) printf("%2d/1 ",dotask1);
	if(clas==3) printf("%2d/1 ",en_sum);
	if(clas==4) printf("%2d/6 ",dotask1);
	if(clas==5) printf("%2d/1 ",dotask1);
	if(clas==6) printf("%2d/20",dotask1);
	if(clas==7) printf("%2d/6 ",dotask1);
	if(clas==8) printf("%2d/3 ",dotask1);
	if(clas==9) printf("%2d/3 ",dotask1);
}
void chou1()//�ҷ�����
{
	p6(0,1);
	if(tot1>0)
	{
		//flag[1][top1]=0;
		tot1--;
		if(num1<10) hand[1][++num1]=deck[1][top1],deck[1][top1]=nu;
		else p6(top1,1);
		top1++;
	}
	else fa1++,hea[1]-=fa1;
	p4();
	gotoxy(26,93);
	if(tot1>0) printf("%2d",tot1);
	else printf("%2d",-fa1);
	p1();
}
void chou2()//�Է�����
{
	p6(0,2);
	if(tot2>0)
	{
		//flag[2][top2]=0;
		tot2--;
		if(num2<10) hand[2][++num2]=deck[2][top2];
		else p6(top2,2);
		top2++;
	}
	else fa2++,hea[2]-=fa2;
	p4();
	gotoxy(6,93);
	if(tot2>0) printf("%2d",tot2);
	else printf("%2d",-fa2);
}
void start()//˫��Ӣ���볡
{
	clear();
	gotoxy(4,40);
	printf("  /-------\\  ");
	gotoxy(5,40);
	printf("//         \\\\");
	int t=strlen(name);
	t=46-t/2;
	gotoxy(6,t);
	puts(name);
	gotoxy(6,40);
	printf("||");
	gotoxy(6,51);
	printf("||");
	gotoxy(7,40);
	printf("||       -1||");
	gotoxy(8,40);
	printf("=============");

	gotoxy(24,40);
	printf("  /-------\\  ");
	gotoxy(25,40);
	printf("//         \\\\");
	gotoxy(26,40);
	printf("||  %s  ||",occu[clas]);
	gotoxy(27,40);
	printf("||       -1||");
	gotoxy(28,40);
	printf("=============");

	gotoxy(4,58);
	printf("2 //----\\\\  ");
	gotoxy(5,58);
	printf(" //      \\\\ ");
	gotoxy(6,58);
	printf("||Ӣ�ۼ���||");
	gotoxy(7,58);
	printf(" \\\\      // ");
	gotoxy(8,58);
	printf("  \\\\----//  ");

	gotoxy(24,57);
	printf("%2d //----\\\\  ",hpcost[clas]);
	gotoxy(25,58);
	printf(" //      \\\\ ");
	gotoxy(26,58);
	printf("||Ӣ�ۼ���||");
	gotoxy(27,58);
	printf(" \\\\      // ");
	gotoxy(28,58);
	printf("  \\\\----//  ");

	gotoxy(5,90);
	printf("[ �ƿ� ]");
	gotoxy(6,90);
	printf("[  %2d  ]",tot2);
	gotoxy(7,90);
	printf("[ ʣ�� ]");

	gotoxy(25,90);
	printf("[ �ƿ� ]");
	gotoxy(26,90);
	printf("[  %2d  ]",tot1);
	gotoxy(27,90);
	printf("[ ʣ�� ]");

	gotoxy(26,83);
	printf("%2d/%2d",mana1,tma1);
	gotoxy(4,11);
	printf("%2d/%2d",mana2,tma2);

	gotoxy(33,107);
	printf("Ͷ ��");

	gotoxy(36,6);
	if(clas==1) printf("2��ͨ��ʹ������С��鼮�����+2+1");
	if(clas==2) printf("2��ͨ���ٻ�һ��2-3�ġ�ֱ�ߡ�");
	if(clas==3) printf("0common: I only know this is a common cell card.");
	if(clas==4) printf("0��ͨ�����ڱ��غϣ����һ�㷨��ˮ�����Ҳ��ܳ���10��");
	if(clas==5) printf("2��ͨ����һ���ƣ�������غ�ʹ�ù�Ӣ�ۼ��ܣ����Ϊ��������");
	if(clas==6) printf("1��ͨ���ٻ�һ����ϸ������һ����");
	if(clas==7) printf("2��ͨ����2���ƣ���һ�Ŵ�˵����ϴ������ֵ��ƿ�");
	if(clas==8) printf("1��ͨ����һ���ƣ�������Ʊ����ã����Զ��ͷ�");
	if(clas==9) printf("1��ͨ��������ƿ��ｫ1�ŷ����Ƶĸ��������������");
	gotoxy(37,6);
	if(clas==1) printf("2ϡ�У���ÿ����һ�����鼮������һ����");
	if(clas==2) printf("0ϡ�У�����������з���ˮ����Ȼ��ÿ����2�㷨��ˮ�����һ����");
	if(clas==3) printf("1rare: I only know this is a rare cell card.");
	if(clas==4) printf("2ϡ�У���ÿ����һ�����������һ����");
	if(clas==5) printf("2ϡ�У�ʹ������ӵ�����ֵ��Ϊ1");
	if(clas==6) printf("3ϡ�У�ʹ�з����Ҳ���ӱ�Ϊ0-0");
	if(clas==7) printf("0ϡ�У�ʹ����������ƻ��-1/+1+1������ƿ�ÿ��һ���ƣ����Ƶķ���ֵ���ı�����1��");
	if(clas==8) printf("2ϡ�У������ֵ�һ������ϴ������ƿ�");
	if(clas==9) printf("6ϡ�У��ٻ����ƿ���һ��6�������ϵ���ӵĸ���");
	gotoxy(38,6);
	if(clas==1) printf("2ʷʫ��ʹ��������鼮�����������");
	if(clas==2) printf("6ʷʫ������ķ���ˮ����Ϊ������2-2�ġ�ֱ�ߡ�");
	if(clas==3) printf("3epic: I only know this is an epic cell card.");
	if(clas==4) printf("2ʷʫ���ݻ�������з���ˮ�����ٻ�һ������ֵ��С��6�Ĵ�˵��ӡ��⡱���ڸ����������ԭ��ķ���ˮ��");
	if(clas==5) printf("3ʷʫ���Ƴ����й�������ͬ��������ֵ�����");
	if(clas==6) printf("5ʷʫ��ʹ˫��Ӣ�۱�Ϊ30Ѫ");
	if(clas==7) printf("3ʷʫ����Ķ��ֳ�2���ƣ�Ϊ���Ӣ�ۻָ�8������ֵ");
	if(clas==8) printf("2ʷʫ��ʹ���������ӻ��+1+1������1���ƺ�������");
	if(clas==9) printf("2ʷʫ���������������������Ƶĸ���ϴ������ƿ�");

	p2();
	p1();
	p4();
}
void end(int k)//�غϽ���ʱ�Զ�����
{
	int k1=k==1?2:1;
	float mid[3];
	mid[1]=alive[1]/2.0+0.5;
	mid[2]=alive[2]/2.0+0.5;
	if(mid[1]==0.5) mid[1]=0;
	if(mid[2]==0.5) mid[2]=0;
	for(int i=1;i<=alive[k];i++)
	{
		float dis=i-mid[k];
		int hit=1;
		if(mid[k1]+dis<=0 || mid[k1]+dis>=alive[k1]+1 || alive[k1]==0) hit=0;
		if(hit==0)
		{
			if(k==2 || chapter!=6) hea[k1]-=war[k][i].attack;
			else hea[k1]-=war[k][i].attack>2?2:war[k][i].attack;
		}
		else for(float j=-0.5;j<=0.5;j+=0.5)
		{
			float to=mid[k1]+dis+j;
			if(to-(int)to!=0) continue;
			war[k1][(int)to].health-=war[k][i].attack*war[k][i].time;
			if(mode==4 && clas==6 && dotask1<20 && war[k1][(int)to].special==6) dotask1+=war[k][i].attack*war[k][i].time,p8();
		}
		p4();
		p3(k1);
		Sleep(400);
	}
	int al=alive[k1];
	for(int i=1;i<=al;i++)
		if(war[k1][i].health<=0)
		{
			if(k1==1 && war[k1][i].special==14) tma1+=war[k1][i].beam,tma1=tma1>10?10:tma1;
			alive[k1]--;
			hea[k]-=war[k1][i].death;
			war[k1][i]=nu;
		}
	int numm=0;
	for(int i=1;i<=al;i++)
		if(war[k1][i].health) war[k1][++numm]=war[k1][i];
	p3(k1);
	Sleep(300);
	p4();
	Sleep(300);
	if(hea[1]<=0 && hea[2]<=0) over=2;
	else if(hea[1]<=0) over=3;
	else if(hea[2]<=0) over=1;
}
void your()//��Ļغ�
{
	p6(0,1);
	if(mode==4) p8();
	if(tma1<10) tma1++;
	mana1=tma1;
	if(mode==4 && clas==2 && dotask1==0)
	{
		if(tma1==10) dotask1=1;
		if(mana1==5) dotask1=-1;
		p8();
	}
	hpuse1=0;
	gotoxy(16,45);
	printf("��Ļغϣ�");
	Sleep(1000);
	gotoxy(16,45);
	printf("          ");
	gotoxy(16,94);
	printf("����");
	gotoxy(29,107);
	printf("�� ͣ");
	chou1();
	if(hea[1]<=0){over=3;return;}
	if(mode==4 && clas==7 && dotask1<6 && fa1)
	{
		dotask1+=fa1;
		if(dotask1>=6)
		{
			tot1=6;
			create(1);
			top1=1;
			//for(int i=1;i<=6;i++) flag[1][i]=1;
			chou1();
		}
		p8();
	}
	if(mode==4 && clas==8 && dotask1==3 && alive[1]<7)
	{
		war[1][++alive[1]]=nu;
		war[1][alive[1]].attack=3;
		war[1][alive[1]].health=3;
		war[1][alive[1]].time=1;
		war[1][alive[1]].quality=1;
		p3(1);
	}
	fc1=1;
	gotoxy(26,83);
	printf("%2d/%2d",mana1,tma1);
	int ow=1,suspend=0;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(suspend && xx!=29) continue;
			if(xx==29 && yy>106 && yy<113)
			{
				suspend^=1;
				gotoxy(29,107);
				if(suspend) printf("�� ��");
				else printf("�� ͣ");
			}
			if(xx==33 && yy>106 && yy<113)
			{
				over=3;
				return;
			}
			if(xx>28 && yy>0 && yy<105)
			{
				int t=(yy-2)/9+1;
				if(t>num1 || mana1<hand[1][t].cost || alive[1]==7)
				{
					Sleep(400);
					continue;
				}
				if(hand[1][t].spell && clas==7 && hand[1][t].quality==2 && mana1<tot1)
				{
					Sleep(400);
					continue;
				}
				mana1-=hand[1][t].cost;
				if(mode==4 && clas==2 && dotask1==0 && mana1==5) dotask1=-1,p8();
				int y1=(t-1)*9+3;
				gotoxy(30,y1);
				printf("         ");
				gotoxy(31,y1);
				printf("         ");
				gotoxy(32,y1);
				printf("         ");
				gotoxy(33,y1);
				printf("         ");
				gotoxy(34,y1);
				printf("         ");
				Sleep(500);
				node hand1=hand[1][t];
				hand[1][t]=nu;
				for(int i=t+1;i<=num1;i++) hand[1][i-1]=hand[1][i];
				num1--;
				if(!hand1.spell)
				{
					hea[1]+=hand1.treat;
					if(hea[1]>maxhealth) hea[1]=maxhealth;
					if(hand1.special==1 && mode==4 && dotask1<1)
					{
						int num=0;
						for(int i=1;i<=alive[1];i++) if(war[1][i].special) num++;
						if(num>=2) dotask1=1;
						p8();
					}
					p4();
					war[1][++alive[1]]=hand1;
				}
				else
				{
					if(clas==1)
					{
						if(hand1.quality==1)
						{
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].special==1) war[1][i].attack+=2,war[1][i].health+=2;
						}
						if(hand1.quality==2)
						{
							int num=0;
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].special==1) num++;
							for(int i=1;i<=num;i++) chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==3)
						{
							int flag1[10]={0};
							for(int i=1;i<=alive[1];i++) flag1[i]=1;
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].special==1)
									if(num1<7)
									{
										hand[1][++num1]=war[1][i];
										war[1][i]=nu;flag1[i]=0;
									}
							int num=0;
							for(int i=1;i<=alive[1];i++)
								if(flag1[i]) war[1][++num]=war[1][i];
							alive[1]=num;
						}
					}
					if(clas==2)
					{
						if(hand1.quality==1 && alive[1]<7)
						{
							war[1][++alive[1]]=nu;
							war[1][alive[1]].health=3;
							war[1][alive[1]].attack=2;
							war[1][alive[1]].time=1;
							war[1][alive[1]].cost=2;
							war[1][alive[1]].special=2;
						}
						if(hand1.quality==2)
						{
							int num=mana1/2;
							mana1=0;
							p7();
							for(int i=1;i<=num;i++) chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==3)
						{
							int num=7-alive[1];
							if(num>tma1) num=tma1;
							for(int i=1;i<=num;i++)
							{
								tma1--;
								war[1][++alive[1]]=nu;
								war[1][alive[1]].health=2;
								war[1][alive[1]].attack=2;
								war[1][alive[1]].time=1;
								war[1][alive[1]].cost=2;
								war[1][alive[1]].special=2;
							}
						}
					}
					if(clas==3)
					{
						if(hand1.quality==1)
						{
							en_sum++;
						}
						if(hand1.quality==2)
						{
							en_sum++;chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==3)
						{
							en_sum+=3;
						}
					}
					if(clas==4)
					{
						if(hand1.quality==1)
						{
							mana1++;
							if(mana1>10) mana1=10;
						}
						if(hand1.quality==2)
						{
							int num=0;
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].special==4) num++;
							for(int i=1;i<=num;i++) chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==3 && alive[1]<7)
						{
							int num=tma1;
							mana1=0;tma1=0;
							war[1][++alive[1]]=nu;
							war[1][alive[1]].beam=num;
							war[1][alive[1]].cost=rand()%10+1;
							war[1][alive[1]].attack=rand()%10+1;
							war[1][alive[1]].health=rand()%10+1;
							war[1][alive[1]].special=14;
							war[1][alive[1]].time=rand()%3+2;
							while(war[1][alive[1]].attack<war[1][alive[1]].cost-4) war[1][alive[1]].attack=rand()%10+1;
							while(war[1][alive[1]].health<war[1][alive[1]].cost-4) war[1][alive[1]].health=rand()%10+1;
							if(war[1][alive[1]].health<war[1][alive[1]].cost+1) war[1][alive[1]].health=war[1][alive[1]].cost+1;
							if(war[1][alive[1]].health>10) war[1][alive[1]].health=10;
							if(war[1][alive[1]].health<6) war[1][alive[1]].health=6;
						}
					}
					if(clas==5)
					{
						if(hand1.quality==1)
						{
							chou1();
							if(hpuse1) chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==2)
						{
							for(int i=1;i<=alive[1];i++) war[1][i].health=1;
							for(int i=1;i<=alive[2];i++) war[2][i].health=1;
						}
						if(hand1.quality==3)
						{
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].attack==war[1][i].health) war[1][i]=nu;
							for(int i=1;i<=alive[2];i++)
								if(war[2][i].attack==war[2][i].health) war[2][i]=nu;
							int num=0;
							for(int i=1;i<=alive[1];i++)
								if(war[1][i].health) war[1][++num]=war[1][i];
							alive[1]=num;
							num=0;
							for(int i=1;i<=alive[2];i++)
								if(war[2][i].health) war[2][++num]=war[1][i];
							alive[2]=num;
						}
					}
					if(clas==6)
					{
						if(hand1.quality==1)
						{
							if(alive[1]<7)
							{
								war[1][++alive[1]]=nu;
								war[1][alive[1]].time=1;
								war[1][alive[1]].special=6;
							}
							chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==2 && alive[2])
						{
							war[2][alive[2]].health=0;
							war[2][alive[2]].attack=0;
						}
						if(hand1.quality==3)
						{
							hea[1]=hea[2]=30;
						}
					}
					if(clas==7)
					{
						if(hand1.quality==1)
						{
							chou1();chou1();
							if(hea[1]<=0){over=3;return;}
							if(tot2<30)
							{
								tot2++;
								int t=0,k=2,i=--top2;
								deck[k][i].quality=4;
								deck[k][i].attack=rand()%10+1;
								deck[k][i].health=rand()%10+1;
								deck[k][i].cost=rand()%10+1;
								deck[k][i].spell=0;
								while(deck[k][i].attack<deck[k][i].cost-4) deck[k][i].attack=rand()%10+1;
								while(deck[k][i].health<deck[k][i].cost-4) deck[k][i].health=rand()%10+1;
								deck[k][i].time=rand()%3+2;
								if(deck[k][i].health<deck[k][i].cost+1) deck[k][i].health=deck[k][i].cost+1;
								if(deck[k][i].health>10) deck[k][i].health=10;
								shuffle(top2,30,2);
								gotoxy(6,93);
								if(tot2>0) printf("%2d",tot2);
								else printf("%2d",-fa2);
							}
						}
						if(hand1.quality==2)
						{
							mana1-=tot1;
							for(int i=1;i<=num1;i++)
							{
								hand[1][i].cost--;
								if(hand[1][i].cost<0) hand[1][i].cost=0;
								hand[1][i].attack++;
								if(hand[1][i].attack>10) hand[1][i].attack=10;
								hand[1][i].health++;
								if(hand[1][i].health>10) hand[1][i].health=10;
							}
						}
						if(hand1.quality==3)
						{
							chou2();
							chou2();
							hea[1]+=8;
							if(hea[1]>maxhealth) hea[1]=maxhealth;
							if(hea[2]<=0){over=1;return;}
						}
					}
					if(clas==8)
					{
						if(hand1.quality==1)
						{
							chou1();
							if(hea[1]<=0){over=3;return;}
						}
						if(hand1.quality==2 && num2 && tot1<30)
						{
							int t=rand()%num2+1;
							deck[1][--top1]=hand[2][t];
							tot1++;
							for(int i=t+1;i<=num2;i++) hand[2][i-1]=hand[2][i];
							hand[2][num2]=nu;
							num2--;
						}
						if(hand1.quality==3)
						{
							for(int i=1;i<=alive[1];i++)
								war[1][i].attack+=hand1.level+1,
								war[1][i].health+=hand1.level+1;
						}
					}
					if(clas==9)
					{
						if(hand1.quality==1 && tot1>0)
						{
							int num=0,t=rand()%(tot1-top1+1)+top1;
							for(int i=top1;i<=tot1;i++) if(deck[1][i].spell) num=1;
							if(num)
							{
								while(!deck[1][t].spell) t=rand()%(tot1-top1+1)+top1;
								hand[1][++num1]=deck[1][t];
							}
						}
						if(hand1.quality==2 && alive[1]<7)
						{
							int num=0,t=rand()%(tot1-top1+1)+top1;
							for(int i=top1;i<=tot1;i++) if(deck[1][i].attack>=6) num=1;
							if(num)
							{
								while(deck[1][t].attack<6) t=rand()%(tot1-top1+1)+top1;
								war[1][++alive[1]]=deck[1][t];
							}
						}
						if(hand1.quality==3)
						{
							for(int i=1;i<=num1;i++)
							{
								if(tot1>=30) break;
								deck[1][--top1]=hand[1][i];
								tot1++;
							}
							shuffle(top1,30,1);
						}
					}

				}
				p1();p2();
				p3(1);p3(2);p4();
				fc1=0;
				p7();
			}
			if(xx>23 && xx<29 && !hpuse1 && yy>56 && yy<71)
			{
				if(clas==1)//����
				{
					if(mana1<2) continue;
					mana1-=2;
					hpuse1=1;
					if(alive[1]==7) continue;
					war[1][++alive[1]]=nu;
					war[1][alive[1]].health=2;
					war[1][alive[1]].attack=1;
					war[1][alive[1]].time=1;
					war[1][alive[1]].cost=2;
					war[1][alive[1]].special=1;
					if(dotask1==1) war[1][alive[1]].health=6,war[1][alive[1]].attack=5;
					if(mode==4 && dotask1<1)
					{
						int num=0;
						for(int i=1;i<=alive[1];i++) if(war[1][i].special) num++;
						if(num>=2) dotask1=1;
						p8();
					}
					p1();p7();p3(1);p4();
				}
				if(clas==2)//��ѧ
				{
					if(mana1<4) continue;
					mana1-=4;
					if(mode==4 && clas==2 && dotask1==0 && mana1==5) dotask1=-1,p8();
					hpuse1=1;
					if(tma1<10) tma1++;
					else
					{
						p6(0,1);
						if(tot1>0)
						{
							//flag[1][top1]=0;
							tot1--;
							if(num1<10)
							{
								hand[1][++num1]=deck[1][top1];
								deck[1][top1]=nu;
								if(mode==4 && dotask1)
								{
									hand[1][num1].cost-=4;
									if(hand[1][num1].cost<0) hand[1][num1].cost=0;
								}
							}
							else p6(top1,1);
							top1++;
						}
						else fa1++,hea[1]-=fa1;
						p4();
						gotoxy(26,93);
						if(tot1>0) printf("%2d",tot1);
						else printf("%2d",-fa1);
						p1();
					}
					if(hea[1]<=0){over=3;return;}
					p1();p7();p3(1);p4();
				}
				if(clas==3)//english
				{
					en_sum++;
					if(en_sum>=35 || mode==4 && en_sum>=30)
					{
						hea[2]=0;en_sum=0;
						p4();
						over=1;return;
					}
					if(en_sum==1) MessageBox(NULL,"No skill. Please don't try.","Warning!",MB_OK);
					if(en_sum==2) MessageBox(NULL,"It is actually no skill.","Warning!",MB_OK);
					if(en_sum==3) MessageBox(NULL,"Why don't you believe me?","Warning!",MB_OK);
					if(en_sum==4) MessageBox(NULL,"Give up!There is nothing uesful!","Warning!",MB_OK);
					if(en_sum==5) MessageBox(NULL,"If you don't stop,you will regret doing this.","Warning!",MB_OK);
					if(en_sum==6) MessageBox(NULL,"Back to game!Time is up!","Warning!",MB_OK);
					if(en_sum==7) MessageBox(NULL,"I am angry,and you will be punished soon.","Warning!",MB_OK);
					if(en_sum==8) MessageBox(NULL,"You're a reckless person,and recklessness is worng.","Warning!",MB_OK);
					if(en_sum==9) MessageBox(NULL,"I have warned.This is your fault.","Warning!",MB_OK);
					if(en_sum==10) MessageBox(NULL,"Don't grudge me.It's none of my business.","Warning!",MB_OK);
					if(en_sum>10)
					{
						gotoxy(16,95);
						printf("   ");
						ow=0;
					}
				}
				if(clas==4)//����
				{
					if(mana1<1) continue;
					mana1-=1;
					hpuse1=1;
					tma1--;
					if(alive[1]==7) continue;
					war[1][++alive[1]]=nu;
					war[1][alive[1]].time=1;
					war[1][alive[1]].attack=3;
					war[1][alive[1]].health=3;
					if(mode==4 && dotask1<6)
					{
						dotask1++;
						if(dotask1==6)
						{
							mana1=10;
							tma1=10;
							p7();
						}
						p8();
					}
					p1();p7();p3(1);p4();
				}
				if(clas==5)//��ѧ
				{
					if(mana1<2) continue;
					mana1-=2;
					hpuse1=1;
					for(int i=1;i<=alive[1];i++)
						swap(war[1][i].attack,war[1][i].health);
					for(int i=1;i<=alive[2];i++)
						swap(war[2][i].attack,war[2][i].health);
					if(mode==4 && alive[2]>=2 && dotask1<1)
					{
						int ok=1;
						for(int i=1;i<=alive[2];i++) if(war[2][i].attack!=1) ok=0;
						if(ok==1)
						{
							dotask1=1;
							for(int i=1;i<=num2;i++) hand[2][i].attack=1;
							for(int i=1;i<=tot2;i++) deck[2][i+top2-1].attack=1;
							p8();
						}
					}
					p1();p7();p3(1);p4();p3(2);
				}
				if(clas==6)//����
				{
					if(mana1<1) continue;
					mana1-=1;
					hpuse1=1;
					if(alive[1]==7) continue;
					war[1][++alive[1]]=nu;
					war[1][alive[1]].time=1;
					war[1][alive[1]].special=6;
					if(mode==4 && dotask1>=20)
					{
						for(int i=1;i<=2;i++)
						{
							if(alive[1]==7) break;
							war[1][++alive[1]]=nu;
							war[1][alive[1]].time=1;
							war[1][alive[1]].special=6;
						}
						dotask1=20;
						p8();
					}
					p7();p3(1);p4();
				}
				if(clas==7)//����
				{
					if(mana1<2) continue;
					mana1-=2;
					hpuse1=1;
					tot1=0;top1=30;
					tot2=0;top2=30;
					gotoxy(26,93);
					if(tot1>0) printf("%2d",tot1);
					else printf("%2d",-fa1);
					gotoxy(6,93);
					if(tot2>0) printf("%2d",tot2);
					else printf("%2d",-fa2);
					p1();p2();p4();
					p7();p3(1);p3(2);
				}
				if(clas==8)//��ʷ
				{
					if(mana1<2) return;
					mana1-=2;
					hpuse1=1;
					if(num1)
					{
						int t=rand()%num1+1;
						if(mode==4 && dotask1<3 && hand[1][t].quality>1) dotask1++,p8();
						hand[1][t]=nu;
						for(int i=t+1;i<=num1;i++) hand[1][i-1]=hand[1][i];
						hand[1][num1]=nu;
						num1--;
						for(int i=1;i<=num1;i++)
							if(hand[1][i].spell && hand[1][i].quality==3) hand[1][i].level++;
						int y1=(t-1)*9+3;
						gotoxy(30,y1);
						printf("         ");
						gotoxy(31,y1);
						printf("         ");
						gotoxy(32,y1);
						printf("         ");
						gotoxy(33,y1);
						printf("         ");
						gotoxy(34,y1);
						printf("         ");
						Sleep(250);
						p1();
						Sleep(250);
					}
					chou1();
					if(hea[1]<=0){over=3;return;}
					p1();p2();p4();
					p7();p3(1);
				}
				if(clas==9)//����
				{
					if(mana1<5) continue;
					mana1-=5;
					hpuse1=1;
					if(alive[1]==7 || tot1==0) continue;
					//flag[1][top1]=0;
					tot1--;
					war[1][++alive[1]]=deck[1][top1];
					deck[1][top1]=nu;
					top1++;
					if(mode==4 && dotask1<3 && war[1][alive[1]].attack<=3)
					{
						dotask1++;
						if(dotask1==3)
							for(int i=1;i<=3;i++)
							{
								if(alive[1]==7 || tot1==0) break;
								//flag[1][top1]=0;
								tot1--;
								war[1][++alive[1]]=deck[1][top1];
								deck[1][top1]=nu;
								top1++;
							}
						p8();
					}
					gotoxy(26,93);
					if(tot1>0) printf("%2d",tot1);
					else printf("%2d",-fa1);
					p1();p2();p4();
					p7();p3(1);
				}
			}
			if(xx<17 && xx>10 && yy>70 && yy<105)
			{
				gotoxy(16,95);
				printf("   ");
				ow=0;
			}
			Sleep(400);
		}
	}
	end(1);
}
void opponent()//���ֻغ�
{
	if(mode==4) p8();
	if(tma2<10) tma2++;
	mana2=tma2;
	chou2();
	if(hea[2]<=0){over=1;return;}
	fc2=1;
	gotoxy(4,11);
	printf("%2d/%2d",mana2,tma2);
	p2();
	//AI
	double dpf[20]={0};
	int ow=1,hpuse2=0;
	while(ow)
	{
		double maxx=-1;
		int fff=0;
		if(alive[2]==7) break;
		for(int i=1;i<=num2;i++)
		{
			if(!hand[2][i].spell)
				dpf[i]=(1.0*hand[2][i].attack*(hand[2][i].attack-1)+hand[2][i].health)/(hand[2][i].cost);
			else
			{
				if(hand[2][i].quality==1)
				{
					dpf[i]=4.1;
					if(num2==10 || alive[2]==7) dpf[i]=0;
					if(tot2<=0) dpf[i]=1;
				}
				if(hand[2][i].quality==2 && alive[1]) dpf[i]=war[1][1].health;
				if(hand[2][i].quality==3)
				{
					if(mode==4 && dotask2==6) dpf[i]=7;
					else dpf[i]=3;
				}
			}
			if(maxx<dpf[i] && mana2>=hand[2][i].cost) maxx=dpf[i],fff=i;
		}
		if((fff==0 && hpuse2<maxhp2 && mana2>=2 && alive[2]<7)
		||((maxx<=4 || mode==4 && dotask2==6 && maxx<=13) && hpuse2<maxhp2 && mana2>=2 && alive[2]<7))
		{
			mana2-=2;
			hpuse2++;
			war[2][++alive[2]]=nu;
			int t=rand()%4+2;
			war[2][alive[2]].attack=war[2][alive[2]].health=t;
			war[2][alive[2]].time=1;
			if(chapter==9) war[2][alive[2]].quality=3,war[2][alive[2]].death=t;
			else war[2][alive[2]].quality=1;
			if(mode==4 && dotask2==6) war[2][alive[2]].attack=war[2][alive[2]].health=5,t=5;
			if(mode==4 && dotask2<6) dotask2++,p8();
			p5_1(t);p3(2);p7();Sleep(2500);
			continue;
		}
		if(fff==0) break;
		if(hand[2][fff].spell)
		{
			mana2-=hand[2][fff].cost;
			if(hand[2][fff].quality==1)
			{
				war[2][++alive[2]]=nu;
				int t=rand()%4+2;
				war[2][alive[2]].attack=war[2][alive[2]].health=t;
				war[2][alive[2]].time=1;
				war[2][alive[2]].quality=1;
				chou2();
				if(hea[2]<=0){over=1;return;}
				p3(2);p7();
			}
			if(hand[2][fff].quality==2)
			{
				war[1][1]=nu;
				for(int i=2;i<=alive[1];i++) war[1][i-1]=war[1][i];
				war[1][alive[1]]=nu;
				alive[1]--;
				p3(1);
			}
			if(hand[2][fff].quality==3)
			{
				maxhp2++;
				gotoxy(3,63);
				printf("+%d",maxhp2-1);
			}
		}
		else
		{
			war[2][++alive[2]]=hand[2][fff];
			mana2-=hand[2][fff].cost;
			hea[2]+=hand[2][fff].treat;
			if(hea[2]>maxhealth) hea[2]=maxhealth;
		}
		int y1=fff*8+2;
		gotoxy(0,y1);
		printf("        ");
		gotoxy(1,y1);
		printf("        ");
		gotoxy(2,y1);
		printf("        ");
	//	gotoxy(3,y1);
	//	printf("        ");
		p5(fff);
		p3(2);
		gotoxy(4,11);
		printf("%2d/%2d",mana2,tma2);
		Sleep(1500);
		y1=num2*8+2;
		gotoxy(0,y1);
		printf("         ");
		gotoxy(1,y1);
		printf("         ");
		gotoxy(2,y1);
		printf("         ");
	//	gotoxy(3,y1);
	//	printf("         ");
		hand[2][fff]=nu;
		for(int i=fff+1;i<=num2;i++) hand[2][i-1]=hand[2][i];
		hand[2][num2]=nu;
		num2--;
		p2();
		Sleep(1500);
		fc2=0;
	}
	end(2);
}
void game()//������Ϸ
{
	while(!over)
	{
		your();
		if(over) break;
		opponent();
	}
	gotoxy(15,45);
	if(over==1) printf("Victory!");
	if(mode!=3 && over==1) win[clas]++,twin++;
	if(over==3) printf(" Fail!");
	if(mode!=3 && over==3) lose[clas]++,tlose++;
	if(over==2) printf("Evened!");
	if(mode==3 && over==1) ac[chapter]=1;
	data=fopen("data.txt","w");

	for(int i=1;i<=9;i++) fprintf(data,"%d %d\n",win[i],lose[i]);
	fprintf(data,"%d %d\n",twin,tlose);
	for(int i=1;i<=9;i++) fprintf(data,"%d\n",ac[i]);
	fputs(name,data);
	fclose(data);

	gotoxy(17,42);
	pause();
}
void create1()//��������ģʽ����
{
	node card0;
	int t=rand()%100;
	if(t==0) card0.quality=4;
	else if(t<=5) card0.quality=3;
	else if(t>=80) card0.quality=2;
	else card0.quality=1;
	for(int i=1;i<=3;i++)
	{
		t=rand()%100;if(card0.quality==4) t=99;
		if(t<20) card[i].spell=1;
		else card[i].spell=0;
		if(card[i].spell)
		{
			card[i].quality=card0.quality;
			card[i].cost=spcost[(clas-1)*3+card[i].quality];
		}
		else
		{
			card[i].quality=card0.quality;
			card[i].attack=rand()%10+1;
			card[i].health=rand()%10+1;
			card[i].cost=rand()%10+1;
			card[i].time=1;
			if(card[i].quality==4)
			{
				card[i].time=rand()%3+2;
				if(card[i].health<card[i].cost+1) card[i].health=card[i].cost+1;
				if(card[i].health>10) card[i].health=10;
			}
			if(card[i].quality==3)
			{
				card[i].death=rand()%6+2;
				int t1=card[i].attack,t2=card[i].health;
				card[i].attack=card[i].health=(t1+t2+1)/2;
			}
			if(card[i].quality==2)
			{
				card[i].treat=rand()%5+1;
				if(card[i].attack>card[i].cost*2) card[i].attack=card[i].cost*2+1;
				if(card[i].health>card[i].cost*2) card[i].health=card[i].cost*2+1;
			}
			if(card[i].quality==1)
			{
				while(card[i].attack>card[i].cost*2) card[i].attack=rand()%10+1;
				while(card[i].health>card[i].cost*2) card[i].health=rand()%10+1;
			}
			if(card[i].attack<card[i].cost-4) card[i].attack=card[i].cost-4;
			if(card[i].health<card[i].cost-4) card[i].health=card[i].cost-4;
		}
	}
}
void deck_copy()//��������
{
	for(int i=1;i<=tot1;i++) deck[1][i]=card1[i];
	for(int i=1;i<=tot2;i++) deck[2][i]=card1[i];
	for(int i=1;i<=tot2;i++) if(deck[2][i].spell) deck[2][i].cost=3;
}
void init()//��ʼ����Ϸ
{
	memset(deck,0,sizeof deck);
	memset(hand,0,sizeof hand);
	memset(war,0,sizeof war);
	memset(alive,0,sizeof alive);
	//memset(flag,0,sizeof flag);
	fc1=fc2=num1=num2=tot1=tot2=0;
	tot1=30;
	tot2=30;
	tma1=tma2=fa1=fa2=0;
	mana1=mana2=0;
	if(mode==1 || mode==4) create(1),create(2);
	if(mode==2) deck_copy();
	if(mode==3)
	{
		if(chapter==1) tot1=20,create(1),create(2);
		else if(chapter==2) tot1=tot2=10,create(1),create(2);
		else create(1),create(2);
	}
	over=0;
	dotask1=0;dotask2=0;
	hpuse1=hpuse2=0;maxhp2=1;
	hea[1]=hea[2]=maxhealth=30;
	en_sum=0;
}
void onegame()//����һ����Ϸ
{
	init();
	shuffle(1,tot1,1);
	shuffle(1,tot2,2);
	begin();
	start();
	game();
}
void rand_mode()//���ģʽ
{
	system("cls");chapter=0;
	gotoxy(2,0);
	printf("     ��ӭ�������ģʽ��        ����������");
	gotoxy(4,0);
	printf("        ��ѡ��ְҵ��\n");
	gotoxy(6,10);printf("%s",occu[1]);
	gotoxy(6,30);printf("%s",occu[2]);
	gotoxy(6,50);printf("%s",occu[3]);
	gotoxy(8,10);printf("%s",occu[4]);
	gotoxy(8,30);printf("%s",occu[5]);
	gotoxy(8,50);printf("%s",occu[6]);
	gotoxy(10,10);printf("%s",occu[7]);
	gotoxy(10,30);printf("%s",occu[8]);
	gotoxy(10,50);printf("%s",occu[9]);
	gotoxy(13,6);printf("��%3.0lf%%��2���ģ��ٻ�һ��1-2�ġ��鼮����",1.0*win[1]/(win[1]+lose[1])*100);
	gotoxy(14,6);printf("��%3.0lf%%��4��ѧ�����һ���յķ���ˮ��������10�㷨��ˮ����ĳ�һ���ơ�",1.0*win[2]/(win[2]+lose[2])*100);
	gotoxy(15,6);printf("��%3.0lf%%��0Ӣ�This hero power is unknown.",1.0*win[3]/(win[3]+lose[3])*100);
	gotoxy(16,6);printf("��%3.0lf%%��1�����ݻ����һ������ˮ�����ޣ��ٻ�һ��3-3�ġ������",1.0*win[4]/(win[4]+lose[4])*100);
	gotoxy(17,6);printf("��%3.0lf%%��2��ѧ��ʹ������ӽ�������ֵ�͹�������",1.0*win[5]/(win[5]+lose[5])*100);
	gotoxy(18,6);printf("��%3.0lf%%��1����ٻ�һ��0-0�ġ�ϸ��������ֻ�ܴ��һ���غϡ�",1.0*win[6]/(win[6]+lose[6])*100);
	gotoxy(19,6);printf("��%3.0lf%%��2���Σ��ݻ�˫���ƿ⡣",1.0*win[7]/(win[7]+lose[7])*100);
	gotoxy(20,6);printf("��%3.0lf%%��2��ʷ������һ������Ȼ���һ���ơ�",1.0*win[8]/(win[8]+lose[8])*100);
	gotoxy(21,6);printf("��%3.0lf%%��5����������ƿ��ｫһ���������ս����",1.0*win[9]/(win[9]+lose[9])*100);
	gotoxy(22,6);printf("��%3.0lf%%��2ѧ��������ٻ�һ��������������ֵ��ȵ����(2~5)��",1.0*tlose/(twin+tlose)*100);
	int ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx==2) return;
			if(xx==6 && (yy>= 8 && yy<=13)) clas=1,ow=0;
			if(xx==6 && (yy>=29 && yy<=34)) clas=2,ow=0;
			if(xx==6 && (yy>=50 && yy<=55)) clas=3,ow=0;
			if(xx==8 && (yy>= 8 && yy<=13)) clas=4,ow=0;
			if(xx==8 && (yy>=29 && yy<=34)) clas=5,ow=0;
			if(xx==8 && (yy>=50 && yy<=55)) clas=6,ow=0;
			if(xx==10&& (yy>= 8 && yy<=13)) clas=7,ow=0;
			if(xx==10&& (yy>=29 && yy<=34)) clas=8,ow=0;
			if(xx==10&& (yy>=50 && yy<=55)) clas=9,ow=0;
			Sleep(150);
		}
	}
	gotoxy(0,0);
	onegame();
}
void print_card(int x,int y,int k)//�����������
{
	if(k==0)
	{
		gotoxy(x,y-1); printf("          ");
		gotoxy(++x,y-1); printf("          ");
		gotoxy(++x,y-1); printf("          ");
		gotoxy(++x,y-1); printf("          ");
		gotoxy(++x,y-1); printf("          ");
		return;
	}
	gotoxy(x,y-1);
	printf("%2d ==",card[k].cost);
	gotoxy(++x,y);
	printf("[");
	if(card[k].quality==1) printf("��]");
	if(card[k].quality==2) printf("��]");
	if(card[k].quality==3) printf("��]");
	if(card[k].quality==4) printf("��]");
	gotoxy(++x,y);
	if(!card[k].spell)
	{
		if(card[k].quality==1) printf("    ");
		if(card[k].quality==2) printf("B:%d",card[k].treat);
		if(card[k].quality==3) printf("D:%d",card[k].death);
		if(card[k].quality==4) printf("A:%d",card[k].time);
	}
	else printf("    ");
	gotoxy(++x,y-1);
	if(!card[k].spell) printf("%2d %2d",card[k].attack,card[k].health);
	else printf(" ���� ");
	gotoxy(++x,y);
	printf("====");
}
void xuanpai()//����ģʽѡ��
{
	system("cls");
	gotoxy(2,10);
	printf("��ѡ���ְҵΪ��%s",occu[clas]);
	gotoxy(4,8);
	printf("�������¿�����ѡȡ30������Ϊ������ƣ�");
	gotoxy(5,8);
	printf("�����ѡ����Ķ���Ҳ���ǿ��");
	gotoxy(7,10);
	if(clas==1) printf("2��ͨ��ʹ������С��鼮�����+2+1");
	if(clas==2) printf("2��ͨ���ٻ�һ��2-3�ġ�ֱ�ߡ�");
	if(clas==3) printf("0common: I only know this is a common cell card.");
	if(clas==4) printf("0��ͨ�����ڱ��غϣ����һ�㷨��ˮ�����Ҳ��ܳ���10��");
	if(clas==5) printf("2��ͨ����һ���ƣ�������غ�ʹ�ù�Ӣ�ۼ��ܣ����Ϊ��������");
	if(clas==6) printf("1��ͨ���ٻ�һ����ϸ������һ����");
	if(clas==7) printf("2��ͨ����2���ƣ���һ�Ŵ�˵����ϴ������ֵ��ƿ�");
	if(clas==8) printf("1��ͨ����һ���ƣ�������Ʊ����ã����Զ��ͷ�");
	if(clas==9) printf("1��ͨ��������ƿ��ｫ1�ŷ����Ƶĸ��������������");
	gotoxy(8,10);
	if(clas==1) printf("2ϡ�У���ÿ����һ�����鼮������һ����");
	if(clas==2) printf("0ϡ�У�����������з���ˮ����Ȼ��ÿ����2�㷨��ˮ�����һ����");
	if(clas==3) printf("1rare: I only know this is a rare cell card.");
	if(clas==4) printf("2ϡ�У���ÿ����һ�����������һ����");
	if(clas==5) printf("2ϡ�У�ʹ������ӵ�����ֵ��Ϊ1");
	if(clas==6) printf("3ϡ�У�ʹ�з����Ҳ���ӱ�Ϊ0-0");
	if(clas==7) printf("0ϡ�У�ʹ����������ƻ��-1/+1+1������ƿ�ÿ��һ���ƣ����Ƶķ���ֵ���ı�����1��");
	if(clas==8) printf("2ϡ�У������ֵ�һ������ϴ������ƿ�");
	if(clas==9) printf("6ϡ�У��ٻ����ƿ���һ��6�������ϵ���ӵĸ���");
	gotoxy(9,10);
	if(clas==1) printf("2ʷʫ��ʹ��������鼮�����������");
	if(clas==2) printf("6ʷʫ������ķ���ˮ����Ϊ������2-2�ġ�ֱ�ߡ�");
	if(clas==3) printf("3epic: I only know this is an epic cell card.");
	if(clas==4) printf("2ʷʫ���ݻ�������з���ˮ�����ٻ�һ������ֵ��С��6�Ĵ�˵��ӡ��⡱���ڸ����������ԭ��ķ���ˮ��");
	if(clas==5) printf("3ʷʫ���Ƴ����й�������ͬ��������ֵ�����");
	if(clas==6) printf("5ʷʫ��ʹ˫��Ӣ�۱�Ϊ30Ѫ");
	if(clas==7) printf("3ʷʫ����Ķ��ֳ�2���ƣ�Ϊ���Ӣ�ۻָ�8������ֵ");
	if(clas==8) printf("2ʷʫ��ʹ���������ӻ��+1+1������1���ƺ�������");
	if(clas==9) printf("2ʷʫ���������������������Ƶĸ���ϴ������ƿ�");

	gotoxy(5,65);
	printf("��ѡ���ƣ�");
	gotoxy(20,8);
	printf("�ܼƣ�");
	for(int i=1;i<=10;i++)
		gotoxy(20+i,8),printf("%2d : ",i);
	int jilu[20];
	for(int i=1;i<=10;i++) jilu[i]=0;
	int choose=0;
	for(int i=1;i<=30;i++)
	{
		gotoxy(5,75);printf("��%2d��",i-1);
		create1();
		print_card(12,18,1);
		print_card(12,32,2);
		print_card(12,46,3);
		int ow=1;
		while(ow)
		{
			if(KEY_DOWN(MOUSE_MOVED))
			{
				search();
				change(X,Y);
				if(xx>11 && xx<17 && yy>17 && yy<22) choose=1,ow=0;
				if(xx>11 && xx<17 && yy>31 && yy<36) choose=2,ow=0;
				if(xx>11 && xx<17 && yy>45 && yy<50) choose=3,ow=0;
				if(!ow)
				{
					print_card(12,14,0);
					print_card(12,28,0);
					print_card(12,42,0);
				}
				Sleep(750);
			}
		}
		card1[i]=card[choose];
		gotoxy(5+i,75);
		if(card[choose].spell) printf("%2d/      ����",card[choose].cost);
		else
		{
			printf("%2d/%2d-%2d ",card[choose].cost,card[choose].attack,card[choose].health);
			if(card[choose].quality==2) printf("  B : %d",card[choose].treat);
			if(card[choose].quality==3) printf("  D : %d",card[choose].death);
			if(card[choose].quality==4) printf("  A : %d",card[choose].time);
		}
		gotoxy(5,75);printf("��%2d��",i);
		jilu[card[choose].cost]++;
		int t=card[choose].cost;
		gotoxy(20+t,14);printf("%2d",jilu[t]);
	}
	gotoxy(32,8);
	pause();
}
void arena_mode()//����ģʽ
{
	system("cls");chapter=0;
	gotoxy(2,0);
	printf("     ��ӭ��������ģʽ��        ����������");
	gotoxy(4,0);
	printf("        ��ѡ��ְҵ��         �����ϸ���Ϸ\n");
	gotoxy(6,10);printf("%s",occu[1]);
	gotoxy(6,30);printf("%s",occu[2]);
	gotoxy(6,50);printf("%s",occu[3]);
	gotoxy(8,10);printf("%s",occu[4]);
	gotoxy(8,30);printf("%s",occu[5]);
	gotoxy(8,50);printf("%s",occu[6]);
	gotoxy(10,10);printf("%s",occu[7]);
	gotoxy(10,30);printf("%s",occu[8]);
	gotoxy(10,50);printf("%s",occu[9]);
	gotoxy(13,6);printf("��%3.0lf%%��2���ģ��ٻ�һ��1-2�ġ��鼮����",1.0*win[1]/(win[1]+lose[1])*100);
	gotoxy(14,6);printf("��%3.0lf%%��4��ѧ�����һ���յķ���ˮ��������10�㷨��ˮ����ĳ�һ���ơ�",1.0*win[2]/(win[2]+lose[2])*100);
	gotoxy(15,6);printf("��%3.0lf%%��0Ӣ�This hero power is unknown.",1.0*win[3]/(win[3]+lose[3])*100);
	gotoxy(16,6);printf("��%3.0lf%%��1�����ݻ����һ������ˮ�����ޣ��ٻ�һ��3-3�ġ������",1.0*win[4]/(win[4]+lose[4])*100);
	gotoxy(17,6);printf("��%3.0lf%%��2��ѧ��ʹ������ӽ�������ֵ�͹�������",1.0*win[5]/(win[5]+lose[5])*100);
	gotoxy(18,6);printf("��%3.0lf%%��1����ٻ�һ��0-0�ġ�ϸ��������ֻ�ܴ��һ���غϡ�",1.0*win[6]/(win[6]+lose[6])*100);
	gotoxy(19,6);printf("��%3.0lf%%��2���Σ��ݻ�˫���ƿ⡣",1.0*win[7]/(win[7]+lose[7])*100);
	gotoxy(20,6);printf("��%3.0lf%%��2��ʷ������һ������Ȼ���һ���ơ�",1.0*win[8]/(win[8]+lose[8])*100);
	gotoxy(21,6);printf("��%3.0lf%%��5����������ƿ��ｫһ���������ս����",1.0*win[9]/(win[9]+lose[9])*100);
	gotoxy(22,6);printf("��%3.0lf%%��2ѧ��������ٻ�һ��������������ֵ��ȵ����(2~5)��",1.0*tlose/(twin+tlose)*100);
	int ow=1,conti=0;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx==2) return;
			if(xx==4 && yy>=26 && yy<=39 && clas) conti=1,ow=0;
			if(xx==6 && yy>= 8 && yy<=13) clas=1,ow=0;
			if(xx==6 && yy>=29 && yy<=34) clas=2,ow=0;
			if(xx==6 && yy>=50 && yy<=55) clas=3,ow=0;
			if(xx==8 && yy>= 8 && yy<=13) clas=4,ow=0;
			if(xx==8 && yy>=29 && yy<=34) clas=5,ow=0;
			if(xx==8 && yy>=50 && yy<=55) clas=6,ow=0;
			if(xx==10&& yy>= 8 && yy<=13) clas=7,ow=0;
			if(xx==10&& yy>=29 && yy<=34) clas=8,ow=0;
			if(xx==10&& yy>=50 && yy<=55) clas=9,ow=0;
			Sleep(500);
		}
	}
	if(!conti) xuanpai();
	onegame();
}
void adven_mode()//ð��ģʽ
{
	system("cls");
	gotoxy(2,0);
	printf("       ��ӭ����ð��ģʽ��        ����������");
	gotoxy(4,0);
	printf("          ��ѡ��ؿ���\n");
	gotoxy(6,8); if(ac[1]) printf("��");else printf("��");
	printf("���գ�����ƿ�ֻ��20�ſ��ơ�");
	gotoxy(8,8); if(ac[2]) printf("��");else printf("��");
	printf("�¿���˫���ƿ�ֻ��10�ſ��ơ�");
	gotoxy(10,8);if(ac[3]) printf("��");else printf("��");
	printf("���У���û���������õ���ӡ�");
	gotoxy(12,8);if(ac[4]) printf("��");else printf("��");
	printf("��ĩ������������Ӽ���1�ѡ�");
	gotoxy(14,8);if(ac[5]) printf("��");else printf("��");
	printf("��Σ��Ծֿ�ʼʱ���������ʼ���ơ�");
	gotoxy(16,8);if(ac[6]) printf("��");else printf("��");
	printf("һģ�������ӶԵз�Ӣ������˺����Ϊ2��");
	gotoxy(18,8);if(ac[7]) printf("��");else printf("��");
	printf("��ģ�����������Ӷ�Ϊ��ͨ����");
	gotoxy(20,8);if(ac[8]) printf("��");else printf("��");
	printf("��ģ�����ֵ�������Ӷ���Ϊ��ͨ����");
	gotoxy(22,8);if(ac[9]) printf("��");else printf("��");
	printf("�߿������ֵ�������Ӷ�Ϊ��˵����Ӣ�ۼ����ٻ�����ӱ�Ϊʷʫ����");
	gotoxy(24,8);
	int ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx== 2) return;
			if(xx== 6) chapter=1,ow=0;
			if(xx== 8) chapter=2,ow=0;
			if(xx==10) chapter=3,ow=0;
			if(xx==12) chapter=4,ow=0;
			if(xx==14) chapter=5,ow=0;
			if(xx==16) chapter=6,ow=0;
			if(xx==18) chapter=7,ow=0;
			if(xx==20) chapter=8,ow=0;
			if(xx==22) chapter=9,ow=0;
			Sleep(500);
		}
	}
	system("cls");
	gotoxy(2,0);
	printf("     ��ѡ���˵�%d�أ�        ����������",chapter);
	gotoxy(4,0);
	printf("        ��ѡ��ְҵ��\n");
	gotoxy(6,10);printf("%s",occu[1]);
	gotoxy(6,30);printf("%s",occu[2]);
	gotoxy(6,50);printf("%s",occu[3]);
	gotoxy(8,10);printf("%s",occu[4]);
	gotoxy(8,30);printf("%s",occu[5]);
	gotoxy(8,50);printf("%s",occu[6]);
	gotoxy(10,10);printf("%s",occu[7]);
	gotoxy(10,30);printf("%s",occu[8]);
	gotoxy(10,50);printf("%s",occu[9]);
	gotoxy(13,8);printf("2���ģ��ٻ�һ��1-2�ġ��鼮����");
	gotoxy(14,8);printf("4��ѧ�����һ���յķ���ˮ��������10�㷨��ˮ����ĳ�һ���ơ�");
	gotoxy(15,8);printf("0Ӣ�This hero power is unknown.");
	gotoxy(16,8);printf("1�����ݻ����һ������ˮ�����ޣ��ٻ�һ��3-3�ġ������");
	gotoxy(17,8);printf("2��ѧ��ʹ������ӽ�������ֵ�͹�������");
	gotoxy(18,8);printf("1����ٻ�һ��0-0�ġ�ϸ��������ֻ�ܴ��һ���غϡ�");
	gotoxy(19,8);printf("2���Σ��ݻ�˫���ƿ⡣");
	gotoxy(20,8);printf("2��ʷ������һ������Ȼ���һ���ơ�");
	gotoxy(21,8);printf("5����������ƿ��ｫһ���������ս����");
	gotoxy(22,8);printf("2ѧ��������ٻ�һ��������������ֵ��ȵ����(2~5)��");
	ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx==2) {chapter=0;return;}
			if(xx==6 && yy>= 8 && yy<=13) clas=1,ow=0;
			if(xx==6 && yy>=29 && yy<=34) clas=2,ow=0;
			if(xx==6 && yy>=50 && yy<=55) clas=3,ow=0;
			if(xx==8 && yy>= 8 && yy<=13) clas=4,ow=0;
			if(xx==8 && yy>=29 && yy<=34) clas=5,ow=0;
			if(xx==8 && yy>=50 && yy<=55) clas=6,ow=0;
			if(xx==10&& yy>= 8 && yy<=13) clas=7,ow=0;
			if(xx==10&& yy>=29 && yy<=34) clas=8,ow=0;
			if(xx==10&& yy>=50 && yy<=55) clas=9,ow=0;
			Sleep(500);
		}
	}
	gotoxy(0,0);
	onegame();
}
void task_mode()//����ģʽ
{
	system("cls");
	gotoxy(2,0);
	printf("       ��ӭ��������ģʽ��        ����������");
	gotoxy(4,0);
	printf("        ��ѡ��ְҵ��\n");
	gotoxy(6,10);printf("%s",occu[1]);
	gotoxy(6,30);printf("%s",occu[2]);
	gotoxy(6,50);printf("%s",occu[3]);
	gotoxy(8,10);printf("%s",occu[4]);
	gotoxy(8,30);printf("%s",occu[5]);
	gotoxy(8,50);printf("%s",occu[6]);
	gotoxy(10,10);printf("%s",occu[7]);
	gotoxy(10,30);printf("%s",occu[8]);
	gotoxy(10,50);printf("%s",occu[9]);
	gotoxy(13,10);printf("���ģ�������ͬʱ���������鼮");
	gotoxy(14,10);printf("      ���������ٻ��ġ��鼮����Ϊ5-6�����");
	gotoxy(15,10);printf("��ѧ��������ʣ�෨��ˮ����δ����5������������ˮ������10��");
	gotoxy(16,10);printf("      ���������Ӣ�ۼ��ܳ���Ʒ���ֵ���ļ���4��");
	gotoxy(17,10);printf("Ӣ�����Sorry,I don't know what this is.                        ");
	gotoxy(18,10);printf("      ������I don't know what this is,either.");
	gotoxy(19,10);printf("�������񣺴ݻ�6������ˮ������");
	gotoxy(20,10);printf("      ���������10�㷨��ˮ��������ʣ������ޣ�");
	gotoxy(21,10);printf("��ѧ��������ʹ��Ӣ�ۼ��ܺ��¶���������ӱ�Ϊ1�㹥�����������1");
	gotoxy(22,10);printf("      ��������������ƵĹ�������Ϊ1��");
	gotoxy(23,10);printf("�������ϸ��������20���˺�");
	gotoxy(24,10);printf("      ���������Ӣ�ۼ��ܸ�Ϊ�ٻ�3��ϸ��");
	gotoxy(25,10);printf("���Σ������ܵ�6��ƣ���˺�");
	gotoxy(26,10);printf("      ��������6�Ŵ�˵����������ƿ�");
	gotoxy(27,10);printf("��ʷ����������3�ŷ���ͨƷ�ʵĿ���");
	gotoxy(28,10);printf("      ������ÿ���غϿ�ʼʱ�ٻ�1��3-3�����");
	gotoxy(29,10);printf("����������ļ3��3�������µ����");
	gotoxy(30,10);printf("      ��������ļ3�����");
	gotoxy(31,10);printf("ѧ��������ʹ��6��Ӣ�ۼ���");
	gotoxy(32,10);printf("      ������Ӣ�ۼ�����Զ�ٻ�5-5�����");
	int ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx==2) {chapter=0;return;}
			if(xx==6 && yy>= 8 && yy<=13) clas=1,ow=0;
			if(xx==6 && yy>=29 && yy<=34) clas=2,ow=0;
			if(xx==6 && yy>=50 && yy<=55) clas=3,ow=0;
			if(xx==8 && yy>= 8 && yy<=13) clas=4,ow=0;
			if(xx==8 && yy>=29 && yy<=34) clas=5,ow=0;
			if(xx==8 && yy>=50 && yy<=55) clas=6,ow=0;
			if(xx==10&& yy>= 8 && yy<=13) clas=7,ow=0;
			if(xx==10&& yy>=29 && yy<=34) clas=8,ow=0;
			if(xx==10&& yy>=50 && yy<=55) clas=9,ow=0;
			Sleep(500);
		}
	}
	gotoxy(0,0);
	onegame();
}
void mainmenu()//������
{
	system("cls");
	gotoxy(0,0);
	printf("\n\n        ��ӭ������¯Сʯ���������Ԫ\n\n\n");
	printf("\n            ����������ģʽ\n");
	printf("\n            ������뾺��ģʽ\n");
	printf("\n            �������ð��ģʽ\n");
	printf("\n            �����������ģʽ\n");
	printf("\n            ����ģʽ��������\n");
	printf("\n            ����\n");
	int ow=1;
	while(ow)
	{
		if(KEY_DOWN(MOUSE_MOVED))
		{
			search();
			change(X,Y);
			if(xx==6) mode=1,ow=0;
			if(xx==8) mode=2,ow=0;
			if(xx==10) mode=3,ow=0;
			if(xx==12) mode=4,ow=0;
			Sleep(300);
		}
	}
	if(mode==1) rand_mode();
	if(mode==2) arena_mode();
	if(mode==3) adven_mode();
	if(mode==4) task_mode();
}
int main()//������
{
	initialize();
	while(1) mainmenu();
	return 0;
}
//if(KEY_DOWN(MOUSE_MOVED)  KEY_DOWN(MOUSE_EVENT)   KEY_DOWN(MOUSE_WHEELED)
