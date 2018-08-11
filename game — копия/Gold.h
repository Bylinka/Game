#pragma once


class Gold : public  ICeil {

public:
	Gold(ICeil** self, bool set=false ) :ICeil(self, set) {}
	void fill(int count) {
		for (SHORT i = 0; i < count; i++) {
			ICeil** t;
			do {
				t = &matrix()[rand() % (matrix_size.X * matrix_size.Y)];
			} while ((*t)->getType() != Type::empty);
			new Gold(t,true);
		}
	}
	void action(ICeil* to) {
		if (to->getType() == Type::player)
			table.coin++;
		else if (to->getType() == Type::hunter
			|| to->getType() == Type::guard
			|| to->getType() == Type::psycho)
			table.enemyCoin++;
		if (table.enemyCoin > table.totalCoin / 2)Luse();
	}
	Type getType() { return Type::gold; }
	~Gold() {

	}
};