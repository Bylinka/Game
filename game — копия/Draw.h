#pragma once

class Draw {
	thread* self;
	PCHAR_INFO buffer;
	atomic<bool> alive;
	size_t msInterval;
	Matrix* m_matrix;
public:
	inline Matrix* matrix() { return m_matrix; }
	Draw()
		:  buffer((PCHAR_INFO)malloc((matrix_size.X * matrix_size.Y + matrix_size.X) * sizeof(PCHAR_INFO)))
		, self(nullptr)
		, alive(false)
		, msInterval(20)
	{
		m_matrix = new Matrix(matrix_size);
		start();
	}
	~Draw() {
		if (self) {
			alive = false;
			self->join();
			delete self;
		}
		if(buffer)free(buffer);
		delete m_matrix;
	}
	bool isAlive() { return alive; }
	void start() {
		alive = true;
		self = new thread(&Draw::tick, this);
	}
	void stop() {
		alive = false;
	}
	SHORT showHM(SHORT& pos, bool helth = true) {
		for (int i = pos, j = 0; i < pos + 12; i++, j++)
			buffer[i] = j < table.helth
			? helth ? PGraph::helth : PGraph::mana
			: PGraph::empty;
		pos += 12;
		return pos;
	}
	SHORT showCoin(SHORT& pos, bool self = true) {
		buffer[pos++] = self ? PGraph::player : PGraph::guard;
		buffer[pos++] = PGraph::gold;
		CHAR_INFO ch;
		ch.Attributes = 7;
		ch.Char.UnicodeChar = (wchar_t)((self ? table.coin : table.enemyCoin) / 100 + 48);
		buffer[pos++] = (ch.Char.UnicodeChar > 48 ? ch : PGraph::empty);
		if (ch.Char.UnicodeChar > 48) {
			ch.Char.UnicodeChar = (wchar_t)(((self ? table.coin : table.enemyCoin) % 100) / 10 + 48);
			buffer[pos++] = ch;
		}
		else {
			ch.Char.UnicodeChar = (wchar_t)((self ? table.coin : table.enemyCoin) % 100) / 10 + 48;
			buffer[pos++] = (ch.Char.UnicodeChar > 48 ? ch : PGraph::empty);
		}
		ch.Char.UnicodeChar = (wchar_t)((self ? table.coin : table.enemyCoin) % 10 + 48);
		buffer[pos++] = ch;
		buffer[pos++] = PGraph::empty;
		ch.Char.UnicodeChar = L'/';
		buffer[pos++] = ch;
		buffer[pos++] = PGraph::empty;
		ch.Char.UnicodeChar = (wchar_t)(table.totalCoin / 100 + 48);
		buffer[pos++] = (ch.Char.UnicodeChar > 48 ? ch : PGraph::empty);
		if (ch.Char.UnicodeChar > 48) {
			ch.Char.UnicodeChar = (wchar_t)((table.totalCoin % 100) / 10 + 48);
			buffer[pos++] = ch;
		}
		else {
			ch.Char.UnicodeChar = (wchar_t)(table.totalCoin % 100) / 10 + 48;
			buffer[pos++] = (ch.Char.UnicodeChar > 48 ? ch : PGraph::empty);
		}
		ch.Char.UnicodeChar = (wchar_t)(table.totalCoin % 10 + 48);
		buffer[pos++] = ch;
		buffer[pos++] = PGraph::empty;
		return pos;
	}
	void setInterval(size_t ms) { msInterval = ms; }
	void tick() {
		HANDLE hSTD = GetStdHandle(STD_OUTPUT_HANDLE);
		SMALL_RECT r = { 0, 0, matrix_size.X, matrix_size.Y + 1 };

		while (alive) {
			SHORT i = 0;
			buffer[i++] = PGraph::empty;
			showHM(i);
			showHM(i, false);
			showCoin(i);
			buffer[i++] = PGraph::empty;
			buffer[i++] = PGraph::empty;
			buffer[i++] = PGraph::empty;
			showCoin(i, false);
			while(i<matrix_size.X)buffer[i++] = PGraph::empty;
			for (int i = matrix_size.X; i < (int)matrix_size.X + matrix_size.X * matrix_size.Y; i++) {
				if (!alive)break;

				switch (m_matrix->start()[i - matrix_size.X]->getType()) {
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
					register SHORT k = 0;

					k |= (m_matrix->start()[i - matrix_size.X]->left()->getType() == Type::wall ? 1 : 0);
					k |= (m_matrix->start()[i - matrix_size.X]->right()->getType() == Type::wall ? 2 : 0);
					k |= (m_matrix->start()[i - matrix_size.X]->up()->getType() == Type::wall ? 4 : 0);
					k |= (m_matrix->start()[i - matrix_size.X]->down()->getType() == Type::wall ? 8 : 0);

					buffer[i] = PGraph::wall[k];
				}
			}
			WriteConsoleOutputW(hSTD, buffer, { matrix_size.X,matrix_size.Y + 1 }, { 0,0 }, &r);
			Sleep(static_cast<DWORD>(msInterval));
		}	
	}
};