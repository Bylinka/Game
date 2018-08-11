#include <iostream>
#include <windows.h>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

#include "pgraph.h"
#include "table.h"
#include "iceil.h"
#include "way.h"
#include "gold.h"
#include "create.h"
#include "iunit.h"
#include "draw.h"
#include "player.h"
#include "hunter.h"
#include "guard.h"
#include "psycho.h"
#include "createunits.h"

void main() {
/*
	srand(time(0));
	rand(); rand();
	matrix_size = { 119,29 };
		Draw* d = new Draw();
		while (!d->isAlive());
		Sleep(10);
		auto g = new Gold(&d->matrix()->at(0,0));
		g->fill(100);
		delete g;
		auto w = new Way(&d->matrix()->at(matrix_enter));
		w->show(w->find(d->matrix()->at(matrix_exit)),120,12);
		delete w;
		Sleep(1000);
		Player *p = new Player(&d->matrix()->at(matrix_enter));
		CerateUnits c(p);
		c.hunter(3).guard(6).psycho(12);
	while(!GetAsyncKeyState(VK_ESCAPE));
	d->stop();
	delete d;
	*/
}
