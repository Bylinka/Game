#pragma once
class CerateUnits {
	Player *player;
	ICeil** randPos() {
		ICeil** r = NULL;
		do
			r = &player->begin()[rand() % (matrix_size.X*matrix_size.Y)];
		while ((*r)->getType() != Type::empty);
		return r;
	}
public:
	CerateUnits(Player *player)
		: player(player)
	{

	}
	CerateUnits hunter(size_t count) {
		for (; count > 0; count--)new Hunter(randPos(), player);
		return *this;
	}
	CerateUnits guard(size_t count) {
		for (; count > 0; count--)new Guard(randPos(), player);
		return *this;
	}
	CerateUnits psycho(size_t count) {
		for (; count > 0; count--)new Psycho(randPos(), player);
		return *this;
	}
};