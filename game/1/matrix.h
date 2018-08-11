#pragma once
class Matrix : enable_shared_from_this<Matrix> {
	friend class ICeil;
	vector<shared_ptr<ICeil>> object;
	COORD m_size;
public:
	COORD enter, exit;
	inline COORD& size() { return m_size; }
	inline size_t length() { return m_size.X*m_size.Y; }
	inline shared_ptr<Matrix> getptr() { return shared_from_this(); }
	shared_ptr<ICeil> add(ICeil* item) {
		object.push_back(shared_ptr<ICeil>(item));
		return object[object.size() - 1];
	}
	//Matrix(COORD size)
	//	: m_size(size)
	//{
	//	for (register int i = 0; i<length(); i++) {
	//		object.push_back(shared_ptr<ICeil>(new Empty()));
	//		object[i]->position() = { (SHORT)i % size.X,(SHORT)i / size.X };
	//		object[i]->matrix(getptr());
	//	}
	//}

	shared_ptr<ICeil> operator[](size_t i) {
		if (i >= 0 && i<length())
			return object[i];
		return nullptr;
	}
	shared_ptr<ICeil> operator[](const COORD i) {
		if (i.X >= 0
			&& i.X < m_size.X
			&& i.Y >= 0
			&& i.Y < m_size.Y)
			return object[i.Y * m_size.X + i.X];
		return nullptr;
	}
	shared_ptr<ICeil> reset(COORD pos, ICeil* item) {
		auto f = shared_ptr<ICeil>(item);
		object[pos.Y * m_size.X + pos.X].swap(f);
		return object[pos.Y * m_size.X + pos.X];
	}
	//shared_ptr<ICeil> operator[](ICeil* i) {
	//	if (i->position().X >= 0
	//		&& i->position().X < m_size.X
	//		&& i->position().Y >= 0
	//		&& i->position().Y < m_size.Y)
	//		return object[i->position().Y * m_size.X + i->position().X];
	//	return nullptr;
	//}
};
