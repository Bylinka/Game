#pragma once
class IUnit : public ICeil{
	thread* t_self;
	atomic<bool> alive;
	int speed;
//	shared_ptr<vector<IUnit*>> unit;
public:
//	SafeCeil* player;
	int&Speed() { return speed; }
	//shared_ptr<vector<IUnit*>> Unit(shared_ptr<vector<IUnit*>> unit = NULL) {
	//	if (unit)this->unit = unit;
	//	return this->unit;
	//}
	bool canMove(SafeCeil* next) {
		return next->object->getType() != Type::wall
			&& next->object->getType() != Type::guard
			&& next->object->getType() != Type::hunter
			&& next->object->getType() != Type::psycho;
	}
	void go(SafeCeil* to) {
		if (&self() == to)return;
		while (!self().trylock());
		auto *s = &self();
		while (!to->trylock());
		if (to->object->getType() == Type::gold) {
			Gold(to).action(this);
		}
		if (canMove(to)) {
			if (to->object->getType() == Type::player) {
				self().array.helth--;
				self().move(new Empty(),0);
				to->unlock();
				s->unlock();
				isAlive() = false;
				remove();
			}
			else {
				to->reset(this,0);
				s->move(new Empty(),0);
				to->unlock();
				s->unlock();
			}
		}else if ((to->object->getType() == Type::hunter
			|| to->object->getType() == Type::guard
			|| to->object->getType() == Type::psycho)
			&& getType() == Type::player) {
			self().array.helth--;
			delete to->object;
			to->move(this, 0);
			s->move(new Empty(), 0);
			to->unlock();
			s->unlock();
		}
		else {
			to->unlock();
			s->unlock();
		}
	}

	IUnit()
		: ICeil()
		, t_self(NULL)
		, alive(false)
		, speed(250)
//		, player(NULL)
	{
	}

	atomic<bool>& isAlive() { return alive; }

	virtual void move() = 0;
	void start() {
		isAlive() = true;
		t_self = new thread(&IUnit::move, this);
	}
	void stop() {
		alive = false;
		if(t_self->joinable())t_self->join();
		delete t_self;
		t_self = NULL;

	}
	virtual ~IUnit(){
		if (t_self) {
			stop();
		}
	}
};

