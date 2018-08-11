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
			auto step = _matrix[i->Y*matrix_size.X + i->X];
			for (int n = 0; n < 4; n++) {
				SHORT x = i->X + move[n].X;
				SHORT y = i->Y + move[n].Y;
				if (y >= matrix_size.Y
					|| x >= matrix_size.X
					|| x < 0 || y < 0)continue;
				if (x >= 0 
					&& x  <matrix_size.X
					&& y >= 0 
					&& y < matrix_size.Y)
					if (_matrix[y*matrix_size.X + x] == 0) {
						_matrix[y*matrix_size.X + x] = step + 1;
						next.push_back({ x,y });
					}
			}
		}
		todo = next;
		if (todo.size())test(_matrix, todo);
	}
	void temp(COORD i, DWORD time) {

		Way w(&begin()[i.X + i.Y*matrix_size.X]);
		w.set(time);

	}
	void set(DWORD time) {
		old = matrix()[0];
		matrix()[0] = this;
		Sleep(time);
		matrix()[0] = old;
	}
	SHORT* toSimple() {
		SHORT*m = (SHORT*)malloc((matrix_size.X*matrix_size.Y+1) * sizeof(SHORT));
		for (size_t i = 0; i < matrix_size.X*matrix_size.Y; i++) {
			m[i] = (Type::wall == begin()[i]->getType() ? -1 : 0);
		}
		return m;
	}
public:
	Way(ICeil**self) 
		: ICeil(self,false)
		, old(NULL)
	{
	}
	/*
	показывает путь
	way - путь
	length - длинна отрезка показываемого пути
	time - скорость движения отрезка пути
	*/
	void show(vector<COORD> way,SHORT length=9, DWORD time=40) {
		reverse(way.begin(),way.end());
		for (auto i : way) {
			thread th(&Way::temp, this, i, time*length);
			th.detach();
			Sleep(time);
		}
	}
	vector<COORD> find(ICeil* _end) {
		COORD end = _end->position();
		SHORT* _matrix = toSimple();
		vector<COORD> wave;
		wave.push_back(position());
		test(_matrix, wave);
		wave.clear();
		auto current = end;
		wave.push_back(current);
		if (current.X == matrix_size.X - 1) {
			current.X--;
			wave.push_back(current);
		}

		SHORT nstep = _matrix[current.Y*matrix_size.X+current.X];
		const COORD finish = {
			!position().X ? 1 : position().X == matrix_size.X - 1 ? matrix_size.X - 2 : position().X,
			!position().Y ? 1 : position().Y == matrix_size.Y - 1 ? matrix_size.Y - 2 : position().Y
		};
		while (nstep>1) {
			for (int i = 0; i < 4; i++) {
				COORD next = { current.X + move[i].X,current.Y + move[i].Y };
				if (next.X < matrix_size.X
					&& next.X >= 0
					&& next.Y < matrix_size.Y
					&& next.Y >= 0)
					if ((_matrix[current.Y * matrix_size.X + current.X] - 1
						== _matrix[next.Y * matrix_size.X + next.X])
						|| (next.Y == finish.Y&&next.X == finish.X)) {
						wave.push_back(next);
						current = next;
						break;
					}
			}
			nstep = _matrix[current.Y * matrix_size.X + current.X];
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
	~Way() {
//		if (old)delete old;
		old = NULL;
//		MessageBox(0, L"release Way", L" ", MB_OK);
	}
};