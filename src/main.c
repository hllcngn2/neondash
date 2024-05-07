#include <stdlib.h>
#include <ncurses.h>
#define LEFT	0
#define MID	1
#define RIGHT	2
WINDOW** create_windows();
void pick_color(int* palette);
void display_color_palette(int* palette,WINDOW* w,int y,int x);

int main(int ac, char** av){
initscr();
 //noecho(); curs_set(0);
 cbreak(); //nodelay(stdscr,TRUE);
 start_color(); refresh();
WINDOW** w =create_windows();
int* palette =calloc(20,sizeof(int));
pick_color(palette);
display_color_palette(palette,w[LEFT],3,3);

printw("Hello world");
int color =can_change_color();
printw("%i",color);

getch();
endwin();
return 0;}



WINDOW** create_windows(){
WINDOW** w =malloc(sizeof(WINDOW*)*3);
int wlefth =20,
 wleftw =18,
 wlefty =2,
 wleftx =4;
 w[LEFT] =newwin(wlefth,wleftw,wlefty,wleftx);
box(w[LEFT],0,0);
wrefresh(w[LEFT]);
 int wrighth =20,
 wrightw =18,
 wrighty =2,
 wrightx =wleftx+wleftw+4;
 w[RIGHT] =newwin(wrighth,wrightw,wrighty,wrightx);
box(w[RIGHT],0,0);
wrefresh(w[RIGHT]);
 int wmidh =20,
 wmidw =20,
 wmidy =2,
 wmidx =wrightx+wrightw+4;
 w[MID] =newwin(wmidh,wmidw,wmidy,wmidx);
box(w[MID],0,0);
wrefresh(w[MID]);
return w;}



void pick_color(int* palette){
printw("\nChoose a color: ");
int r1,r2,r3, g1,g2,g3, b1,b2,b3;
r1=r2=r3=g1=g2=g3=b1=b2=b3=0;
r1 =getch()-'0';
r2 =getch();
if(r2!=' '){
	r2-='0';
	r3 =getch();
	if(r3!=' '){
		r3-='0';
       		getch();}
	else r3=0;}
else r2=0;
g1 =getch()-'0';
g2 =getch();
if(g2!=' '){
	g2-='0';
	g3 =getch();
	if(g3!=' '){
		g3-='0';
		getch();}
	else g3=0;}
else g2=0;
b1 =getch()-'0';
b2 =getch();
if(b2!=' '){
	b2-='0';
	b3 =getch();
	if(b3!=' ') b3-='0';
	else b3=0;}
else b2=0;
int r,g,b;
if(r3) r=r1*100+r2*10+r3;
else if(r2) r=r1*10+r2;
else r=r1;
if(g3) g=g1*100+g2*10+g3;
else if(g2) g=g1*10+g2;
else g=g1;
if(b3) b=b1*100+b2*10+b3;
else if(b2) b=b1*10+b2;
else b=b1;
printw("Your color: ");
printw("%i,%i,%i",r,g,b);
init_color(20,r,g,b);
init_pair(1,COLOR_BLACK,20);
attron(COLOR_PAIR(1));
printw("   ");
palette[0]=1;
return;}



void display_color_palette(int* palette,WINDOW* w,int y,int x){
wmove(w,y,x);
int width=10;
for(int i=0;i<width;i++){
	wattron(w,COLOR_PAIR(palette[i]));
	wprintw(w,"  ");
	wattroff(w,COLOR_PAIR(palette[i]));}
wrefresh(w);
return;}
