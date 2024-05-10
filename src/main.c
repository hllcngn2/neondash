#include <stdlib.h>
#include <ncurses.h>
#include "controls.h"
//windows id
#define LEFT	0
#define MID	1
#define RIGHT	2
//palettes id
#define B	0
#define F	1
typedef struct{
	int	range;
	WINDOW*	w;
	int	x,y;
	int	width;	}palette;
typedef struct{
	int	b,f;	}paint;
typedef struct{
	int	x,y;	}curs;
void create_ui(WINDOW** w);
void create_canvas(WINDOW** w,int h,int width);
void delete_windows(WINDOW** w);
void create_palettes(palette** p,WINDOW** w);
void delete_palettes(palette** p);
int main2(WINDOW** w,curs* cursor,palette** p,paint* pnt);
int in_palette(palette* p,int* pnt);
void display_color_palette(palette* pal,int pnt);
void update_curs_palette(palette* p,int pnt);
int pick_color();

int main(int ac, char** av){
initscr();
 noecho(); curs_set(0);
 cbreak(); //nodelay(stdscr,TRUE);
 start_color(); refresh();
WINDOW** w =malloc(sizeof(WINDOW*)*3);
 create_ui(w);
 create_canvas(w,5+2,10+2);
palette** p =malloc(sizeof(palette)*2);
 create_palettes(p,w);
 for(int i=p[B]->range;i<p[B]->range+20;i++)
	init_pair(i,COLOR_BLACK,COLOR_WHITE);
 for(int i=p[F]->range;i<p[F]->range+20;i++)
	init_pair(i,COLOR_WHITE,COLOR_BLACK);
paint* pnt =malloc(sizeof(paint));
 *pnt =(paint){0,0};
curs* cursor =malloc(sizeof(curs));
 *cursor =(curs){0,0};

display_color_palette(p[B],pnt->b);
display_color_palette(p[F],pnt->f);
wrefresh(w[LEFT]);

int ret =main2(w,cursor,p,pnt);

delete_windows(w);
delete_palettes(p);
endwin();
return ret;}


int main2(WINDOW** w,curs* cursor,palette** p,paint* pnt){
int c; while((c=getch())!=K_QUIT){
	int run=1; while(run){
		switch(c){
case K_PAL_BG:	run=in_palette(p[B],&(pnt->b));	break;
case K_PAL_FG:	run=in_palette(p[F],&(pnt->f));	break;
case 'n':	break;
case 'g':	break;
case 'i':
	curs_set(1);
	wmove(w[MID],cursor->y+1,cursor->x+1);
	wrefresh(w[MID]);
		break;
default:					break;
		}
		if(run){ c= run;}
	}};
return 0;}


int in_palette(palette* p,int* pnt){
	int ret=0;
int c=0; while(c!=K_ESC){ curs_set(1);
display_color_palette(p,*pnt);
update_curs_palette(p,*pnt);
wrefresh(p->w); switch((c=getch())){
case K_ADD:  int coln =p->range+*pnt;
	if(pick_color(coln)!=-1)
		init_pair(coln,COLOR_BLACK,coln);	break;
case K_LEFT:	if(*pnt%p->width)     (*pnt)--;		break;
case K_DOWN:	if(*pnt<20-p->width)  *pnt+=p->width;	break;
case K_UP:	if(*pnt>p->width-1)   *pnt-=p->width;	break;
case K_RIGHT:	if((*pnt+1)%p->width) (*pnt)++;		break;
case K_PAL_FG:	ret= K_PAL_FG; c= K_ESC; break;
case K_PAL_BG:	ret= K_PAL_BG; c= K_ESC; break;
default:						break;}}
curs_set(0); wrefresh(p->w);	return ret;}


void display_color_palette(palette* p,int pnt){
int range=	p->range;
WINDOW* w=	p->w;
int y= p->y,	x= p->x;
int width=	p->width;
for(int i=0;i<20;i++){
	if(!(i%width)) wmove(w,y+i/width,x);
	wattron(w,COLOR_PAIR(range+i));
	if(i==pnt)	wprintw(w,"XX");
	else		wprintw(w,"xx");
	wattroff(w,COLOR_PAIR(range+i));}
return;}


void update_curs_palette(palette* p,int pnt){
wmove(p->w,	p->y +pnt/p->width,
		p->x +pnt%p->width*2);
return;}


int pick_color(int col){
int ret= -1; int continu= 1; while(continu){
echo(); curs_set(1);
mvprintw(3,24,"Choose a color: "); move(4,24);
int r1,r2,r3, g1,g2,g3, b1,b2,b3;
r1=r2=r3=g1=g2=g3=b1=b2=b3=0;
r1 =getch()-'0';
r2 =getch();	if(r2!=' '){
		r2-='0';
		r3 =getch();	if(r3!=' '){
				r3-='0';
				getch();}
				else r3=0;}
		else r2=0;
g1 =getch()-'0';
g2 =getch();	if(g2!=' '){
		g2-='0';
		g3 =getch();	if(g3!=' '){
				g3-='0';
				getch();}
				else g3=0;}
		else g2=0;
b1 =getch()-'0';
b2 =getch();	if(b2!=' '){
		b2-='0';
		b3 =getch();	if(b3!=' ') b3-='0';
				else b3=0;}
		else b2=0;
noecho(); curs_set(0);
int r,g,b;
 if(r3)		r= r1*100+r2*10+r3;
 else if(r2)	r= r1*10+r2;
 else		r= r1;
 if(g3)		g= g1*100+g2*10+g3;
 else if(g2)	g= g1*10+g2;
 else		g= g1;
 if(b3)		b= b1*100+b2*10+b3;
 else if(b2)	b= b1*10+b2;
 else		b= b1;
init_color(col,r,g,b);
move(5,24); printw("Your color: ");
 init_pair(1,COLOR_BLACK,col);
 attron(COLOR_PAIR(1));
 printw("  ");
 attroff(COLOR_PAIR(1));
char c; switch((c=getch())){
case K_ADD:	ret= col;
		continu= 0;	break;
case K_CHANGE:			break;
default:	ret= -1;
		continu= 0;	break;}
mvprintw(3,24,"               ");
mvprintw(4,24,"           ");
mvprintw(5,24,"              ");}
curs_set(1); refresh();
return ret;}



void create_ui(WINDOW** w){
 int wlefth =20,
 wleftw =16,
 wlefty =2,
 wleftx =4;
w[LEFT] =newwin(wlefth,wleftw,wlefty,wleftx);
 box(w[LEFT],0,0);
 wrefresh(w[LEFT]);
 int wrighth =20,
 wrightw =16,
 wrighty =2,
 wrightx =46;
w[RIGHT] =newwin(wrighth,wrightw,wrighty,wrightx);
 box(w[RIGHT],0,0);
 wrefresh(w[RIGHT]);
return;}

void create_canvas(WINDOW** w,int h,int width){
 int wmidh =h,
 wmidw =width,
 wmidy =7,
 wmidx =27;
w[MID] =newwin(wmidh,wmidw,wmidy,wmidx);
 box(w[MID],0,0);
 wrefresh(w[MID]);
return;}

void delete_windows(WINDOW** w){
 delwin(w[LEFT]);
 delwin(w[RIGHT]);
 delwin(w[MID]);
free(w);
return;}

void create_palettes(palette** p,WINDOW** w){
p[B] =malloc(sizeof(palette));
 p[B]->range =20;
 p[B]->w =w[LEFT];
 p[B]->y =3;	p[B]->x =3;
 p[B]->width =5;
p[F] =malloc(sizeof(palette));
 p[F]->range =40;
 p[F]->w =w[LEFT];
 p[F]->y =9;	p[F]->x =3;
 p[F]->width =5;
return;}

void delete_palettes(palette** p){
 free(p[B]);
 free(p[F]);
free(p);
return;}
