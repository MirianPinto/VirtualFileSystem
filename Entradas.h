#pragma once


#include "DataFile.h"
#include "Bloques.h"
#include "MapaBits.h"

struct MDEntrada
{
	char nombreEntrada[30];
	bool esArchivo; // 1->File, 0->Folder
	unsigned int tamañoentrada;
	char fechaCreacion[80];
	//smallint indPadre
	unsigned int indice;
	unsigned int indPadre;
	unsigned int indPrimerhijo;
	unsigned int indHermanoDerecho;
	unsigned int ptrsBD[12];
	unsigned int ptrsBDI[3];

	MDEntrada()
	{
		memcpy(nombreEntrada, "NotDefined", strlen("NotDefined") + 1);
		memcpy(fechaCreacion, "NotDefined", strlen("NotDefined") + 1);
		tamañoentrada = 0;
		esArchivo = 0;
		indHermanoDerecho = 0;
		indPadre = 0;
		indPrimerhijo = 0;
		indice = 0;
		for (int i = 0; i < 12; i++)
		{
			ptrsBD[i] = 0;
		}
		for (int i = 0; i < 3; i++)
		{
			ptrsBDI[i] = 0;
		}
	}
};

class Entradas
{
private:
	DataFile* file;
	unsigned int sizeMetadata;
	unsigned int sizeMapaBits;
	int ED;
	Bloques* Bloqui;

	//Entrada del momento;
	MDEntrada Ahora;
	MDEntrada Principal;

	MapaBits* mapi;


public:
	Entradas();
	void nuevaEntradas(const char*, int);

	void writeentradas();
	MDEntrada fromcharentrada(char*);

	int sizeofEntrada();

	void EntradaNueva(const char*, bool);

	//cd
	void ActualizarAhora(const char*, const char*);

	unsigned int SizeArchivo(const char*);

	//ls
	void hijos();


	void ultimohijo(unsigned int);

	bool eshijo(const char*);

	MDEntrada Existe(const char*);

	void printEntrada(MDEntrada);

	const char* Directorio();

	void WriteINH(unsigned int);


	//bloques			Bloques enteros, Bytes restantes
	MDEntrada BloquesW(unsigned int, unsigned int);
	void WriteArchivo(MDEntrada);
	void Export(const char*);
	void Eliminar(const char*);
	void ElemiCarpeta(MDEntrada);
	void ElemiArchivo(MDEntrada);
	void sizeCarpeta(unsigned int);


	//bool Bloquedisponible(unsigned int);

};

