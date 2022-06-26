#include <iostream>
#include "Menu.h"
#include "conio.h"



using namespace std;


int main()
{
	Menu* menu = new Menu();
	menu->MenuPrincipal();

	_getch();

}
