#include <stdlib.h>
#include <ncurses.h>
#define K_QUIT	'q'
#define LEFT	0
#define MID	1
#define RIGHT	2
#define B	0
#define F	1
typedef struct{
	int*	col;
	int	n;	}palette;
void create_ui(WINDOW** w);
void create_canvas(WINDOW** w,int h,int width);
void create_palettes(palette** p);
int main2(WINDOW** w,palette** p);
int pick_color();
void display_color_palette(palette* pal,WINDOW* w,int y,int x);
void delete_windows(WINDOW** w);
void delete_palettes(palette** p);

int main(int ac, char** av){
initscr();
noecho(); curs_set(0);
cbreak(); //nodelay(stdscr,TRUE);
start_color(); refresh();
WINDOW** w =malloc(sizeof(WINDOW*)*3);
create_ui(w);
create_canvas(w,5+2,10+2);
palette** p =malloc(sizeof(palette)*2);
create_palettes(p);


display_color_palette(p[B],w[LEFT],3,3);
display_color_palette(p[F],w[LEFT],9,3);

int ret =main2(w,p);


delete_windows(w);
delete_palettes(p);
endwin();
return ret;}



int main2(WINDOW** w,palette** p){
char c; while((c=getch())!=K_QUIT){ switch(c){
case 'b':
	if((c=getch())=='a'){
		int col =pick_color(20+p[B]->n);
		init_pair(20+p[B]->n,COLOR_BLACK,col);
		p[B]->col[p[B]->n]=20+p[B]->n; p[B]->n++;
		display_color_palette(p[B],w[LEFT],3,3);}
	break;
case 'f':
	if((c=getch())=='a'){
		int col =pick_color(40+p[F]->n);
		init_pair(40+p[F]->n,col,COLOR_BLACK);
		p[F]->col[p[F]->n]=40+p[F]->n; p[F]->n++;
		display_color_palette(p[F],w[LEFT],9,3);}
	break;
default:	break;}};
return 0;}




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

void create_palettes(palette** p){
p[B] =malloc(sizeof(palette));
p[B]->col =calloc(20,sizeof(int));
p[B]->n =0;
p[F] =malloc(sizeof(palette));
p[F]->col =calloc(20,sizeof(int));
p[F]->n =0;
return;}

void delete_palettes(palette** p){
free(p[B]->col);
free(p[B]);
free(p[F]->col);
free(p[F]);
free(p);
return;}



int pick_color(int col){
echo(); curs_set(1);
mvprintw(3,24,"Choose a color: ");
move(4,24);
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
move(5,24);
printw("Your color: ");
//printw("%i,%i,%i",r,g,b);
init_color(col,r,g,b);
init_pair(1,COLOR_BLACK,col);
attron(COLOR_PAIR(1));
printw("  ");
attroff(COLOR_PAIR(1));
noecho(); curs_set(0);
int ret=-1; char c; switch((c=getch())){
case 'a':	ret= col;	break;
case 'd':	ret= -1;	break;
default:	ret= -1;	break;}
mvprintw(3,24,"               ");
mvprintw(4,24,"           ");
mvprintw(5,24,"              ");
return ret;}



void display_color_palette(palette* pal,WINDOW* w,int y,int x){
int width=5;
for(int i=0;i<20;i+=width){
wmove(w,y+i/width,x);
for(int j=0;j<width;j++){
	wattron(w,COLOR_PAIR(pal->col[i+j]));
	wprintw(w,"XX");
	wattroff(w,COLOR_PAIR(pal->col[i+j]));}}
wrefresh(w);
return;}
