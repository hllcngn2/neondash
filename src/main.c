#include <stdlib.h>
#include <ncurses.h>
#define K_QUIT	'q'
#define LEFT	0
#define MID	1
#define RIGHT	2
#define B	0
#define F	1
typedef struct{
	int	n;
	int	range;
	WINDOW*	w;
	int	x,y;
	int	width;	}palette;
typedef struct{
	int	b,f;	}paint;
void create_ui(WINDOW** w);
void create_canvas(WINDOW** w,int h,int width);
void delete_windows(WINDOW** w);
void create_palettes(palette** p,WINDOW** w);
void delete_palettes(palette** p);
int main2(WINDOW** w,palette** p,paint* pnt);
void update_curs_palette(palette* p,int pnt);
int pick_color();
void display_color_palette(palette* pal,int pnt);

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

display_color_palette(p[B],pnt->b);
display_color_palette(p[F],pnt->f);
wrefresh(w[LEFT]);

int ret =main2(w,p,pnt);

delete_windows(w);
delete_palettes(p);
endwin();
return ret;}


int main2(WINDOW** w,palette** p,paint* pnt){
char c; while((c=getch())!=K_QUIT){ switch(c){
case 'b':  curs_set(1); update_curs_palette(p[B],pnt->b);
	wrefresh(p[B]->w);
	while((c=getch())!=27){ switch(c){
	case 'a':  int coln =p[B]->range+p[B]->n;
		if(pick_color(coln)!=-1){
			init_pair(coln,COLOR_BLACK,coln);
			display_color_palette(p[B],pnt->b);
			p[B]->n++;}				break;
	case 'h':  if(pnt->b%p[B]->width){	pnt->b--;
			update_curs_palette(p[B],pnt->b); }	break;
	case 'j':  if(pnt->b<20-p[B]->width){	pnt->b+=p[B]->width;
			update_curs_palette(p[B],pnt->b); }	break;
	case 'k':  if(pnt->b>p[B]->width-1){	pnt->b-=p[B]->width;
			update_curs_palette(p[B],pnt->b); }	break;
	case 'l':  if((pnt->b+1)%p[B]->width){	pnt->b++;
			update_curs_palette(p[B],pnt->b); }	break;
	default:						break;}
	wrefresh(p[B]->w);} curs_set(0); wrefresh(p[B]->w); break;
case 'f':  curs_set(1); update_curs_palette(p[F],pnt->f);
	wrefresh(p[F]->w);
	while((c=getch())!=27){ switch(c){
	case 'a':  int coln =p[F]->range+p[F]->n;
		if(pick_color(coln)!=-1){
			init_pair(coln,coln,COLOR_BLACK);
			display_color_palette(p[F],pnt->f);
			p[F]->n++;}				break;
	case 'h':  if(pnt->f%p[F]->width){	pnt->f--;
			update_curs_palette(p[F],pnt->f); }	break;
	case 'j':  if(pnt->f<20-p[F]->width){	pnt->f+=p[F]->width;
			update_curs_palette(p[F],pnt->f); }	break;
	case 'k':  if(pnt->f>p[F]->width-1){	pnt->f-=p[F]->width;
			update_curs_palette(p[F],pnt->f); }	break;
	case 'l':  if((pnt->f+1)%p[F]->width){	pnt->f++;
			update_curs_palette(p[F],pnt->f); }	break;
	default:						break;}
	wrefresh(p[F]->w);} curs_set(0); wrefresh(p[F]->w); break;
default: break;}};
return 0;}



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
int ret= -1;
int continu= 1; while(continu){
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
init_color(col,r,g,b);
printw("Your color: ");
init_pair(1,COLOR_BLACK,col);
attron(COLOR_PAIR(1));
printw("  ");
attroff(COLOR_PAIR(1));
noecho(); curs_set(0);
char c; switch((c=getch())){
case 'a':	ret= col;
		continu= 0;	break;
case 's':			break;
case 'd':	ret= -1;
		continu= 0;	break;
default:	ret= -1;
		continu= 0;	break;}
mvprintw(3,24,"               ");
mvprintw(4,24,"           ");
mvprintw(5,24,"              ");}
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
 p[B]->n =0;	p[B]->range =20;
 p[B]->w =w[LEFT];
 p[B]->y =3;	p[B]->x =3;
 p[B]->width =5;
p[F] =malloc(sizeof(palette));
 p[F]->n =0;	p[F]->range =40;
 p[F]->w =w[LEFT];
 p[F]->y =9;	p[F]->x =3;
 p[F]->width =5;
return;}

void delete_palettes(palette** p){
 free(p[B]);
 free(p[F]);
free(p);
return;}
