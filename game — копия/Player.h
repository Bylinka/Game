#pragma once
class Player : public IUnit {
public:
	Type getType() { return Type::player; }
	Player(ICeil** self)
		: IUnit(self, this)
	{
		start();
	}
	//void go(shared_ptr<ICeil> to) {
	//	
	//	if (to) {
	//		if (to->getType() == Type::gold)((Gold*)(to.get()))->action(this);
	//		//if (to->getType() == Type::hunter)((Hunter*)to)->action(this);
	//		//if (to->getType() == Type::guard)((Guard*)to)->action(this);
	//		//if (to->getType() == Type::psycho)((Psycho*)to)->action(this);
	//		if (to->getType() != Type::wall) {
	//			COORD t = position();
	//			auto &m = matrix();
	//			to->change(this);
	//			m[position()].reset(new Empty());
	//			m[position()]->position() = t;
	//			m[position()]->matrix(&m);
	//		}
	//	}

	//}
	void move() {
		int i = 60;

		while (isAlive()) {
			if (position().X == matrix_exit.X
				&&position().Y == matrix_exit.Y)Win();
			if (GetAsyncKeyState(VK_LEFT)) {
				go(left());
				i = i > 20 ? i -= 5 : 20;
			}
			else if (GetAsyncKeyState(VK_RIGHT)) {
				go(right());
				i = i > 20 ? i -= 5 : 20;
			}
			else if (GetAsyncKeyState(VK_UP)) {
				go(up());
				i = i > 20 ? i -= 5 : 20;
			}
			else if (GetAsyncKeyState(VK_DOWN)) {
				go(down());
				i = i > 20 ? i -= 5 : 20;
			}
			else i = 60;
			Sleep(i);
			
		}
	}

};