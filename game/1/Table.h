#pragma once
class Table {
public:
	SHORT helth;
	SHORT mana;
	SHORT coin;
	SHORT totalCoin;
	SHORT enemyCoin;
	time_t time;

	Table()
		: helth(10)
		, mana(10)
		, coin(0)
		, enemyCoin(0)
		, totalCoin(100)
		, time(0)
	{
	
	
	}
} table;