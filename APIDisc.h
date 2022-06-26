#pragma once
#include "DataFile.h"
#include "Entradas.h"
class APIDisc
{
private:
	DataFile* file;


	class Metadata
	{
	private:

		char namDisc[20];
		char fechaCreacion[80];
		//char fechaMod[8];
		//int size;

		//bloques de datos 
		unsigned int tamañoBloque; //4096


		int cantBD;
		int cantBINivel1;
		int cantBINivel2;
		int cantBINivel3; // =1


	public:
		Metadata();
		Metadata(const char[], int);

		//void printMetadata();
		char* toChar(); //convertir toda la clase a bytecode
		void fromChar(char*); //recibe de bytecode y lo inserta en los atributos de clase
		int getSizeOf(); //devuelve el sizeof de la clase
		void PrintMetadata();

		int cantED;   //dado por el usuario 
	};

public:
	APIDisc();
	void Nuevodisco(const char*);

	bool Existe();
	void saveMetadata(const char*, int);
	void readMetadata();
	unsigned int CntED();

	//entradas:
	void nuevaEntrada();


};


