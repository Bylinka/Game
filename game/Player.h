#pragma once
class Player : public IUnit {
public:
	Type getType() { return Type::player; }
	Player()
		: IUnit()
	{
	}
	void move() {
		int i = 60;

		while (isAlive()) {
			if (position().X == self().array.exit->position().X
				&&position().Y == self().array.exit->position().Y)Win();
			if (GetAsyncKeyState(VK_LEFT)) {
				go(&self().left());
				i = i > 20 ? i -= 5 : 20;
			}
			else if (GetAsyncKeyState(VK_RIGHT)) {
				go(&self().right());
				i = i > 20 ? i -= 5 : 20;
			}
			else if (GetAsyncKeyState(VK_UP)) {
				go(&self().up());
				i = i > 20 ? i -= 5 : 20;
			}
			else if (GetAsyncKeyState(VK_DOWN)) {
				go(&self().down());
				i = i > 20 ? i -= 5 : 20;
			}
			else i = 60;
			this_thread::sleep_for(chrono::milliseconds(i));

//			Sleep(i);
			
		}
	}

};