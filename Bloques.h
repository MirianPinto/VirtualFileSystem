#pragma once
class Bloques
{
private:

	class BloqueDirecto
	{
	public:
		char BD[4096];
		BloqueDirecto();

		char* tocharBD();
		void fromcharBD(char[4096]);
	};

	class BloqueI1
	{
	public:
		unsigned int I1[16];
		BloqueI1();

		char* tocharBI1();
		void fromcharBI1(char*);
	private:

	};

	class BloqueI2
	{
	public:
		unsigned int I2[32];
		BloqueI2();

		char* tocharBI2();
		void fromcharBI2(char*);

	private:

	};

	class BloqueI3
	{
	public:
		unsigned int I3[64];
		BloqueI3();

		char* tocharBI3();
		void fromcharBI3(char*);
	private:

	};


public:

	BloqueDirecto* BDD;
	BloqueI1* I11;
	BloqueI2* I22;
	BloqueI3* I33;
	Bloques();


	char* tocharBLoqueDirecto();
	char* tocharBLoqueI1();
	char* tocharBLoqueI2();
	char* tocharBLoqueI3();




};

