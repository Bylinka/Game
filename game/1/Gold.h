#pragma once


class Gold : public  ICeil {

public:
	Gold() :ICeil() {
	}
	Gold(SafeCeil* self) :ICeil() {
		this->self(self);
	}
	void fill(int count) {
		for (SHORT i = 0; i < count; i++) {
			size_t t;
			do {
				t = rand() % (self().count.X * self().count.Y);
			} while (self()(t).type() != Type::empty);
			self()(t).reset(new Gold());
		}
	}
	void action(ICeil* to) {
		if (to->getType() == Type::player)
			self().array.coin++;
		else if (to->getType() == Type::hunter
			|| to->getType() == Type::guard
			|| to->getType() == Type::psycho)
			self().array.enemyCoin++;
		if (self().array.enemyCoin > self().array.totalCoin / 2)Luse();
	}
	Type getType() { return Type::gold; }

};