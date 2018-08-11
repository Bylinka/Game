#pragma once
COORD matrix_enter, matrix_exit;

class Matrix {
	ICeil** _matrix;
	//COORD _size;
	//COORD _enter;
	//COORD _exit;
	ICeil** fill(SHORT width, SHORT height) {
		ICeil** m = (ICeil**)malloc(sizeof(ICeil*) + sizeof(ICeil*) * width * height);
		new Empty(m, { -1,-1 });
		m++;
		for(register auto i = 0; i<width*height;i++)
			new Empty(m + i, {(SHORT) i % width,(SHORT) i / width });

		return m;
	}
public:
	//inline size_t count() { return static_cast<size_t>(_size.X * _size.Y); }
	//inline COORD& size() { return _size; }
	//inline COORD& enter() { return _enter; }
	//inline COORD& exit() { return _exit; }
	inline ICeil*& test(ICeil** pos) {
		return pos >= _matrix
			&& pos < _matrix + matrix_size.X * matrix_size.Y
			? *pos : _matrix[-1];
	}

	inline ICeil** start() { return _matrix; }
	inline ICeil*& at(COORD i) { return test(_matrix + i.Y * matrix_size.X + i.X); }
	inline ICeil*& at(SHORT x, SHORT y) { return test(_matrix + y * matrix_size.X + x); }

	Matrix(COORD size)
		: _matrix(fill(size.X, size.Y))
		//, _size(size)
		//, _enter({ 0,0 })
		//, _exit({ 0,0 })
	{
		create();
	}
	Matrix(SHORT width, SHORT height)
		: _matrix(fill(width, height))
		//, _size({ width,height })
		//, _enter({ 0,0 })
		//, _exit({ 0,0 })
	{
		create();
	}
	~Matrix() {
		for(register size_t i=-1;i<matrix_size.X * matrix_size.Y;i++){
			if (_matrix[i]) {
				delete _matrix[i];
				_matrix[i] = NULL;
			}
		}
		free(_matrix-1);
	}
	void create() {
		for (register SHORT i = 0; i < matrix_size.X; i++) {
			new Wall(&at(i, 0));
		}

		SHORT*a = new SHORT[matrix_size.X / 2];
		SHORT i = 0;
		generate(a, a + matrix_size.X / 2, [&]() {return i++; });
		last_line(matrix_size.Y - 2, a);
		delete[]a;


		rand_exit();
	}
	void rand_exit() {
		auto e = rand() % 2;

		if (rand() % 2) {
			make_exit((rand() % (matrix_size.X / 2 - 4) + 2) * 2 + 1, 0, e);
			make_exit(matrix_size.X - (((matrix_size.X / 2 - 4) + 2) * 2), matrix_size.Y - 1, !e);
		}
		else {
			make_exit(0, (rand() % (matrix_size.Y / 2 - 4) + 2) * 2 + 1, e);
			make_exit(matrix_size.X - 1, matrix_size.Y - (((matrix_size.Y / 2 - 4) + 2) * 2), !e);
		}
	}
	void make_exit(SHORT x, SHORT y, bool enter) {
		new Empty(&at(x, y));
		if (enter)matrix_enter = { x,y };
		else matrix_exit = { x,y };
	}
	SHORT uniqu(vector<SHORT>& v) {
		SHORT r = 0;
		while (find(r++, v));
		v.push_back(--r);
		return r;
	}
	SHORT uniqu(vector<SHORT>& v, SHORT r) {
		while (find(r--, v))if (!r)break;
		v.push_back(--r);
		return r;
	}
	bool find(SHORT i, vector<SHORT> v) {
		for (auto k : v)
			if (i == k)return true;
		return false;
	}
	void last_line(SHORT y, SHORT* a) {
		vertical_line(y, a);
		for (register SHORT i = 0; i < matrix_size.X; i++)
			new Wall(&at(i, matrix_size.Y - 1));
		
		for (SHORT x = matrix_size.X / 2 - 1; x > 0; x--) {
			conners(y - 1, x);
			if (x > 0) {
				if (at(x * 2, y)->getType() == Type::wall) {
					auto w = new Way(&at(x * 2, y)->right());
					auto v = w->find(
						at(x * 2, y)->left()
					);
					delete w;
					if (v.size() == 2)
						new Empty(&at(x * 2, y));
				}
			}
		}
	}
	void vertical_line(SHORT y, SHORT* a) {
		if (y > 1)horisontal_line(y - 1, a);
		auto length = matrix_size.X / 2;
		for (SHORT x = length - 1; x >= 0; x--) {
			if (x > 0 && a[x] == a[x - 1])new Wall(&at(x * 2, y));
			else if (rand() % 2)new Wall(&at(x * 2, y));
			else new Empty(&at(x * 2, y));
			new Empty(&at(x * 2, y)->right());
		}
		new Wall(&at(matrix_size.X - 1, y));
		new Wall(&at(0, y));

		vector<SHORT> v;
		SHORT index = -1;
		SHORT wall = 0;
		for (SHORT x = 1; x <= length; x++) {
			auto &c = at(x * 2, y);
			if (Type::wall != c->up()->left()->getType()) {
				if (!find(a[x - 1], v))
					v.push_back(a[x - 1]);
				index = x - 1;
			}
			if (Type::wall == c->getType()) {
				if (wall <= index
					&& index < x)
					for (SHORT k = wall; k < x; k++)
						a[k] = a[index];
				wall = x;
			}
		}
		index = -1;
		wall = 0;
		for (SHORT x = 1; x <= length; x++) {
			auto c = at(x * 2, y);
			if (Type::wall != c->up()->left()->getType()) {
				index = x - 1;
			}
			if (Type::wall == c->getType()) {
				if (wall <= index
					&& index < x);
				else {
					a[wall] = uniqu(v);
					for (SHORT k = wall; k < x; k++)
						a[k] = a[wall];
				}
				wall = x;
			}


		}

	}
	void horisontal_line(SHORT y, SHORT* a) {
		vertical_line(y - 1, a);
		SHORT length = matrix_size.X / 2;

		map<SHORT, pair<vector<SHORT>, SHORT[2]>> ii;
		for (SHORT x = 0; x < length; x++) {
			if (ii[a[x]].first.size()) {
				ii[a[x]].first.push_back(x);
				ii[a[x]].second[0] = ii[a[x]].first[rand() % ii[a[x]].first.size()];
				ii[a[x]].second[1] = ii[a[x]].first[rand() % ii[a[x]].first.size()];
			}
			else {
				ii[a[x]].first.push_back(x);
				ii[a[x]].second[0] = x;
				ii[a[x]].second[1] = x;
			}
		}
		for (SHORT x = length - 1; x >= 0; x--) {

			conners(y - 2, x);
			if (ii[a[x]].second[0] == x || ii[a[x]].second[1] == x) {
				new Empty(&at(x * 2, y));
				new Empty(&at(x * 2, y)->right());

			}
			else {
				new Wall(&at(x * 2, y));
				new Wall(&at(x * 2, y)->right());
			}
		}
		new Wall(&at(matrix_size.X - 1, y));
		new Wall(&at(0, y));

		vector<SHORT> v;
		for (SHORT x = 0; x < length; x++)
			if (at(x * 2, y)->getType() != Type::wall)
				if (!find(a[x], v))v.push_back(a[x]);
		for (SHORT x = 0; x < length; x++) {
			if (at(x * 2, y)->getType() == Type::wall)
				a[x] = uniqu(v);
		}
	}

	void conners(SHORT y, SHORT x) {
		auto &j = at(x * 2, y);
		if (j->left()->getType() == Type::wall 
			|| j->right()->getType() == Type::wall
			|| j->up()->getType() == Type::wall
			|| j->down()->getType() == Type::wall) {
			new Wall(&at(x * 2, y));

		}
		else {
			new Empty(&at(x * 2, y));
		}
	}
};

//class Creator {
//	shared_ptr<Matrix> m_matrix;
//
//
//public:
//	Creator(COORD size)
//		:m_matrix(new Matrix())
//	{
//		matrix_size = size;
//		for (register int i = 0; i<m_matrix->length(); i++) {
//			auto e = matrix().add(new Empty());
//			e->position() = { (SHORT)i % size.X,(SHORT)i / size.X };
//			e->matrix(&matrix());
//		}
//
//	}
//	Matrix& matrix() {
//		return *m_matrix.get();
//	}
//	void create() {
//		for (register SHORT i = 0; i < matrix_size.X; i++) {
//			matrix()[{i, 0}]->change(new Wall());
//		}
//
//		SHORT*a = new SHORT[matrix_size.X / 2];
//		SHORT i = 0;
//		generate(a, a + matrix_size.X / 2, [&]() {return i++; });
//		last_line(matrix_size.Y - 2, a);
//		delete[]a;
//
//
//		rand_exit();
//	}
//	void rand_exit() {
//		auto e = rand() % 2;
//
//		if (rand() % 2) {
//			make_exit((rand() % (matrix_size.X / 2 - 4) + 2) * 2 + 1, 0, e);
//			make_exit(matrix_size.X - (((matrix_size.X / 2 - 4) + 2) * 2), matrix_size.Y - 1, !e);
//		}
//		else {
//			make_exit(0, (rand() % (matrix_size.Y / 2 - 4) + 2) * 2 + 1, e);
//			make_exit(matrix_size.X - 1, matrix_size.Y - (((matrix_size.Y / 2 - 4) + 2) * 2), !e);
//		}
//	}
//	void make_exit(SHORT x, SHORT y, bool enter) {
//		matrix[{x, y}]->change(new Empty());
//		if (enter)m_matrix->enter = { x,y };
//		else m_matrix->exit = { x,y };
//	}
//	SHORT uniqu(vector<SHORT>& v) {
//		SHORT r = 0;
//		while (find(r++, v));
//		v.push_back(--r);
//		return r;
//	}
//	SHORT uniqu(vector<SHORT>& v, SHORT r) {
//		while (find(r--, v))if (!r)break;
//		v.push_back(--r);
//		return r;
//	}
//	bool find(SHORT i, vector<SHORT> v) {
//		for (auto k : v)
//			if (i == k)return true;
//		return false;
//	}
//	void last_line(SHORT y, SHORT* a) {
//		vertical_line(y, a);
//		for (register SHORT i = 0; i < matrix_size.X; i++)
//			matrix[{i, matrix_size.Y - 1}]->change(new Wall());
//		Way* w = new Way(shared_ptr<ICeil>(matrix[0]));
//		for (SHORT x = matrix_size.X / 2 - 1; x > 0; x--) {
//			conners(y - 1, x);
//			if (x > 0){
//				auto c = matrix[{x * 2, y}];
//				if (c->getType() == Type::wall) {
//					auto v = w->find(c->right()->position(),c->left()->position());
//					if (v.size()==2)c->change(new Empty());
//				}
//			}
//		}
//	}
//	void vertical_line(SHORT y, SHORT* a) {
//		if (y > 1)horisontal_line(y - 1, a);
//		auto length = matrix_size.X / 2;
//		for (SHORT x = length - 1; x >= 0; x--) {
//			auto c = matrix[{x * 2, y}];
//			if (x > 0 && a[x] == a[x - 1])c->change(new Wall());
//			else if (rand() % 2)c->change(new Wall());
//			else c->change(new Empty());
//			c->right()->change(new Empty());
//		}
//		matrix[{matrix_size.X - 1, y}]->change(new Wall());
//		matrix[{0, y}]->change(new Wall());
//
//		vector<SHORT> v;
//		SHORT index = -1;
//		SHORT wall = 0;
//		for (SHORT x = 1; x <= length; x++) {
//			auto c = matrix[{x * 2, y}];
//			if (Type::wall != c->up()->left()->getType()) {
//				if (!find(a[x - 1], v))
//					v.push_back(a[x - 1]);
//				index = x - 1;
//			}
//			if (Type::wall == c->getType()) {
//				if (wall <= index
//					&& index < x)
//					for (SHORT k = wall; k < x; k++)
//						a[k] = a[index];
//				wall = x;
//			}
//		}
//		index = -1;
//		wall = 0;
//		for (SHORT x = 1; x <= length; x++) {
//			auto c = matrix[{x * 2, y}];
//			if (Type::wall != c->up()->left()->getType()) {
//				index = x - 1;
//			}
//			if (Type::wall == c->getType()) {
//				if (wall <= index
//					&& index < x);
//				else {
//					a[wall] = uniqu(v);
//					for (SHORT k = wall; k < x; k++)
//						a[k] = a[wall];
//				}
//				wall = x;
//			}
//
//
//		}
//
//	}
//	void horisontal_line(SHORT y, SHORT* a) {
//		vertical_line(y - 1, a);
//		SHORT length = matrix_size.X / 2;
//
//		map<SHORT, pair<vector<SHORT>, SHORT[2]>> ii;
//		for (SHORT x = 0; x < length; x++) {
//			if (ii[a[x]].first.size()) {
//				ii[a[x]].first.push_back(x);
//				ii[a[x]].second[0] = ii[a[x]].first[rand() % ii[a[x]].first.size()];
//				ii[a[x]].second[1] = ii[a[x]].first[rand() % ii[a[x]].first.size()];
//			}
//			else {
//				ii[a[x]].first.push_back(x);
//				ii[a[x]].second[0] = x;
//				ii[a[x]].second[1] = x;
//			}
//		}
//		for (SHORT x = length - 1; x >= 0; x--) {
//			
//			conners(y - 2, x);
//			auto c = matrix[{x * 2, y}];;
//			if (ii[a[x]].second[0] == x || ii[a[x]].second[1] == x) {
//				c->change(new Empty());
//				c->right()->change(new Empty());
//
//			}
//			else {
//				c->change(new Wall());
//				c->right()->change(new Wall());
//
//			}
//		}
//		matrix[{matrix_size.X - 1, y}]->change(new Wall());
//		matrix[{0, y}]->change(new Wall());
//
//		vector<SHORT> v;
//		for (SHORT x = 0; x < length; x++)
//			if (matrix[{x * 2, y}]->getType() != Type::wall)
//				if (!find(a[x], v))v.push_back(a[x]);
//		for (SHORT x = 0; x < length; x++) {
//			if (matrix[{x * 2, y}]->getType() == Type::wall)
//				a[x] = uniqu(v);
//		}
//	}
//
//	void conners(SHORT y, SHORT x) {
//		auto j = matrix[{x * 2, y}];
//		if ((j->left() ? j->left()->getType() == Type::wall : false)
//			|| (j->right() ? j->right()->getType() == Type::wall : false)
//			|| (j->up() ? j->up()->getType() == Type::wall : false)
//			|| (j->down() ? j->down()->getType() == Type::wall : false)) {
//			j->change(new Wall());
//
//		}
//		else {
//			j->change(new Empty());
//		}
//	}
//
//	Type getType() { return Type::empty; }
//
//};