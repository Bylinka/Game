#pragma once
#ifdef _XP
struct CONSOLE_FONT
{
	DWORD index;
	COORD dim;
};//CONSOLE_FONT

BOOL(WINAPI *SetConsoleFont)(HANDLE, DWORD);
BOOL(WINAPI *GetConsoleFontInfo)(HANDLE, BOOL, DWORD, CONSOLE_FONT*);
DWORD(WINAPI *GetNumberOfConsoleFonts)();
template<typename pfn_t>
inline bool LoadFunc(HMODULE hmod, const char *name, pfn_t &fn)
{
	fn = (pfn_t)GetProcAddress(hmod, name);
	return fn != 0;
}//LoadFunc
/*
HMODULE hmod = ::GetModuleHandleA("KERNEL32.DLL");
!LoadFunc(hmod, "SetConsoleFont", SetConsoleFont) ||
!LoadFunc(hmod, "GetConsoleFontInfo", GetConsoleFontInfo) ||
!LoadFunc(hmod, "GetNumberOfConsoleFonts", GetNumberOfConsoleFonts))
*/
void setFount(wchar_t*name) {

		HMODULE hmod = GetModuleHandleA("KERNEL32.DLL");     // функция здесь
		LoadFunc(hmod, "SetConsoleFont", SetConsoleFont);   // берем ее адрес
		SetConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), 8);  // устанавливаем 8-й шрифт..
															 //эксперементально установил что в обычной XP  это Lucida  Console = 6 или 8.
															 // разница только в размерах. 6 - помельче.
		SetConsoleCP(1251);            // устанавливаем кодировку ввода
		SetConsoleOutputCP(1251);  // устанавливаем кодировку вывода
								   // ************ Здесь начинается код программы **************
}
#else
void setFount(wchar_t*name) {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;
	cfi.dwFontSize.Y = 9;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, name);
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
#endif
namespace Consolas {
	wchar_t type[100] = L"Consolas";
	const CHAR_INFO wall[16]{
		/*0*/{ L' ', 7 },
		/*left*/{ L'\u2578', 2 },
		/*right*/{ L'\u257a', 2 },
		/*left | right*/{ L'\u2501', 2 },

		/*up*/{ L'\u2579', 2 },
		/*up | left*/{ L'\u251b', 2 },
		/*up | right*/{ L'\u2517', 2 },
		/*up | left | right*/{ L'\u253b', 2 },

		/*down*/{ L'\u257b', 2 },
		/*down | left*/{ L'\u2513', 2 },
		/*down | right*/{ L'\u250f', 2 },
		/*down | left | right */{ L'\u2533', 2 },

		/*down | up */{ L'\u2503', 2 },
		/*down | up | left*/{ L'\u252b', 2 },
		/*down | up | right*/{ L'\u2523', 2 },
		/*down | up | left | right*/{ L'\u254b', 2 }
	};
	const CHAR_INFO empty = { ' ',7 };
	const CHAR_INFO gold = { L'\u2776' ,14 };
	const CHAR_INFO psycho = { L'\u263a',15 };
	const CHAR_INFO guard = { L'\u263a',12 };
	const CHAR_INFO hunter = { L'\u263a',13 };
	const CHAR_INFO player = { L'\u263b',11 };
	const CHAR_INFO player2 = { L'\u263b',10 };
	const CHAR_INFO way = { ' ',4 * 16 };
	const CHAR_INFO helth = { L'\u2665',12 };
	const CHAR_INFO mana = { L'\u2666',9 };
}
namespace LucidaConsole {
	wchar_t type[100] = L"Lucida Console";
	const CHAR_INFO wall[16]{
		/*0*/{ L' ', 7 },
		/*left*/{ L'\u2550', 2 },
		/*right*/{ L'\u2550', 2 },
		/*left | right*/{ L'\u2550', 2 },

		/*up*/{ L'\u2551', 2 },
		/*up | left*/{ L'\u255d', 2 },
		/*up | right*/{ L'\u255a', 2 },
		/*up | left | right*/{ L'\u2569', 2 },

		/*down*/{ L'\u2551', 2 },
		/*down | left*/{ L'\u2557', 2 },
		/*down | right*/{ L'\u2554', 2 },
		/*down | left | right */{ L'\u2566', 2 },

		/*down | up */{ L'\u2551', 2 },
		/*down | up | left*/{ L'\u2563', 2 },
		/*down | up | right*/{ L'\u2560', 2 },
		/*down | up | left | right*/{ L'\u256c', 2 }
	};
	const CHAR_INFO empty = { ' ',7 };
	const CHAR_INFO gold = { L'\u25cb' ,14 };
	const CHAR_INFO psycho = { L'\u263a',15 };
	const CHAR_INFO guard = { L'\u263a',12 };
	const CHAR_INFO hunter = { L'\u263a',13 };
	const CHAR_INFO player = { L'\u263b',11 };
	const CHAR_INFO player2 = { L'\u263b',10 };
	const CHAR_INFO way = { ' ',4 * 16 };
	const CHAR_INFO helth = { L'\u2665',12 };
	const CHAR_INFO mana = { L'\u2666',9 };
}
namespace Terminal {
	wchar_t type[100] = L"Terminal";
	const CHAR_INFO wall[16]{
		/*0*/{ L' ', 7 },
		/*left*/{ L'\u2550', 2 },
		/*right*/{ L'\u2550', 2 },
		/*left | right*/{ L'\u2550', 2 },

		/*up*/{ L'\u2551', 2 },
		/*up | left*/{ L'\u255d', 2 },
		/*up | right*/{ L'\u255a', 2 },
		/*up | left | right*/{ L'\u2569', 2 },

		/*down*/{ L'\u2551', 2 },
		/*down | left*/{ L'\u2557', 2 },
		/*down | right*/{ L'\u2554', 2 },
		/*down | left | right */{ L'\u2566', 2 },

		/*down | up */{ L'\u2551', 2 },
		/*down | up | left*/{ L'\u2563', 2 },
		/*down | up | right*/{ L'\u2560', 2 },
		/*down | up | left | right*/{ L'\u256c', 2 }
	};
	const CHAR_INFO empty = { ' ',7 };
	const CHAR_INFO gold = { L'\u25cb' ,14 };
	const CHAR_INFO psycho = { L'\u263a',15 };
	const CHAR_INFO guard = { L'\u263a',12 };
	const CHAR_INFO hunter = { L'\u263a',13 };
	const CHAR_INFO player = { L'\u263b',11 };
	const CHAR_INFO player2 = { L'\u263b',10 };
	const CHAR_INFO way = { ' ',4 * 16 };
	const CHAR_INFO helth = { L'\u2665',12 };
	const CHAR_INFO mana = { L'\u2666',9 };

}
namespace PGraph{
	wchar_t type[100] = L"";
#ifdef _XP
	const CHAR_INFO wall[16]{
		/*0*/{ L' ', 7 },
		/*left*/{ L'\u2550', 2 },
		/*right*/{ L'\u2550', 2 },
		/*left | right*/{ L'\u2550', 2 },

		/*up*/{ L'\u2551', 2 },
		/*up | left*/{ L'\u255d', 2 },
		/*up | right*/{ L'\u255a', 2 },
		/*up | left | right*/{ L'\u2569', 2 },

		/*down*/{ L'\u2551', 2 },
		/*down | left*/{ L'\u2557', 2 },
		/*down | right*/{ L'\u2554', 2 },
		/*down | left | right */{ L'\u2566', 2 },

		/*down | up */{ L'\u2551', 2 },
		/*down | up | left*/{ L'\u2563', 2 },
		/*down | up | right*/{ L'\u2560', 2 },
		/*down | up | left | right*/{ L'\u256c', 2 }
	};
	const CHAR_INFO empty = { ' ',7 };
	const CHAR_INFO gold = { L'\u25cb' ,14 };
	const CHAR_INFO psycho = { L'\u263a',15 };
	const CHAR_INFO guard = { L'\u263a',12 };
	const CHAR_INFO hunter = { L'\u263a',13 };
	const CHAR_INFO player = { L'\u263b',11 };
	const CHAR_INFO player2 = { L'\u263b',10 };
	const CHAR_INFO way = { ' ',4 * 16 };
	const CHAR_INFO helth = { L'\u2665',12 };
	const CHAR_INFO mana = { L'\u2666',9 };

#else
	CHAR_INFO wall[16]{
		/*0*/{ L' ', 7 },
		/*left*/{ L'\u2578', 2 },
		/*right*/{ L'\u257a', 2 },
		/*left | right*/{ L'\u2501', 2 },

		/*up*/{ L'\u2579', 2 },
		/*up | left*/{ L'\u251b', 2 },
		/*up | right*/{ L'\u2517', 2 },
		/*up | left | right*/{ L'\u253b', 2 },

		/*down*/{ L'\u257b', 2 },
		/*down | left*/{ L'\u2513', 2 },
		/*down | right*/{ L'\u250f', 2 },
		/*down | left | right */{ L'\u2533', 2 },

		/*down | up */{ L'\u2503', 2 },
		/*down | up | left*/{ L'\u252b', 2 },
		/*down | up | right*/{ L'\u2523', 2 },
		/*down | up | left | right*/{ L'\u254b', 2 }
	};
	CHAR_INFO empty = { ' ',7 };
	CHAR_INFO gold = { L'\u2776' ,14 };
	CHAR_INFO psycho = { L'\u263a',15 };
	CHAR_INFO guard = { L'\u263a',12 };
	CHAR_INFO hunter = { L'\u263a',13 };
	CHAR_INFO player = { L'\u263b',11 };
	CHAR_INFO player2 = { L'\u263b',10 };
	CHAR_INFO way = { ' ',4 * 16 };
	CHAR_INFO helth = { L'\u2665',12 };
	CHAR_INFO mana = { L'\u2666',9 };
#endif

	static const WORD
		left = 1,
		right = 2,
		up = 4,
		down = 8;

	#define copy(T) \
		wsprintf((wchar_t*)type, T::type);\
		memcpy((PCHAR_INFO)wall, T::wall, sizeof(wall));\
		memcpy((PCHAR_INFO)&empty, &T::empty, sizeof(CHAR_INFO));\
		memcpy((PCHAR_INFO)&gold, &T::gold, sizeof(CHAR_INFO));\
		memcpy((PCHAR_INFO)&psycho, &T::psycho, sizeof(CHAR_INFO));\
		memcpy((PCHAR_INFO)&guard, &T::guard, sizeof(CHAR_INFO));\
		memcpy((PCHAR_INFO)&hunter, &T::hunter, sizeof(CHAR_INFO));\
		memcpy((PCHAR_INFO)&player, &T::player, sizeof(CHAR_INFO));\
		memcpy((PCHAR_INFO)&player2, &T::player2, sizeof(CHAR_INFO));\
		memcpy((PCHAR_INFO)&way, &T::way, sizeof(CHAR_INFO));\
		memcpy((PCHAR_INFO)&helth, &T::helth, sizeof(CHAR_INFO));\
		memcpy((PCHAR_INFO)&mana, &T::mana, sizeof(CHAR_INFO));

	static void setGraph() {
#ifndef _XP
		HANDLE std = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_FONT_INFOEX cfi;
		memset(&cfi, 0, sizeof(cfi));
		cfi.cbSize = sizeof(cfi);

		GetCurrentConsoleFontEx(std, FALSE, &cfi);

		if (wcscmp(cfi.FaceName, type)) {
			if (wcscmp(cfi.FaceName, L"Consolas") == 0) {
				setFount(L"Consolas");
				copy(Consolas)
			}
			else if (wcscmp(cfi.FaceName, L"Lucida Console") == 0) {
				setFount(L"Lucida Console");
				copy(LucidaConsole)
			}
			else if (wcscmp(cfi.FaceName, L"Terminal") == 0) {
				setFount(L"Terminal");
				copy(Terminal)
			}
			else {
				setFount(L"Consolas");
				GetCurrentConsoleFontEx(std, FALSE, &cfi);
				if (wcscmp(cfi.FaceName, L"Consolas")) {
					setFount(L"Lucida Console");
					GetCurrentConsoleFontEx(std, FALSE, &cfi);
					if (wcscmp(cfi.FaceName, L"Lucida Console")) {
						setFount(L"Lucida Console");
						copy(Terminal)
					}
					else {
						copy(LucidaConsole)
					}
				}
				else {
					copy(Consolas)
				}
			}
		}
#endif
	}

};


void Luse() {
	MessageBox(NULL, L"ВЫ ПРОИГРАЛИ!!!\nВ этой игре не реально победить:-)", L"ПРОИГРЫШ!!!", MB_ICONWARNING | MB_OK);
	exit(0);
}
void Win() {
	MessageBox(NULL, L"ВЫ ПOБЕДИЛИ!!!\nВы совершили невозможное!!!", L"ПОБЕДА!!!", MB_ICONWARNING | MB_OK);
	exit(0);
}
