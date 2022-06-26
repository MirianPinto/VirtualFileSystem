#include "MapaBits.h"
#include <iostream>


//Mapa bits
MapaBits::MapaBits(int _ED)
{
	BD = new MapaBits_BD(_ED);
	BI1 = new MapaBits_BI1(_ED);
	BI2 = new MapaBits_BI2(_ED);
	BI3 = new MapaBits_BI3(_ED);

	Ed = _ED;
	cantidadtotalbloques = (BD->getsizeof_BD() + BI1->getsizeof_BI1() + BI2->getsizeof_BI2() + BI3->getsizeof_BI3());


	ptrstotal = new char[getsizeof()];
	memcpy(&ptrstotal[0], BD->toChar_BD(), BD->getsizeof_BD());
	memcpy(&ptrstotal[BD->getsizeof_BD()], BI1->toChar_BI1(), BI1->getsizeof_BI1());
	memcpy(&ptrstotal[BD->getsizeof_BD() + BI1->getsizeof_BI1()], BI2->toChar_BI2(), BI2->getsizeof_BI2());
	memcpy(&ptrstotal[BD->getsizeof_BD() + BI1->getsizeof_BI1() + BI2->getsizeof_BI2()], BI3->toChar_BI3(), BI3->getsizeof_BI3());
	ptrstotal[BD->getsizeof_BD() + BI1->getsizeof_BI1() + BI2->getsizeof_BI2() + BI3->getsizeof_BI3()] = '\0';

}

char* MapaBits::toChar()
{

	return ptrstotal;
}

void MapaBits::fromChar(char* _Mapabits)
{
	ptrstotal = _Mapabits;
}

int MapaBits::getsizeof()
{

	return cantidadtotalbloques;
}

void MapaBits::seton(int _nbloque)
{
	int posicionbyte = _nbloque / 8;
	int bitinicialbyte = posicionbyte * 8;

	for (int i = bitinicialbyte, shift = 7; i < bitinicialbyte + 8; i++, shift--)
	{
		if (_nbloque == i)
		{
			ptrstotal[posicionbyte] |= (1 << shift);
			break;
		}
	}
}

void MapaBits::setoff(int _nbloque)
{
	int posicionbyte = _nbloque / 8;
	int bitinicialbyte = posicionbyte * 8;
	for (int i = bitinicialbyte, shift = 7; i < bitinicialbyte + 8; i++, shift--)
	{
		if (_nbloque == i)
		{
			ptrstotal[posicionbyte] &= ~(1 << shift);
			break;
		}
	}
}

bool MapaBits::BitDesponible(int _bloque)
{
	// para BD 
	int posicionbyte = _bloque / 8;
	int bitinicialbyte = posicionbyte * 8;

	const int SHFT = 8 * sizeof(char) - 1;
	const char mask = 1 << SHFT;

	int antes = _bloque - bitinicialbyte;

	for (int i = bitinicialbyte, shift = 7; i < bitinicialbyte + 8; i++, shift--)
	{
		if (_bloque == i)
		{
			char tembyte;
			tembyte = ptrstotal[posicionbyte];

			for (int j = 0; j < SHFT + 1; j++)
			{
				if (antes == j) {


					if (tembyte & mask)
					{
						//ocupado - 1
						return false;
					}
					else
					{
						//libre  - 0
						return true;
					}
					//cout << endl;
				}
				tembyte = tembyte << 1;
			}
			//cout << " ";
			break;
		}
	}
	return false;
}


// Bloques directos
MapaBits::MapaBits_BD::MapaBits_BD(int ED)
{

	int CantBD = ((33308 * ED) / 8);
	if (((33308 * ED) % 8) != 0)
	{
		CantBD += 1;
	}
	Ed0 = ED;
	ptrs = new char[(CantBD)];

	for (int i = 0; i < (CantBD); i++)
	{
		ptrs[i] = 0;
	}


}

char* MapaBits::MapaBits_BD::toChar_BD()
{
	return ptrs;
}

void MapaBits::MapaBits_BD::fromChar_BD(char* _mapbit)
{
	ptrs = _mapbit;
}

int MapaBits::MapaBits_BD::getsizeof_BD()
{

	int size = ((33308 * Ed0) / 8);
	if (((33308 * Ed0) % 8) != 0)
	{
		size += 1;
	}

	return size;
}

//Bloques indirectos de 1 

MapaBits::MapaBits_BI1::MapaBits_BI1(int ED)
{

	int CantBI1 = ((2081 * ED) / 8);
	if (((2081 * ED) % 8) != 0)
	{
		CantBI1 += 1;
	}

	Ed1 = ED;
	ptrs = new char[CantBI1];

	for (int i = 0; i < CantBI1; i++)
	{
		ptrs[i] = 0;
	}
}

char* MapaBits::MapaBits_BI1::toChar_BI1()
{
	return ptrs;
}

void MapaBits::MapaBits_BI1::fromChar_BI1(char* _mapbit)
{
	ptrs = _mapbit;
}

int MapaBits::MapaBits_BI1::getsizeof_BI1()
{
	int size = ((2081 * Ed1) / 8);
	if (((2081 * Ed1) % 8) != 0)
	{
		size += 1;
	}

	return size;

}


//Bloques indirectos de 2
MapaBits::MapaBits_BI2::MapaBits_BI2(int ED)
{

	int CantBI2 = ((65 * ED) / 8);
	Ed2 = ED;
	if (((65 * ED) % 8) != 0)
	{
		CantBI2 += 1;
	}

	ptrs = new char[CantBI2];

	for (int i = 0; i < CantBI2; i++)
	{
		ptrs[i] = 0;
	}

}

char* MapaBits::MapaBits_BI2::toChar_BI2()
{
	return ptrs;
}

void MapaBits::MapaBits_BI2::fromChar_BI2(char* _mapbit)
{
	ptrs = _mapbit;
}

int MapaBits::MapaBits_BI2::getsizeof_BI2()
{
	int size = ((65 * Ed2) / 8);
	if (((65 * Ed2) % 8) != 0)
	{
		size += 1;
	}
	return size;
}

// Bloques Indirectos de 3
MapaBits::MapaBits_BI3::MapaBits_BI3(int ED)
{

	int CantBI3 = (ED / 8);
	Ed3 = ED;
	if ((ED % 8) != 0)
	{
		CantBI3 += 1;
	}

	ptrs = new char[CantBI3];


	for (int i = 0; i < (CantBI3); i++)
	{
		ptrs[i] = 0;
	}

}

char* MapaBits::MapaBits_BI3::toChar_BI3()
{
	return ptrs;
}

void MapaBits::MapaBits_BI3::fromChar_BI3(char* _mapbit)
{
	ptrs = _mapbit;
}

int MapaBits::MapaBits_BI3::getsizeof_BI3()
{
	int size = (Ed3 / 8);
	if ((Ed3 % 8) != 0)
	{
		size += 1;
	}
	return size;
}