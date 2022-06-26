#include "Bloques.h"
#include <iostream>


Bloques::BloqueDirecto::BloqueDirecto()
{

	for (int i = 0; i < 4096; i++)
	{
		BD[i] = '*';
	}
}

char* Bloques::BloqueDirecto::tocharBD()
{
	char* resultado = new char[4096];
	memcpy(resultado, &BD[0], 4096);


	return resultado;
}

void Bloques::BloqueDirecto::fromcharBD(char _nuevo[4096])
{
	memcpy(BD, &_nuevo[0], 4096);

}

Bloques::Bloques()
{
	BDD = new BloqueDirecto();
	I11 = new BloqueI1();
	I22 = new BloqueI2();
	I33 = new BloqueI3();
}

char* Bloques::tocharBLoqueDirecto()
{
	return BDD->tocharBD();
}

char* Bloques::tocharBLoqueI1()
{
	return I11->tocharBI1();
}

char* Bloques::tocharBLoqueI2()
{

	return I22->tocharBI2();
}

char* Bloques::tocharBLoqueI3()
{
	BloqueI3* b3 = new BloqueI3();
	return I33->tocharBI3();
}


Bloques::BloqueI1::BloqueI1()
{

	for (int i = 0; i < 16; i++)
	{
		I1[i] = 0;
	}

}

char* Bloques::BloqueI1::tocharBI1()
{
	char* result = new  char[sizeof(I1)];
	memcpy(result, &I1[0], sizeof(I1));
	result[sizeof(I1)] = '\0';


	return result;
}

void Bloques::BloqueI1::fromcharBI1(char* _nuevo)
{

	memcpy(&I1[0], &_nuevo[0], sizeof(I1));

}




Bloques::BloqueI2::BloqueI2()
{

	for (int i = 0; i < 32; i++)
	{
		I2[i] = 0;
	}
}

char* Bloques::BloqueI2::tocharBI2()
{

	char* result = new  char[sizeof(I2)];
	memcpy(result, &I2[0], sizeof(I2));
	result[sizeof(I2)] = '\0';


	return result;
}

void Bloques::BloqueI2::fromcharBI2(char* _nuevo)
{
	memcpy(&I2[0], &_nuevo[0], sizeof(I2));
}



Bloques::BloqueI3::BloqueI3()
{

	for (int i = 0; i < 64; i++)
	{
		I3[i] = 0;
	}

}

char* Bloques::BloqueI3::tocharBI3()
{
	char* result = new  char[sizeof(I3)];
	memcpy(result, &I3[0], sizeof(I3));
	result[sizeof(I3)] = '\0';


	return result;
}

void Bloques::BloqueI3::fromcharBI3(char* _nuevo)
{
	memcpy(&I3[0], &_nuevo[0], sizeof(I3));
}
