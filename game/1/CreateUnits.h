#pragma once
class CerateUnits {
	Player& player;
	SafeCeil& randPos() {
		SafeCeil* r = NULL;
		auto length = player.self().count.X * player.self().count.Y;
		do
			r = &player.self()(rand() % length);
		while (r->type() != Type::empty);
		return *r;
	}
public:
	CerateUnits(Player *player)
		: player(*player)
	{
	}
	CerateUnits& hunter(size_t count) {
		vector<Hunter*> gg;
		for (; count > 0; count--) {
			auto &r = randPos();
			auto *g = new Hunter(&player);
			g->Speed() = (rand() % 200) + 200;
			r.reset(g);
			gg.push_back(g);
		}
		for (auto*g : gg) {
			g->self().lock();
			g->start();
			g->self().unlock();
		}
	return *this;
	}
	CerateUnits& guard(size_t count) {
		vector<Guard*> gg;
		for (; count > 0; count--) {
			auto &r = randPos();
			auto *g = new Guard();
			g->Speed() = (rand() % 200) + 200;
			r.reset(g);
			gg.push_back(g);
		}
		for (auto*g : gg) {
			g->self().lock();
			g->start();
			g->self().unlock();
		}
		return *this;
	}
	CerateUnits& psycho(size_t count) {
		vector<Psycho*> gg;
		for (; count > 0; count--) {
			auto &r = randPos();
			auto *g = new Psycho();
			g->Speed() = (rand() % 200) + 200;
			r.reset(g);
			gg.push_back(g);
		}
		for (auto*g : gg) {
			g->self().lock();
			g->start();
			g->self().unlock();
		}
		return *this;
	}
};