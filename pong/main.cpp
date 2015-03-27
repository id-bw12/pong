#include <iostream>
#include "menuClass.h"

int main()
{

	MenuClass *menu = new MenuClass();

	menu->startPong();

	delete menu;
	menu = nullptr;

	return 0;
}