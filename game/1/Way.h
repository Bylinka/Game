#pragma once
class Way : public ICeil {
private:
	ICeil* old;
	const COORD move[4] = {
		{ -1, 0 },
		{ 0, 1 },
		{ 1, 0 },
		{ 0, -1 }
	};
	void test(SHORT* _matrix, vector<COORD>& todo) {

		vector<COORD> next;
		for (auto i = todo.begin(); i != todo.end(); i++) {
			auto step = _matrix[i->Y*self().count.X + i->X];
			for (int n = 0; n < 4; n++) {
				SHORT x = i->X + move[n].X;
				SHORT y = i->Y + move[n].Y;
				if (y >= self().count.Y
					|| x >= self().count.X
					|| x < 0 || y < 0)continue;
				if (x >= 0 
					&& x  <self().count.X
					&& y >= 0 
					&& y < self().count.Y)
					if (_matrix[y*self().count.X + x] == 0) {
						_matrix[y*self().count.X + x] = step + 1;
						next.push_back({ x,y });
					}
			}
		}
		todo = next;
		if (todo.size())test(_matrix, todo);
	}
	void temp(COORD i, DWORD time) {
		Way w(&self()(i.X, i.Y));
		w.set(time);
	}
	void set(DWORD time) {
		Sleep(time);
	}
	SHORT* toSimple() {
		SHORT*m = (SHORT*)malloc((self().count.X*self().count.Y+1) * sizeof(SHORT));
		for (register size_t i = 0; i < self().count.X*self().count.Y; i++) {
			m[i] = (Type::wall == self()(i).type() ? -1 : 0);
		}
		return m;
	}
public:
	Way(SafeCeil*self)
		: ICeil()
		, old(self->object)
	{
		this->self(self);
	}
	/*
	показывает путь
	way - путь
	length - длинна отрезка показываемого пути
	time - скорость движения отрезка пути
	*/
	void hide(vector<SafeCeil*>* way, SHORT length, DWORD time, bool* finish) {
		Sleep(time*length);
		for (register int k = 0; k < way->size(); k++) {
			SafeCeil*i = (*way)[k];
			i->lock();
			Way*w = dynamic_cast<Way*>(i->object);
			if (w->old)i->object = w->old;
			i->array.redraw(i->position(), w->old->getType());
			if(w!=this)delete w;
			i->unlock();
			Sleep(time);
		}
		*finish = true;
	}
	void show(vector<COORD> way,SHORT length=9, DWORD time=40) {
		reverse(way.begin(),way.end());
		vector<SafeCeil*>* wh = new vector<SafeCeil*>();
		bool finish=false;
		thread th(&Way::hide, this, wh, length, time, &finish);

		for (auto i : way) {
			auto &o = self()(i);
			o.lock();
			Way*w = new Way(&o);
			o.move(w, 0);
			o.array.redraw(o.position(), w->getType());


			wh->push_back(&o);
			o.unlock();
			Sleep(time);
		}
		while (!finish)Sleep(time);
		th.join();
		delete wh;
	}
	vector<COORD> find(SafeCeil* _end) {
		COORD end = _end->position();
		SHORT* _matrix = toSimple();
		vector<COORD> wave;
		wave.push_back(position());
		test(_matrix, wave);
		wave.clear();
		auto current = end;
		wave.push_back(current);
		if (current.X == self().count.X - 1) {
			current.X--;
			wave.push_back(current);
		}

		SHORT nstep = _matrix[current.Y*self().count.X+current.X];
		const COORD finish = {
			!position().X ? 1 : position().X == self().count.X - 1 ? self().count.X - 2 : position().X,
			!position().Y ? 1 : position().Y == self().count.Y - 1 ? self().count.Y - 2 : position().Y
		};
		while (nstep>1) {
			for (int i = 0; i < 4; i++) {
				COORD next = { current.X + move[i].X,current.Y + move[i].Y };
				if (next.X < self().count.X
					&& next.X >= 0
					&& next.Y < self().count.Y
					&& next.Y >= 0)
					if ((_matrix[current.Y * self().count.X + current.X] - 1
						== _matrix[next.Y * self().count.X + next.X])
						|| (next.Y == finish.Y&&next.X == finish.X)) {
						wave.push_back(next);
						current = next;
						break;
					}
			}
			nstep = _matrix[current.Y * self().count.X + current.X];
		}
		if (wave.size()) {
			if (wave.rbegin()->X != position().X || wave.rbegin()->Y != position().Y)
				wave.push_back(position());

			if (wave.begin()->X != end.X || wave.begin()->Y != end.Y)
				wave.insert(wave.begin(), end);
		}
		free(_matrix);
		return wave;
	}
	Type getType() { 
		return Type::way; 
	}
};