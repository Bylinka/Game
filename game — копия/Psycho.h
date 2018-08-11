#pragma once
class Psycho
	: public IUnit
{
public:
	Type getType() { return Type::psycho; }
	Psycho(ICeil**self, IUnit* player)
		:IUnit(self, player) {}

	ICeil*& nextCeil() {
		switch (rand()%4) {
		case 0: return left();
		case 1: return  right();
		case 2: return  up();
		case 3: return  down();
		}
	}
	void move() {
		while (isAlive()) {
			go(nextCeil());
			Sleep(Speed());
		}
	}
};
