#pragma once
class Hunter
	: public IUnit
{

	int reread;
public:
	Type getType() { return Type::hunter; }
	Hunter(ICeil**self, IUnit* player)
		:IUnit(self, player)
		, reread(0)
	{
		start();
	}
	void move() {
		vector<COORD> way;
		

		
		while (isAlive()) {
			if (reread == 25) {
				Way *w = new Way(player()->matrix());
				way = w->find(this);
				delete w;
				reread = 0;
			}
			if (way.size()) {
				COORD c = way[0];
				way.erase(way.begin());
				go((*this)[{c.X, c.Y}]);
			}
			else {
				switch (rand() % 4) {
				case 0: go(left()); break;
				case 1: go(right()); break;
				case 2: go(up()); break;
				case 3: go(down()); break;

				}
			}
			reread++;
			Sleep(Speed());
		}
		
	}
};
