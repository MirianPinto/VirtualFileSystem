#include "APIDisc.h"
#include <iostream>
#include <stdio.h>     
#include <time.h>
#include <string>
#include "MapaBits.h"
#include "Bloques.h"

using std::cout;
using namespace std;
struct tm newtime;
__time32_t aclock;


APIDisc::Metadata::Metadata() : cantED(0), tamañoBloque(0), cantBD(0), cantBINivel1(0), cantBINivel2(0), cantBINivel3(0)
{
	memcpy(namDisc, "NotDefined", strlen("NotDefined") + 1);
	memcpy(fechaCreacion, "NotDefined", strlen("NotDefined") + 1);
	//memcpy(fechaMod, "NotDefined", strlen("NotDefined") + 1);
}

APIDisc::Metadata::Metadata(const char _namDisc[], int _cantED)
	: cantED(_cantED), tamañoBloque(4096), cantBD(_cantED * 33308), cantBINivel1(_cantED * 2081), cantBINivel2(_cantED * 65), cantBINivel3(_cantED * 1)
{

	char buffer[32];
	errno_t errNum;
	_time32(&aclock);
	_localtime32_s(&newtime, &aclock);
	errNum = asctime_s(buffer, 32, &newtime);



	memcpy(namDisc, _namDisc, strlen(_namDisc) + 1);
	memcpy(fechaCreacion, buffer, strlen(buffer) + 1);




}




//convertir los valores ingresados a los atributos de la clase
//a char*
char* APIDisc::Metadata::toChar()
{
	char* charResult = new char[sizeof(Metadata)];

	memcpy(&charResult[0], &namDisc, sizeof(namDisc));
	memcpy(&charResult[sizeof(namDisc)], fechaCreacion, sizeof(fechaCreacion));
	memcpy(&charResult[sizeof(namDisc) + sizeof(fechaCreacion)], &cantED, sizeof(cantED));
	memcpy(&charResult[sizeof(namDisc) + sizeof(fechaCreacion) + sizeof(cantED)], &tamañoBloque, sizeof(tamañoBloque));
	memcpy(&charResult[sizeof(namDisc) + sizeof(fechaCreacion) + sizeof(cantED) + sizeof(tamañoBloque)], &(cantBD), sizeof(cantBD));
	memcpy(&charResult[sizeof(namDisc) + sizeof(fechaCreacion) + sizeof(cantED) + sizeof(tamañoBloque) + sizeof(cantBD)], &(cantBINivel1), sizeof(cantBINivel1));
	memcpy(&charResult[sizeof(namDisc) + sizeof(fechaCreacion) + sizeof(cantED) + sizeof(tamañoBloque) + sizeof(cantBD) + sizeof(cantBINivel1)], &(cantBINivel2), sizeof(cantBINivel2));
	memcpy(&charResult[sizeof(namDisc) + sizeof(fechaCreacion) + sizeof(cantED) + sizeof(tamañoBloque) + sizeof(cantBD) + sizeof(cantBINivel1) + sizeof(cantBINivel2)], &(cantBINivel3), sizeof(cantBINivel3));

	return charResult;
}

void APIDisc::Metadata::fromChar(char* _charReaded)
{

	memcpy(namDisc, &_charReaded[0], sizeof(namDisc));
	memcpy(fechaCreacion, &_charReaded[sizeof(namDisc)], sizeof(fechaCreacion));
	memcpy(&cantED, &_charReaded[sizeof(namDisc) + sizeof(fechaCreacion)], sizeof(cantED));
	memcpy(&tamañoBloque, &_charReaded[sizeof(namDisc) + sizeof(fechaCreacion) + sizeof(cantED)], sizeof(tamañoBloque));
	memcpy(&cantBD, &_charReaded[sizeof(namDisc) + sizeof(fechaCreacion) + sizeof(cantED) + sizeof(tamañoBloque)], sizeof(cantBD));
	memcpy(&cantBINivel1, &_charReaded[sizeof(namDisc) + sizeof(fechaCreacion) + sizeof(cantED) + sizeof(tamañoBloque) + sizeof(cantBD)], sizeof(cantBINivel1));
	memcpy(&cantBINivel2, &_charReaded[sizeof(namDisc) + sizeof(fechaCreacion) + sizeof(cantED) + sizeof(tamañoBloque) + sizeof(cantBD) + sizeof(cantBINivel1)], sizeof(cantBINivel2));
	memcpy(&cantBINivel3, &_charReaded[sizeof(namDisc) + sizeof(fechaCreacion) + sizeof(cantED) + sizeof(tamañoBloque) + sizeof(cantBD) + sizeof(cantBINivel1) + sizeof(cantBINivel2)], sizeof(cantBINivel3));



}

int APIDisc::Metadata::getSizeOf()
{
	return sizeof(Metadata);
}

void APIDisc::Metadata::PrintMetadata()
{
	cout << "METADATA" << endl
		<< "Nombre: " << namDisc << endl
		<< "FechaCreacion: " << fechaCreacion << endl
		<< "Entradas de direcctorio: " << cantED << endl
		<< "Cantidad BD: " << cantBD << endl
		<< "Cantidad BI1: " << cantBINivel1 << endl
		<< "Cantidad BI2: " << cantBINivel2 << endl
		<< "Cantidad BI3: " << cantBINivel3 << endl;

}

APIDisc::APIDisc()
{
	file = new DataFile(nullptr);
}
void APIDisc::Nuevodisco(const char* _Disckname)
{
	file = new DataFile(_Disckname);


}
bool APIDisc::Existe()
{
	return file->Existe();
}

void APIDisc::saveMetadata(const char* _nombre, int _catEd)
{
	file->open("W");



	Metadata* meta = new Metadata(_nombre, _catEd);



	file->write(meta->toChar(), 0, meta->getSizeOf());

	//escribir mapaBits
	MapaBits* mapi = new MapaBits(meta->cantED);
	file->write(mapi->toChar(), meta->getSizeOf(), mapi->getsizeof());

	Entradas* a = new Entradas();
	a->nuevaEntradas(file->path, meta->cantED);
	a->writeentradas();

	file->close();


}

void APIDisc::readMetadata()
{
	int currentPosition = 0;
	file->open("R");

	Metadata* toFind = new Metadata();
	toFind->fromChar(file->read(currentPosition, toFind->getSizeOf()));
	toFind->PrintMetadata();
	cout << "\nTamaño de metadata: " << toFind->getSizeOf();

	MapaBits* mapi = new MapaBits(toFind->cantED);

	mapi->fromChar(file->read(toFind->getSizeOf(), mapi->getsizeof()));

	cout << "\nTamaño de MapaBits: " << mapi->getsizeof();


	Entradas* a = new Entradas();
	a->nuevaEntradas(file->path, toFind->cantED);
	cout << "\nTamaño de Entrada: " << a->sizeofEntrada();




	file->close();

}

unsigned int APIDisc::CntED()
{
	int currentPosition = 0;
	file->open("R");

	Metadata* toFind = new Metadata();
	toFind->fromChar(file->read(currentPosition, toFind->getSizeOf()));
	toFind->PrintMetadata();

	file->close();
	return toFind->cantED;
}

void APIDisc::nuevaEntrada()
{

}





