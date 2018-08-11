#pragma once

typedef enum {
	empty,
	wall,
	gold,
	psycho,
	guard,
	hunter,
	player,
	player2,
	way
}Type;

struct Coord {
	SHORT X, Y;
	bool operator < (Coord a) const {
		return Y == a.Y ? X < a.X : Y < a.Y;
	}
	Coord(SHORT X, SHORT Y)
		: X(X)
		, Y(Y)
	{
	}
	Coord(COORD c)
		: X(c.X)
		, Y(c.Y)
	{
	}
	Coord operator=(COORD c) {
		X = c.X;
		Y = c.Y;
		return *this;
	}
	operator COORD () {
		return { X,Y };
	}
};
class ICeil {
	friend class SafeCeil;
	SafeCeil* object;
public:
	inline COORD position();
	inline SafeCeil& self(SafeCeil* object=NULL);
	virtual ~ICeil() {}
	virtual Type getType() = 0;
	void remove();
};
class SafeCeil {
	friend class CeilArray;
	std::mutex locker;
	SafeCeil(CeilArray* array);

public:
	CeilArray& array;
	ICeil* object;
	COORD& const count;
	inline bool trylock() {
		return locker.try_lock();
	}
	inline void lock() {
		locker.lock();
	}
	inline void unlock() {
		locker.unlock();
	}
	inline COORD position();
	inline COORD position(CeilArray& start);
	inline SafeCeil& begin();
	inline SafeCeil& end();
	inline SafeCeil& operator()(SHORT x, SHORT y);
	inline SafeCeil& operator()(COORD pos);
	inline SafeCeil& operator()(size_t pos);
	inline SafeCeil& left();
	inline SafeCeil& right();
	inline SafeCeil& up();
	inline SafeCeil& down();
	void reset(ICeil* o, bool lock = true);
	void conect(ICeil* o, bool lock = true);
	void move(ICeil* o, bool lock = true);
	Type type();
};

struct CeilArray {
	SHORT helth;
	SHORT mana;
	SHORT coin;
	SHORT totalCoin;
	SHORT enemyCoin;
	time_t time;
	map<Coord,Type>* _redraw;
	mutex redraw_m;
	void redraw(COORD c, Type t) {
		redraw_m.lock();
		(*_redraw)[c] = t;
		redraw_m.unlock();
	}
	SafeCeil* array;
	COORD count;
	SafeCeil* enter;
	SafeCeil* exit;
	inline size_t length() {
		return count.X * count.Y;
	}
	inline SafeCeil* end() {
		return array + count.X * count.Y;
	}
	CeilArray(COORD count);
	virtual ~CeilArray();
	SafeCeil* operator()(SHORT x, SHORT y);
	inline SafeCeil* operator()(size_t pos);
	void create();
private:
	void rand_exit();
	void make_exit(SHORT x, SHORT y, bool enter);
	SHORT uniqu(vector<SHORT>& v);
	SHORT uniqu(vector<SHORT>& v, SHORT r);
	bool find(SHORT i, vector<SHORT> v);
	void last_line(SHORT y, SHORT* a);
	void vertical_line(SHORT y, SHORT* a);
	void horisontal_line(SHORT y, SHORT* a);
	void conners(SHORT y, SHORT x);
};
class Empty : public ICeil {
public:
	Type getType() { return Type::empty; }
};
class Wall : public  ICeil {
public:
	Type getType() { return Type::wall; }
};