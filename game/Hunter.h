#pragma once
class Hunter
	: public IUnit
{
	ICeil* player;
	int reread;
public:
	Type getType() { return Type::hunter; }
	Hunter(ICeil* player)
		:IUnit()
		, reread(0)
		, player(player)
	{
	}
	void move() {
		vector<COORD> way;
		

		
		while (isAlive()) {
			if (reread == 25) {
				way = Way(&player->self()).find(&self());
				reread = 0;
			}
			if (way.size()) {
				COORD c = way[0];
				way.erase(way.begin());
				go(&self()(c));
			}
			else {
				switch (rand() % 4) {
				case 0: go(&self().left()); break;
				case 1: go(&self().right()); break;
				case 2: go(&self().up()); break;
				case 3: go(&self().down()); break;

				}
			}
			reread++;
			this_thread::sleep_for(chrono::milliseconds(Speed()));
	//		Sleep(Speed());
		}
		
	}
};
