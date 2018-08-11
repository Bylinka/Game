#pragma once
namespace PGraph {
	const CHAR_INFO empty = { ' ',7 };
	const WORD
		left = 1,
		right = 2,
		up = 4,
		down = 8;

	const CHAR_INFO wall[]{
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
	//for (int i = 0; i < 16; i++)
	//	wprintf(L"%d: %c -%s%s%s%s\n",
	//		i,
	//		PGraph::wall[i].Char.UnicodeChar,
	//		i & 1? L" left" : L"",
	//		i & 2? L" right" : L"",
	//		i & 4? L" up" : L"",
	//		i & 8? L" down" : L"");

	const CHAR_INFO gold = { L'\u2776' ,14 };
	const CHAR_INFO psycho = { L'\u263a',15 };
	const CHAR_INFO guard = { L'\u263a',12 };
	const CHAR_INFO hunter = { L'\u263a',13 };
	const CHAR_INFO player = { L'\u263b',11 };
	const CHAR_INFO player2 = { L'\u263b',10 };
	const CHAR_INFO way = { ' ',4 * 16 };
	const CHAR_INFO helth = { L'\u2665',12 };
	const CHAR_INFO mana = { L'\u2666',9 };
};
void Luse() {
	MessageBox(NULL, L"ÂÛ ÏÐÎÈÃÐÀËÈ!!!\nÂ ýòîé èãðå íå ðåàëüíî ïîáåäèòü:-)", L"ÏÐÎÈÃÐÛØ!!!", MB_ICONWARNING | MB_OK);
	exit(0);
}
void Win() {
	MessageBox(NULL, L"ÂÛ ÏOÁÅÄÈËÈ!!!\nÂû ñîâåðøèëè íåâîçìîæíîå!!!", L"ÏÎÁÅÄÀ!!!", MB_ICONWARNING | MB_OK);
	exit(0);
}