#include <ncurses.h>
void pick_color();

int main(int ac, char** av){
initscr();
 //noecho(); curs_set(0);
 cbreak(); //nodelay(stdscr,TRUE);
 start_color(); refresh();

/*
WINDOW* wleft;
{int wlefth =
*/

printw("Hello world");
int color =can_change_color();
printw("%i",color);

pick_color();

getch();

endwin();
return 0;}

void pick_color(){
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
return;}
