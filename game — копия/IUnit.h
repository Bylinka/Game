#pragma once
class IUnit : public ICeil{
	thread* self;
	atomic<bool> alive;
	int speed;
	IUnit* _player;
//	shared_ptr<vector<IUnit*>> unit;
public:
	int&Speed() { return speed; }
	//shared_ptr<vector<IUnit*>> Unit(shared_ptr<vector<IUnit*>> unit = NULL) {
	//	if (unit)this->unit = unit;
	//	return this->unit;
	//}
	bool canMove(ICeil* next) {
		if (!next)return false;
		return next->getType() != Type::wall
			&& next->getType() != Type::guard
			&& next->getType() != Type::hunter
			&& next->getType() != Type::psycho;
	}
	void go(ICeil*& to) {
		if(&to==begin())return;
		if (!to)return;
		if (to->getType() == Type::gold) {
			auto g = new Gold(&to);
			g->action(this);
			delete g;
		}
		if (canMove(to)) {
			if (to->getType() == Type::player && getType() == !Type::player) {
				table.helth--;
				stop();
				new Empty(matrix(), position());
//				dynamic_cast<IUnit*>(to)->remove(this);
				Sleep(speed * 3);
				delete this;
			}
			else {
				auto t = to;
				new Empty(matrix(), position());
				position() = to->position();
				matrix(to->matrix());
				*matrix() = this;	
				delete t;
			}
		}
	}
	IUnit*& player() {
		return _player;
	}
	void remove(ICeil* what) {
		thread t([&]() {
			Sleep(speed * 3);
			delete what;
		});
		t.detach();
	}
	IUnit(ICeil** self, IUnit* player = NULL)
		: ICeil(self)
		, self(NULL)
		, alive(false)
		, speed(250)
		, _player(player)
	{
	}

	atomic<bool>& isAlive() { return alive; }
	thread* Self() {
		return self;
	}

	virtual void move() = 0;
	void start() {
		isAlive() = true;
		self = new thread(&IUnit::move, this);
	}
	void stop() {
		alive = false;
		if(self->joinable())self->join();
		delete self;
		self = NULL;

	}
	virtual ~IUnit(){
		if (self) {
			stop();
		}
	}
};

