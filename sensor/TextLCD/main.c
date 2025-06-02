#include "textlcd.h"

int main(){
	textLCDInit();
	textLCDWrite("hello", 1);
	textLCDWrite("This is Jaewon", 2);
	
	sleep(3);
	textLCDExit();
}
