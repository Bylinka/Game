#include <iostream>
#include <windows.h>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>
#include <atomic>
#include <mutex>
#pragma inline_depth(10)


using namespace std;
#pragma comment(lib, "user32")
#pragma comment(lib, "kernel32")
#include "draw.h"


void main() {

	system("mode con cols=91 lines=48");
	system("mode con cp select=866");
	srand(time(0));
	rand(); rand();
	Draw d({ 91,47 });
/*		Sleep(1000);
		Player *p = new Player(&d->matrix()->at(matrix_enter));
		CerateUnits c(p);
		c.hunter(3).guard(6).psycho(12);*/
	while(!GetAsyncKeyState(VK_ESCAPE))	this_thread::sleep_for(chrono::milliseconds(100));

	d.stop();
}
