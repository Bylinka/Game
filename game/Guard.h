#pragma once
class Guard
	: public IUnit
{
	int direction;
public:
	Type getType() { return Type::guard; }
	Guard()
		:IUnit()
		, direction(0)
	{
	}

	SafeCeil& nextCeil(int i) {
		switch (i) {
		case 0: return  self().left();
		case 1: return  self().right();
		case 2: return  self().up();
		case 3: return  self().down();
		}
		return self();
	}
	SafeCeil& newDirection() {
		int d;
		SafeCeil* next = &self();
		bool b = false;
		do{
			d = rand() % 4;
			if (d == direction)continue;
			next = &nextCeil(d);
			next->lock();
			b = canMove(next);
			next->unlock();
		} while (!b);
		direction = d;
		return *next;
	}
	void move() {
		vector<COORD> way;		
		while (isAlive()) {
			SafeCeil* next = &nextCeil(direction);
			next->lock();
			bool b = canMove(next);
			next->unlock();
			if(b){
				go(next);
			}
			else {
				go(&newDirection());
			}
			this_thread::sleep_for(chrono::milliseconds(Speed()));

//			Sleep(Speed());
		}
	}
};
