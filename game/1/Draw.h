#pragma once

class Draw {
	friend class CeilArray;
	thread* self;
	PCHAR_INFO buffer;
	atomic<bool> alive;
	size_t msInterval;
	CeilArray *m_matrix;
public:
	inline CeilArray& matrix() { return *m_matrix; }
	Draw(COORD size);
	~Draw();
	bool isAlive();
	void start();
	void stop();
	SHORT showHM(SHORT& pos, bool helth = true);
	SHORT showCoin(SHORT& pos, bool self = true);
	void setInterval(size_t ms);
	void tick();
};