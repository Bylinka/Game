#pragma once
class Guard
	: public IUnit
{
	int direction;
public:
	Type getType() { return Type::guard; }
	Guard(ICeil**self, IUnit* player)
		:IUnit(self, player)
		, direction(0)
	{
		start();
	}

	ICeil*& nextCeil(int i) {
		switch (i) {
		case 0: return  left();
		case 1: return  right();
		case 2: return  up();
		case 3: return  down();
		}
	}
	ICeil*& newDirection() {
		int d;
		ICeil** next = matrix();
		do{
			d = rand() % 4;
			if (d == direction)continue;
			next = &nextCeil(d);
		} while (!canMove(*next));
		direction = d;
		return *next;
	}
	void move() {
		vector<COORD> way;		
		while (isAlive()) {
			ICeil* next = nextCeil(direction);
			if(canMove(next)){
				go(next);
			}
			else {
				go(newDirection());
			}
			Sleep(Speed());
		}
	}
};
