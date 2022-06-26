#pragma once
class MapaBits
{


private:

	class MapaBits_BD
	{
	public:
		MapaBits_BD(int);
		char* ptrs;//new char[nbloquesBD/8]
		int Ed0;


		char* toChar_BD();
		void fromChar_BD(char*);

		int getsizeof_BD();
	};


	class MapaBits_BI1
	{
	public:
		MapaBits_BI1(int);
		char* ptrs;//new cahr[nbloquesI1/8]
		int Ed1;


		char* toChar_BI1();
		void fromChar_BI1(char*);

		int getsizeof_BI1();
	};


	class MapaBits_BI2
	{
	public:
		MapaBits_BI2(int);
		char* ptrs;//new cahr[nbloquesI2/8]
		int Ed2;


		char* toChar_BI2();
		void fromChar_BI2(char*);

		int getsizeof_BI2();
	};

	class MapaBits_BI3
	{
	public:
		MapaBits_BI3(int);
		char* ptrs; //new cahr[nbloquesI3/8]
		int Ed3;


		char* toChar_BI3();
		void fromChar_BI3(char*);

		int getsizeof_BI3();
	};
	MapaBits_BD* BD;
	MapaBits_BI1* BI1;
	MapaBits_BI2* BI2;
	MapaBits_BI3* BI3;


public:



	MapaBits(int);

	int Ed;
	int cantidadtotalbloques;

	char* ptrstotal;


	char* toChar();
	void fromChar(char*);
	int getsizeof();

	void seton(int);
	void setoff(int);
	bool BitDesponible(int);


};


