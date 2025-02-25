#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "controls.h"
//windows id
#define LEFT	0
#define MID	1
#define RIGHT	2
//pals id
#define B	0
#define F	1
typedef struct{
	int	h,w;
	int**	c;
	int**	cp;	}vign;
typedef struct{
	int	b,f;	}paint;
typedef struct{
	int	x,y;	}curs;
typedef struct{
	int	range;
	WINDOW*	win;
	int	x,y;
	int	w;	}pal;
int main2(WINDOW** win,vign* v,curs* cur,pal** pl,paint* pnt);
int in_vign(WINDOW* wi,vign* v,curs* cur,paint* pnt);
int in_pal(pal* pl,int* pnt,int pnt2,int d);
void display_color_pal(pal* pl,int pnt);
void display_canvas(WINDOW* wi,vign* v);
void display_symbol_pal(WINDOW* wi,int h,int w);
void redisplay(WINDOW** win, pal** pl, paint* pnt, vign* v);
void update_curs_pal(pal* pl,int pnt);
void create_ui(WINDOW** win);
void create_canvas(WINDOW** win,int h,int w);
void delete_windows(WINDOW** win);
vign* create_vign(int h,int w);
void delete_vign(vign* v);
pal** create_pals(WINDOW** win);
void delete_pals(pal** pl);
void save(vign* v);
int pick_color();

int main(int ac, char** av){
initscr();
 noecho(); curs_set(0);
 cbreak(); //nodelay(stdscr,TRUE);
 start_color(); refresh();

WINDOW** win =malloc(sizeof(WINDOW*)*3);
 create_ui(win);
 int h,w; if(ac==3){ h =atoi(av[1]);
	if(h>10 || h<=0) h =10;
	w =atoi(av[2]);
	if(w>20 || w<=0) w =20;}
 else{	h =5;	w =10;}
 create_canvas(win,h+2,w+2);
vign* v =create_vign(h,w);
pal** pl =create_pals(win);
paint* pnt =malloc(sizeof(paint));
 *pnt =(paint){0,0};
curs* cur =malloc(sizeof(curs));
 *cur =(curs){0,0};

redisplay(win, pl, pnt, v);

int ret =main2(win,v,cur,pl,pnt);

delete_windows(win);
delete_vign(v);
delete_pals(pl);
free(pnt);
free(cur);

endwin();
return ret;}

void redisplay(WINDOW** win, pal** pl, paint* pnt, vign* v){
erase(); refresh();
display_color_pal(pl[B],pnt->b);
display_color_pal(pl[F],pnt->f);
box(win[LEFT],0,0);
wrefresh(win[LEFT]);
display_symbol_pal(win[RIGHT],20,16);
box(win[RIGHT],0,0);
wrefresh(win[RIGHT]);
display_canvas(win[MID],v);
box(win[MID],0,0);
wrefresh(win[MID]);}



int main2(WINDOW** win,vign* v,curs* cur,pal** pl,paint* pnt){
int c; while((c=getch())!=K_QUIT){
while(c){ switch(c){
case K_PAL_BG:	c=in_pal(pl[B],&(pnt->b),pnt->f,'b'); break;
case K_PAL_FG:	c=in_pal(pl[F],&(pnt->f),pnt->b,'f'); break;
case K_CANVAS:	c=in_vign(win[MID],v,cur,pnt);	break;
case K_SAVE:	save(v); redisplay(win, pl, pnt, v); c =0; break;
default:	c =0;					break;}
}};
return 0;}



int in_vign(WINDOW* wi,vign* v,curs* cur,paint* pnt){
int ret=0; curs_set(1);
int c=0; while(c!=K_ESC){
wmove(wi,cur->y+1,cur->x+1);
wrefresh(wi);
switch((c=getch())){
case K_DASH:	curs_set(0); wrefresh(wi);
		if((c=getch())>=32 && c<=126){
			v->c[cur->y][cur->x] =c;
			v->cp[cur->y][cur->x] =(pnt->b)*24+pnt->f;}
		display_canvas(wi,v);
		curs_set(0);	break;
case K_LEFT:	if(cur->x>0)    cur->x--;
		curs_set(1);	break;
case K_DOWN:	if(cur->y<v->h-1)  cur->y++;
		curs_set(1);	break;
case K_UP:	if(cur->y>0)    cur->y--;	
		curs_set(1);	break;
case K_RIGHT:	if(cur->x<v->w-1) cur->x++;
		curs_set(1);	break;
case K_PAL_FG:
case K_PAL_BG:
case K_CANVAS:	ret= c; c= K_ESC;			break;
default:						break;}}
curs_set(0); wrefresh(wi);
return ret;}


int in_pal(pal* p,int* pnt,int pnt2,int d){ int ret=0;
int c=0; while(c!=K_ESC){
curs_set(1);
display_color_pal(p,*pnt);
update_curs_pal(p,*pnt);
wrefresh(p->win);
int col; switch((c=getch())){
case K_ADD:	col =p->range+*pnt;
		if(pick_color(col)!=-1){
		if(d=='b'){
			init_pair(col,COLOR_BLACK,col);
			init_pair(100+(*pnt)*24+pnt2,20+*pnt,50+pnt2);}
		else if(d=='f'){
			init_pair(col,col,COLOR_BLACK);
			init_pair(100+pnt2*24+*pnt,20+pnt2,50+*pnt);}}
					break;
case K_LEFT:	if(*pnt%p->w)     (*pnt)--;		break;
case K_DOWN:	if(*pnt<20-p->w)  *pnt+=p->w;	break;
case K_UP:	if(*pnt>p->w-1)   *pnt-=p->w;	break;
case K_RIGHT:	if((*pnt+1)%p->w) (*pnt)++;		break;
case K_PAL_FG:
case K_PAL_BG:
case K_CANVAS:	ret= c; c= K_ESC;			break;
default:						break;}}
curs_set(0); wrefresh(p->win);
return ret;}



void display_color_pal(pal* p,int pnt){
for(int n=0;n<24;n++){
	if(!(n%p->w))
		wmove(p->win,p->y+n/p->w,p->x);
	wattron(p->win,COLOR_PAIR(p->range+n));
	if(n==pnt)	wprintw(p->win,"XX");
	else		wprintw(p->win,"xx");
	wattroff(p->win,COLOR_PAIR(p->range+n));}
return;}

void display_canvas(WINDOW* wi,vign* v){
for(int y=0;y<v->h;y++){
	wmove(wi,y+1,1);
	for(int x=0;x<v->w;x++){
		wattron(wi,COLOR_PAIR(v->cp[y][x]));
		waddch(wi,v->c[y][x]);
		wattroff(wi,COLOR_PAIR(v->cp[y][x]));}}
return;}

void display_symbol_pal(WINDOW* wi,int h,int w){
for(int c=32,n=0;c<=126;c++,n++){
	if(!(n%((w-2)/2)))
		wmove(wi,n/((w-2)/2)+1,1);
	waddch(wi,' '); waddch(wi,c);}
return;}



void update_curs_pal(pal* p,int pnt){
wmove(p->win, p->y +pnt/p->w,
	      p->x +pnt%p->w*2);
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
noecho(); //curs_set(0);

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
//printw("%i,%i,%i,col=%i,%i",r,g,b,col,can_change_color());
 init_pair(2,COLOR_BLACK,col);
 attron(COLOR_PAIR(2));
 printw("  "); curs_set(0); refresh();
 attroff(COLOR_PAIR(2));
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



void create_ui(WINDOW** win){
 int wlefth =16,
 wleftw =16,
 wlefty =2,
 wleftx =4;
win[LEFT] =newwin(wlefth,wleftw,wlefty,wleftx);
 box(win[LEFT],0,0);
 wrefresh(win[LEFT]);
 int wrighth =16,
 wrightw =17,
 wrighty =2,
 wrightx =46;
win[RIGHT] =newwin(wrighth,wrightw,wrighty,wrightx);
 box(win[RIGHT],0,0);
 wrefresh(win[RIGHT]);
return;}

void create_canvas(WINDOW** win,int h,int w){
 int wmidh =h,
 wmidw =w,
 wmidy =16/2-(h-1)/2+1,
 wmidx =(46+16)/2-w/2+2;
win[MID] =newwin(wmidh,wmidw,wmidy,wmidx);
 box(win[MID],0,0);
 wrefresh(win[MID]);
return;}

void delete_windows(WINDOW** win){
 delwin(win[LEFT]);
 delwin(win[RIGHT]);
 delwin(win[MID]);
free(win);
return;}

vign* create_vign(int h,int w){
vign* v =malloc(sizeof(vign));
v->h =h;
v->w =w;
 v->c =malloc(sizeof(int*)*h);
 v->cp =malloc(sizeof(int*)*h);
 for(int y=0;y<h;y++){
	v->c[y] =malloc(sizeof(int)*w);
	v->cp[y] =malloc(sizeof(int)*w);
	for(int x=0;x<w;x++){
		v->c[y][x] =' ';
		v->cp[y][x] =1;}}
return v;}

void delete_vign(vign* v){
for(int y=0;y<v->h;y++){
	free(v->c[y]);
	free(v->cp[y]);}
free(v->c);
free(v->cp);
free(v);
return;}

pal** create_pals(WINDOW** win){
pal** pl =malloc(sizeof(pal)*2);
pl[B] =malloc(sizeof(pal));
 pl[B]->range =20;
 pl[B]->win =win[LEFT];
 pl[B]->y =2;	pl[B]->x =3;
 pl[B]->w =5;
pl[F] =malloc(sizeof(pal));
 pl[F]->range =50;
 pl[F]->win =win[LEFT];
 pl[F]->y =9;	pl[F]->x =3;
 pl[F]->w =5;
for(int i=pl[B]->range;i<pl[B]->range+24;i++)
	init_pair(i,COLOR_BLACK,COLOR_WHITE);
for(int i=pl[F]->range;i<pl[F]->range+24;i++)
	init_pair(i,COLOR_WHITE,COLOR_BLACK);
for(int i=100;i<100+24*24;i++)
	init_pair(i,COLOR_WHITE,COLOR_BLACK);
return pl;}

void delete_pals(pal** pl){
 free(pl[B]);
 free(pl[F]);
free(pl);
return;}


void save(vign* v){
WINDOW* wi =newwin(4,30,(20+4)/2-4/2,(46+16+2)/2-30/2+1);
mvwprintw(wi,1,30/2-7/2-1,"save as");
box(wi,0,0);
curs_set(1); wmove(wi,2,5);
wrefresh(wi);
char* filename =malloc(20);
char c; int i; for(i=0;(c=getch())!='\n' && i<20;i++){
	waddch(wi,c); wrefresh(wi);
	filename[i] =c;}
filename[i] ='\0';
char* path =calloc(32,1);
strncat(path,"vignettes/",11);
strncat(path,filename,20);
mkdir("./vignettes", 0700);
FILE* f =fopen(path,"w+");
for(int h=0;h<v->h;h++){
	for(int w=0;w<v->w;w++){
		fputc(v->c[h][w],f);}
	fputc('\n',f);}
fclose(f);
delwin(wi);
curs_set(0);
return;}
