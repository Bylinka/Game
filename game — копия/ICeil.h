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
COORD matrix_size;
/*
struct Safe {
atomic<bool> ready;
ICeil* object;
COORD position;
operator bool() {
return ready;
}
};
*/
class ICeil{
	ICeil** m_matrix;
	COORD m_position;
	COORD get_pos(ICeil** self) {
		if (*self) {
			if ((*self)->m_position.X == -1)
				return { 0,0 };
			else {
				COORD p = (*self)->m_position;
				p.X++;
				if (p.X == matrix_size.X)
					return { 0 ,p.Y + 1 };
				else
					return p;
			}
		}
		return  get_pos(self - 1);
		//auto p = get_pos(self-1);
		//p.X++;
		//if (p.X == matrix_size.X)
		//	return { 0,p.Y + 1 };
		//else
		//	return p;
	}
public:
	ICeil(ICeil** self, bool set=true)
	{
		if (set) {
			//ICeil* s = *self;
			//m_matrix = self;
			//*position(self) = this;
			//if (s)
			//	delete s;
			//s = NULL;

			*clear(position(self)) = this;
		}
		else {
			m_matrix = position(self);
		}
	}
	ICeil(ICeil** self,COORD pos)
	{

		m_matrix = self;
		*m_matrix = this;
		m_position = pos;
		
	}
	ICeil** clear(ICeil** self) {
		if (*self)
			delete *self;
		*self = NULL;
		m_matrix = self;
		return self;
	}

	inline COORD& position() { return m_position; }
	inline ICeil** matrix(ICeil** self=NULL) { 
		if (self)m_matrix = self;
		return m_matrix; 
	}
	inline ICeil** position(ICeil** self) {
		if (*self) {
			m_position = (*self)->m_position;
		}
		else {
			m_position = get_pos(self);
		}
		return self;
	}
	virtual ~ICeil() {}
	inline ICeil** begin() {
		return (m_matrix - m_position.X - (m_position.Y*matrix_size.X));
	}
	inline ICeil*& test(ICeil** pos) {
		return pos >= begin()
			&& pos < begin() + matrix_size.X * matrix_size.Y
			? *pos : begin()[-1];
	}
	inline ICeil*& left() {
		return (*m_matrix)->m_position.X ? test(m_matrix - 1) : begin()[-1];
	}
	inline ICeil*& right() { 
		return (*m_matrix)->m_position.X < matrix_size.X - 1 ? test(m_matrix + 1) : begin()[-1];
	}
	inline ICeil*& up() {
		return (*m_matrix)->m_position.Y ? test(m_matrix - matrix_size.X) : begin()[-1];
	}
	inline ICeil*& down() { 
		return (*m_matrix)->m_position.Y < matrix_size.Y - 1 ? test(m_matrix + matrix_size.X) : begin()[-1];
	}

	ICeil*& operator[](size_t i) { return test(begin() +i); }
	ICeil*& operator[](COORD i) { return test(begin()+i.Y * matrix_size.X + i.X); }
	virtual Type getType() = 0;
};
class Empty : public ICeil {
public:
	Empty(ICeil** self) :ICeil(self) {}
	Empty(ICeil** self,COORD pos) :ICeil(self, pos) {}
	Type getType() { return Type::empty; }
};
class Wall : public  ICeil {
public:
	Wall(ICeil** self) :ICeil(self) {}
	Type getType() { return Type::wall; }
};