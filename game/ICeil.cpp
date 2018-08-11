#include <windows.h>
#include <mutex>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>
#include <atomic>
#pragma comment(lib, "user32")
#pragma comment(lib, "kernel32")
#define _XP
using namespace std;
#include "pgraph.hpp"
#include "iceil.h"
#include "way.h"
#include "gold.h"
#include "iunit.h"
#include "player.h"
#include "guard.h"
#include "psycho.h"
#include "hunter.h"
#include "createunits.h"
#include "draw.h"


inline COORD ICeil::position() { return object->position(); }
inline SafeCeil& ICeil::self(SafeCeil* object) { 
	if (object)this->object = object;
	return *this->object;
}

SafeCeil::SafeCeil(CeilArray* array)
	: object(NULL)
	, array(*array)
	, count(array->count)
	, locker()
{}
inline COORD SafeCeil::position() {
	register size_t l = (size_t)(this - array.array);
	return { (SHORT)(l % array.count.X), (SHORT)(l / (size_t)array.count.X) };
}
inline COORD SafeCeil::position(CeilArray& start) {
	register size_t l = (size_t)(this - start.array);
	return { (SHORT)(l % start.count.X), (SHORT)(l / (size_t)start.count.X) };
}
inline SafeCeil& SafeCeil::begin() {
	return *array.array;
}
inline SafeCeil& SafeCeil::end() {
	return *array.end();
}

inline SafeCeil& SafeCeil::operator()(SHORT x, SHORT y) {
	return *array(x, y);
}
inline SafeCeil& SafeCeil::operator()(COORD pos) {
	return *array(pos.X, pos.Y);
}
inline SafeCeil& SafeCeil::operator()(size_t pos) {
	return array.array[pos];
}
inline SafeCeil& SafeCeil::left() {
	COORD p = position();
	return position().X ? operator()(--p.X, p.Y) : *this;
}
inline SafeCeil& SafeCeil::right() {
	COORD p = position();
	return position().X < count.X - 1 ? operator()(++p.X, p.Y) : *this;
}
inline SafeCeil& SafeCeil::up() {
	COORD p = position();
	return position().Y ? operator()(p.X, --p.Y) : *this;
}
inline SafeCeil& SafeCeil::down() {
	COORD p = position();
	return position().Y < count.Y - 1 ? operator()(p.X, ++p.Y) : *this;
}
void ICeil::remove() {
	auto r = [](ICeil* t) {
		Sleep(1000);
		delete t;
	};
	thread t(r, this);
	t.detach();
}
void SafeCeil::reset(ICeil* o, bool lock) {
	if(lock)this->lock();
	if (object)delete object;
	object = o;
	o->object = this;
	array.redraw(position(), o->getType());
	if (lock)this->unlock();
}
void SafeCeil::move(ICeil* o, bool lock) {
	if (lock)this->lock();
	object = o;
	o->object = this;
	array.redraw(position(), o->getType());
	if (lock)this->unlock();
}
void SafeCeil::conect(ICeil* o, bool lock) {
	if (lock)this->lock();
	o->object = this;
	if (lock)this->unlock();
}
Type SafeCeil::type() {
	lock();
	auto t = object->getType();
	unlock();
	return t;
}


CeilArray::CeilArray(COORD count)
	: count(count)
	, helth(10)
	, mana(10)
	, coin(0)
	, enemyCoin(0)
	, totalCoin(100)
	, time(0)
	, array((SafeCeil*)malloc(count.X*count.Y * sizeof(SafeCeil)))
	, _redraw(new map<Coord,Type>())
{
	SafeCeil s(this);
	for (register auto i = array; i != end(); i++) {
		memcpy(i,&s, sizeof(SafeCeil));
		i->reset(new Empty());
	}
	create();
}
CeilArray::~CeilArray() {
	free(array);
}

SafeCeil* CeilArray::operator()(SHORT x, SHORT y) {
	return array + count.X * y + x;
}
inline SafeCeil* CeilArray::operator()(size_t pos) {
	return array + pos;
}

void CeilArray::create() {
	for (register SHORT i = 0; i < count.X; i++) {
		(*this)(i, 0)->reset(new Wall());
		(*this)(i, count.Y - 1)->reset(new Wall());
	}

	SHORT*a = new SHORT[count.X / 2];
	SHORT i = 0;
	generate(a, a + count.X / 2, [&]() {return i++; });
	last_line(count.Y - 2, a);
	delete[]a;


	rand_exit();
}
void CeilArray::rand_exit() {
	auto e = rand() % 2;

	if (rand() % 2) {
		make_exit((rand() % (count.X / 2 - 4) + 2) * 2 + 1, 0, e);
		make_exit(count.X - (((count.X / 2 - 4) + 2) * 2), count.Y - 1, !e);
	}
	else {
		make_exit(0, (rand() % (count.Y / 2 - 4) + 2) * 2 + 1, e);
		make_exit(count.X - 1, count.Y - (((count.Y / 2 - 4) + 2) * 2), !e);
	}
}
void CeilArray::make_exit(SHORT x, SHORT y, bool enter) {
	(*this)(x, y)->reset(new Empty());
	if (enter)this->enter = (*this)(x, y);
	else exit = (*this)(x, y);
}
SHORT CeilArray::uniqu(vector<SHORT>& v) {
	SHORT r = 0;
	while (find(r++, v));
	v.push_back(--r);
	return r;
}
SHORT CeilArray::uniqu(vector<SHORT>& v, SHORT r) {
	while (find(r--, v))if (!r)break;
	v.push_back(--r);
	return r;
}
bool CeilArray::find(SHORT i, vector<SHORT> v) {
	for (auto k : v)
		if (i == k)return true;
	return false;
}
void CeilArray::last_line(SHORT y, SHORT* a) {
	vertical_line(y, a);
	//for (register SHORT i = 0; i < count.X; i++)
	//	(*this)(i, count.Y - 1)->reset(new Wall());

	for (SHORT x = count.X / 2 - 1; x > 0; x--) {
		conners(y - 1, x);
		if (x > 0) {
			if ((*this)(x * 2, y)->type() == Type::wall) {
				auto w = new Way(&(*this)(x * 2, y)->right());
				auto v = w->find(
					&(*this)(x * 2, y)->left()
				);
				delete w;
				if (v.size() == 2)
					(*this)(x * 2, y)->reset(new Empty());
			}
		}
	}
}
void CeilArray::vertical_line(SHORT y, SHORT* a) {
	if (y > 1)horisontal_line(y - 1, a);
	auto length = count.X / 2;
	for (SHORT x = length - 1; x >= 0; x--) {
		if (x > 0 && a[x] == a[x - 1])(*this)(x * 2, y)->reset(new Wall());
		else if (rand() % 2)(*this)(x * 2, y)->reset(new Wall());
		else (*this)(x * 2, y)->reset(new Empty());
		(*this)(x * 2, y)->right().reset(new Empty());
	}
	(*this)(count.X - 1, y)->reset(new Wall());
	(*this)(0, y)->reset(new Wall());

	vector<SHORT> v;
	SHORT index = -1;
	SHORT wall = 0;
	for (SHORT x = 1; x <= length; x++) {
		auto c = (*this)(x * 2, y);
		if (Type::wall != c->up().left().type()) {
			if (!find(a[x - 1], v))
				v.push_back(a[x - 1]);
			index = x - 1;
		}
		if (Type::wall == c->type()) {
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
		auto c = (*this)(x * 2, y);
		if (Type::wall != c->up().left().type()) {
			index = x - 1;
		}
		if (Type::wall == c->type()) {
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
void CeilArray::horisontal_line(SHORT y, SHORT* a) {
	vertical_line(y - 1, a);
	SHORT length = count.X / 2;

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
			(*this)(x * 2, y)->reset(new Empty());
			(*this)(x * 2, y)->right().reset(new Empty());
		}
		else {
			(*this)(x * 2, y)->reset(new Wall());
			(*this)(x * 2, y)->right().reset(new Wall());
		}
	}
	(*this)(count.X - 1, y)->reset(new Wall());
	(*this)(0, y)->reset(new Wall());

	vector<SHORT> v;
	for (SHORT x = 0; x < length; x++)
		if ((*this)(x * 2, y)->type() != Type::wall)
			if (!find(a[x], v))v.push_back(a[x]);
	for (SHORT x = 0; x < length; x++) {
		if ((*this)(x * 2, y)->type() == Type::wall)
			a[x] = uniqu(v);
	}
}

void CeilArray::conners(SHORT y, SHORT x) {
	auto j = (*this)(x * 2, y);
	if (j->left().type() == Type::wall
		|| j->right().type() == Type::wall
		|| j->up().type() == Type::wall
		|| j->down().type() == Type::wall) {
		(*this)(x * 2, y)->reset(new Wall());
	}
	else {
		(*this)(x * 2, y)->reset(new Empty());
	}
}

Draw::Draw(COORD size)
	: self(nullptr)
	, alive(false)
	, msInterval(30)
{
	m_matrix = new CeilArray(size);
	buffer = (PCHAR_INFO)malloc((size.X * size.Y + size.X) * sizeof(PCHAR_INFO));
	start();
	Gold(matrix().array).fill(100);
	Player* p = new Player();
	m_matrix->enter->reset(p);
	Way w(matrix().enter);
	w.show(w.find(matrix().exit), 120, 12);
	while (m_matrix->enter->type() != Type::player)Sleep(20);
	Sleep(20);
	m_matrix->enter->lock();
	dynamic_cast<Player*>(m_matrix->enter->object)->start();
	m_matrix->enter->unlock();
	CerateUnits(p).guard(24).psycho(15).hunter(9);
}
Draw::~Draw() {
	if (self) {
		alive = false;
		self->join();
		delete self;
	}
	if (buffer)free(buffer);
	delete m_matrix;
}
bool Draw::isAlive() { return alive; }
void Draw::start() {
	alive = true;
	self = new thread(&Draw::tick, this);
}
void Draw::stop() {
	alive = false;
}
SHORT Draw::showHM(SHORT& pos, bool helth) {
	for (int i = pos, j = 0; i < pos + 12; i++, j++)
		buffer[i] = j < matrix().helth
		? helth ? PGraph::helth : PGraph::mana
		: PGraph::empty;
	pos += 12;
	return pos;
}
SHORT Draw::showCoin(SHORT& pos, bool self) {
	buffer[pos++] = self ? PGraph::player : PGraph::guard;
	buffer[pos++] = PGraph::gold;
	CHAR_INFO ch;
	ch.Attributes = 7;
	ch.Char.UnicodeChar = (wchar_t)((self ? matrix().coin : matrix().enemyCoin) / 100 + 48);
	buffer[pos++] = (ch.Char.UnicodeChar > 48 ? ch : PGraph::empty);
	if (ch.Char.UnicodeChar > 48) {
		ch.Char.UnicodeChar = (wchar_t)(((self ? matrix().coin : matrix().enemyCoin) % 100) / 10 + 48);
		buffer[pos++] = ch;
	}
	else {
		ch.Char.UnicodeChar = (wchar_t)((self ? matrix().coin : matrix().enemyCoin) % 100) / 10 + 48;
		buffer[pos++] = (ch.Char.UnicodeChar > 48 ? ch : PGraph::empty);
	}
	ch.Char.UnicodeChar = (wchar_t)((self ? matrix().coin : matrix().enemyCoin) % 10 + 48);
	buffer[pos++] = ch;
	buffer[pos++] = PGraph::empty;
	ch.Char.UnicodeChar = L'/';
	buffer[pos++] = ch;
	buffer[pos++] = PGraph::empty;
	ch.Char.UnicodeChar = (wchar_t)(matrix().totalCoin / 100 + 48);
	buffer[pos++] = (ch.Char.UnicodeChar > 48 ? ch : PGraph::empty);
	if (ch.Char.UnicodeChar > 48) {
		ch.Char.UnicodeChar = (wchar_t)((matrix().totalCoin % 100) / 10 + 48);
		buffer[pos++] = ch;
	}
	else {
		ch.Char.UnicodeChar = (wchar_t)(matrix().totalCoin % 100) / 10 + 48;
		buffer[pos++] = (ch.Char.UnicodeChar > 48 ? ch : PGraph::empty);
	}
	ch.Char.UnicodeChar = (wchar_t)(matrix().totalCoin % 10 + 48);
	buffer[pos++] = ch;
	buffer[pos++] = PGraph::empty;
	return pos;
}
void Draw::setInterval(size_t ms) { msInterval = ms; }
void Draw::tick() {
	HANDLE hSTD = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT r = { 0, 0, m_matrix->count.X, m_matrix->count.Y + 1 };
	bool l = 1;
	int iii = 100;
	while (alive) {
		PGraph::setGraph();
		SHORT i = 0;
		buffer[i++] = PGraph::empty;
		showHM(i);
		showHM(i, false);
		showCoin(i);
		buffer[i++] = PGraph::empty;
		buffer[i++] = PGraph::empty;
		buffer[i++] = PGraph::empty;
		showCoin(i, false);
		while (i<m_matrix->count.X)buffer[i++] = PGraph::empty;

		if (++iii >= 300) {
			iii = 0;
			m_matrix->redraw_m.lock();
			m_matrix->_redraw->clear();
			m_matrix->redraw_m.unlock();
			for (; i < (int)m_matrix->count.X + m_matrix->count.X * m_matrix->count.Y; i++) {
				if (!alive)break;

				switch ((*m_matrix->array)(i - m_matrix->count.X).type()) {
				case Type::empty:
					buffer[i] = PGraph::empty;
					break;
				case Type::gold:
					buffer[i] = PGraph::gold;
					break;
				case Type::guard:
					buffer[i] = PGraph::guard;
					break;
				case Type::hunter:
					buffer[i] = PGraph::hunter;
					break;
				case Type::player:
					buffer[i] = PGraph::player;
					break;
				case Type::player2:
					buffer[i] = PGraph::player2;
					break;
				case Type::psycho:
					buffer[i] = PGraph::psycho;
					break;
				case Type::way:
					buffer[i] = PGraph::way;
					break;
				case Type::wall:
					register auto m = matrix()(i - m_matrix->count.X);
					register SHORT k = 0;

					k |= (&m->left() != m && m->left().type() == Type::wall ? 1 : 0);
					k |= (&m->right() != m && m->right().type() == Type::wall ? 2 : 0);
					k |= (&m->up() != m && m->up().type() == Type::wall ? 4 : 0);
					k |= (&m->down() != m && m->down().type() == Type::wall ? 8 : 0);

					buffer[i] = PGraph::wall[k];
				}
			}
			WriteConsoleOutputW(hSTD, buffer, { m_matrix->count.X, m_matrix->count.Y + 1 }, { 0,0 }, &r);
		}
		else {
			m_matrix->redraw_m.lock();
			map<Coord, Type>* rrr = m_matrix->_redraw;
			m_matrix->_redraw = new map<Coord, Type>();
			m_matrix->redraw_m.unlock();
			for(auto& cc : *rrr){
				Coord c = cc.first;
				PCHAR_INFO p = &buffer[c.X + c.Y*m_matrix->count.X+ m_matrix->count.X];
				switch (cc.second) {
				case Type::empty:
					*p = PGraph::empty;
					break;
				case Type::gold:
					*p = PGraph::gold;
					break;
				case Type::guard:
					*p = PGraph::guard;
					break;
				case Type::hunter:
					*p = PGraph::hunter;
					break;
				case Type::player:
					*p = PGraph::player;
					break;
				case Type::player2:
					*p = PGraph::player2;
					break;
				case Type::psycho:
					*p = PGraph::psycho;
					break;
				case Type::way:
					*p = PGraph::way;
					break;
				case Type::wall:
					register auto* m = (*m_matrix)(c.X,c.Y);
					register SHORT k = 0;

					k |= (&m->left() != m && m->left().type() == Type::wall ? 1 : 0);
					k |= (&m->right() != m && m->right().type() == Type::wall ? 2 : 0);
					k |= (&m->up() != m && m->up().type() == Type::wall ? 4 : 0);
					k |= (&m->down() != m && m->down().type() == Type::wall ? 8 : 0);

					*p = PGraph::wall[k];
				}
				DWORD n = 0;
				//WriteConsoleOutputAttribute(hSTD, &p->Attributes, 1, c,&n);
				//WriteConsoleOutputCharacterW(hSTD, &p->Char.UnicodeChar, 1, c, &n);
			}
			delete rrr;

			WriteConsoleOutputW(hSTD, buffer, { m_matrix->count.X, m_matrix->count.Y + 1 }, { 0,0 }, &r);

		}
		this_thread::sleep_for(std::chrono::milliseconds(msInterval));
//		Sleep(static_cast<DWORD>(msInterval));
		if (m_matrix->helth <= 0 && l) {
			thread t([] {Sleep(100); Luse(); });
			t.detach();
			l = 0;
		}
	}
}
