#pragma once
class Psycho
	: public IUnit
{
public:
	Type getType() { return Type::psycho; }

	SafeCeil& nextCeil() {
		switch (rand()%4) {
		case 0: return  self().left();
		case 1: return  self().right();
		case 2: return  self().up();
		case 3: return  self().down();
		}
	}
	void move() {
		while (isAlive()) {
			go(&nextCeil());
			this_thread::sleep_for(chrono::milliseconds(Speed()));
			//			Sleep(Speed());
		}
	}
};
