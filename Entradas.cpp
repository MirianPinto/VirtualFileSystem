#include "Entradas.h"
#include <iostream>
#include <stdio.h>     
#include <time.h>
#include <string>


using namespace std;
struct tm newtime2;
__time32_t aclock2;

Entradas::Entradas()
{
	file = new DataFile(nullptr);
	sizeMetadata = 124;
	sizeMapaBits = 0;
	ED = 0;
	Bloqui = new Bloques();
	mapi = new MapaBits(0);
}

void Entradas::nuevaEntradas(const char* _path, int _ed)
{
	file = new DataFile(_path);
	mapi = new MapaBits(_ed);
	sizeMetadata = 124;
	sizeMapaBits = mapi->getsizeof();
	ED = _ed;
	Bloqui = new Bloques();

	memcpy(Principal.nombreEntrada, _path, strlen(_path) + 1);
}

void Entradas::writeentradas()
{

	file->open("RW");

	MDEntrada entrada;

	long puntero = (sizeMapaBits + sizeMetadata);


	for (int i = 0; i < ED; i++)
	{
		entrada.indice = (i + 1);
		file->write(reinterpret_cast<char*>(&entrada), puntero, sizeof(MDEntrada));
		puntero += sizeof(MDEntrada);
	}

	Bloques* Bloqui = new Bloques();
	long ping = (sizeMapaBits + sizeMetadata + (sizeof(MDEntrada) * ED));



	for (int i = 0; i < (33308 * ED); i++)
	{
		file->write(Bloqui->tocharBLoqueDirecto(), ping, 4096);
		ping += 4096;
	}


	for (int j = 0; j < (2081 * ED); j++) {

		file->write(Bloqui->tocharBLoqueI1(), ping, 64);
		ping += 64;

	}

	for (int j = 0; j < (65 * ED); j++) {

		file->write(Bloqui->tocharBLoqueI2(), ping, 128);
		ping += 128;

	}

	for (int j = 0; j < ED; j++) {

		file->write(Bloqui->tocharBLoqueI3(), ping, 256);
		ping += 256;

	}

	file->close();

}

MDEntrada Entradas::fromcharentrada(char* _mdchar)
{
	MDEntrada leido;

	memcpy(&leido, &_mdchar[0], sizeof(MDEntrada));


	return leido;
}

int Entradas::sizeofEntrada()
{
	return sizeof(MDEntrada);
}

void Entradas::sizeCarpeta(unsigned int _sizenueva)
{
	MDEntrada nueva;
	unsigned int puntero = sizeMetadata + sizeMapaBits;
	unsigned int nuevaentrada = puntero;

	nueva = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
	puntero += sizeof(MDEntrada);

	if (strcmp(Ahora.nombreEntrada, file->path) != 0)
	{




		for (int i = 0; i < ED; i++)
		{

			if (strcmp(nueva.nombreEntrada, "NotDefined") != 0)
			{
				if (strcmp(nueva.nombreEntrada, Ahora.nombreEntrada) == 0)
				{
					if (nueva.indice == Ahora.indice) {
						nueva.tamañoentrada += _sizenueva;

						file->write(reinterpret_cast<char*>(&nueva), nuevaentrada, sizeof(MDEntrada));

						Ahora = nueva;
						break;
					}

				}

			}
			nuevaentrada = puntero;
			nueva = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
			puntero += sizeof(MDEntrada);
		}
	}

}

void Entradas::EntradaNueva(const char* _nombre, bool _folach)
{
	//cout << file->path;
	file->open("RW");

	MDEntrada nueva;
	MDEntrada punteros;
	unsigned int puntero = sizeMetadata + sizeMapaBits;
	unsigned int nuevaentrada = puntero;

	nueva = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
	puntero += sizeof(MDEntrada);

	int cantbloque = 0;
	int restante = 0;
	int bloquerest = 0;

	for (int i = 0; i < ED; i++)
	{

		if (strcmp(nueva.nombreEntrada, "NotDefined") == 0)
		{

			memcpy(nueva.nombreEntrada, _nombre, strlen(_nombre) + 1);
			nueva.esArchivo = _folach;

			if (_folach)
			{
				nueva.tamañoentrada = SizeArchivo(_nombre);
				cantbloque = nueva.tamañoentrada / 4096;
				if ((nueva.tamañoentrada % 4096) != 0)
				{
					restante = nueva.tamañoentrada - (cantbloque * 4096);
					bloquerest++;
				}
				if ((cantbloque + bloquerest) > (33308 * ED))
				{
					cout << "Este archivo es demasiado grande\n";
					MDEntrada mal;
					mal.indice = nueva.indice;
					nueva = mal;
					break;
				}
				punteros = BloquesW(cantbloque, restante);
				memcpy(&nueva.ptrsBD, &punteros.ptrsBD[0], 48);
				memcpy(&nueva.ptrsBDI, &punteros.ptrsBDI[0], 12);

				WriteArchivo(nueva);
			}
			else
			{
				nueva.tamañoentrada = 192;
				cantbloque = 0;
			}

			if (Ahora.indPrimerhijo == 0)
			{
				WriteINH(nueva.indice);
			}

			sizeCarpeta(nueva.tamañoentrada);

			char buffer[32];
			errno_t errNum;
			_time32(&aclock2);
			_localtime32_s(&newtime2, &aclock2);
			errNum = asctime_s(buffer, 32, &newtime2);
			memcpy(nueva.fechaCreacion, buffer, strlen(buffer) + 1);

			nueva.indPadre = Ahora.indice;
			ultimohijo(nueva.indice);
			nueva.indHermanoDerecho = 0;
			break;
		}
		nuevaentrada = puntero;
		nueva = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
		puntero += sizeof(MDEntrada);

	}


	file->write(reinterpret_cast<char*>(&nueva), nuevaentrada, sizeof(MDEntrada));
	file->close();





}

// para cambiar de directorio
void Entradas::ActualizarAhora(const char* _name, const char* path)
{
	file->open("RW");
	if (strcmp(_name, path) == 0)
	{
		Ahora = Principal;
	}
	else {

		MDEntrada Recibida;
		unsigned int puntero = sizeMetadata + sizeMapaBits;
		Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
		puntero += sizeof(MDEntrada);

		for (int i = 0; i < ED; i++)
		{
			if (strcmp(Recibida.nombreEntrada, _name) == 0)
			{
				if (!Recibida.esArchivo) {

					if (eshijo(_name)) {
						Ahora = Recibida;
						break;
					}
					else
					{
						cout << "\n Esta carpeta no es hijo\n";
						break;
					}
				}
				else
				{
					cout << "\n Esta no es una carpeta \n";
					break;
				}

			}
			Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
			puntero += sizeof(MDEntrada);

		}
	}



	file->close();


}

const char* Entradas::Directorio()
{

	return Ahora.nombreEntrada;

}

void Entradas::WriteINH(unsigned int _hijo)
{
	if (strcmp(Ahora.nombreEntrada, file->path) == 0)
	{
		//nada
	}
	else
	{
		MDEntrada Recibida;
		unsigned int puntero = sizeMetadata + sizeMapaBits;
		unsigned int punteroescri = puntero;
		Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
		puntero += sizeof(MDEntrada);


		for (int i = 0; i < ED; i++)
		{

			if (strcmp(Recibida.nombreEntrada, Ahora.nombreEntrada) == 0)
			{
				if (Ahora.indice == Recibida.indice) {
					Recibida.indPrimerhijo = _hijo;
					file->write(reinterpret_cast<char*>(&Recibida), punteroescri, sizeof(MDEntrada));
					break;
				}
			}
			punteroescri = puntero;
			Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
			puntero += sizeof(MDEntrada);


		}

	}


}

unsigned int Entradas::SizeArchivo(const char* archivo)
{
	DataFile* file2 = new DataFile(archivo);

	file2->open("R");

	unsigned int size = 0;

	size = file2->sizeArchivo();

	file2->close();
	return size;
}

void Entradas::hijos()
{
	file->open("RW");

	MDEntrada Recibida;
	unsigned int puntero = sizeMetadata + sizeMapaBits;
	Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
	puntero += sizeof(MDEntrada);

	char archivo = 'A';
	cout << "    Nombre" << "    Tipo    " << "Tamaño " << endl;
	for (int i = 0; i < ED; i++)
	{

		if (Recibida.indPadre == Ahora.indice)
		{

			if (strcmp(Recibida.nombreEntrada, "NotDefined") != 0)
			{
				cout << "   " << Recibida.nombreEntrada;
				if (Recibida.esArchivo)
				{
					archivo = 'A';
					cout << "     " << archivo;
				}
				else
				{
					archivo = 'F';
					cout << "     " << archivo;
				}

				cout << "      " << Recibida.tamañoentrada << endl;
			}


		}
		Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
		puntero += sizeof(MDEntrada);
	}

	file->close();

}


//solo revisar si funciona la escritura
MDEntrada Entradas::Existe(const char* _nombre)
{
	file->open("RW");

	MDEntrada Recibida;
	unsigned int puntero = sizeMetadata + sizeMapaBits;
	Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
	puntero += sizeof(MDEntrada);

	for (int i = 0; i < ED; i++)
	{
		if (strcmp(Recibida.nombreEntrada, _nombre) == 0) {
			//cout << "encontrada";
			printEntrada(Recibida);
			break;
		}
		Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
		puntero += sizeof(MDEntrada);
	}


	file->close();
	return Recibida;
}

void Entradas::printEntrada(MDEntrada ENTRADA)
{
	cout << "nombre: " << ENTRADA.nombreEntrada << endl
		<< "Tipo: " << ENTRADA.esArchivo << endl
		<< "Fecha: " << ENTRADA.fechaCreacion << endl
		<< "Indice: " << ENTRADA.indice << endl
		<< "Indice Padre: " << ENTRADA.indPadre << endl
		<< "Indice hijo: " << ENTRADA.indPrimerhijo << endl
		<< "Indice hermano: " << ENTRADA.indHermanoDerecho << endl
		<< "size: " << ENTRADA.tamañoentrada << endl;

}


void Entradas::ultimohijo(unsigned int _indice)
{
	MDEntrada Recibida;
	MDEntrada ultimohijo;
	unsigned int puntero = sizeMetadata + sizeMapaBits;

	Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
	puntero += sizeof(MDEntrada);

	for (int i = 0; i < ED; i++)
	{

		if (Recibida.indPadre == Ahora.indPadre)
		{
			if (strcmp(Recibida.nombreEntrada, Ahora.nombreEntrada) != 0)
			{
				if (Recibida.indHermanoDerecho == 0) {
					if (strcmp(Recibida.nombreEntrada, "NotDefined") != 0) {
						Recibida.indHermanoDerecho = _indice;
						file->write(reinterpret_cast<char*>(&Recibida), (puntero - 192), sizeof(MDEntrada));
						break;
					}
				}
			}
		}
		Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
		puntero += sizeof(MDEntrada);

	}

}


bool Entradas::eshijo(const char* _namehijo)
{

	MDEntrada Recibida;
	unsigned int puntero = sizeMetadata + sizeMapaBits;
	Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
	puntero += sizeof(MDEntrada);


	for (int i = 0; i < ED; i++)
	{

		if (Recibida.indPadre == Ahora.indice)
		{
			if (strcmp(Recibida.nombreEntrada, _namehijo) == 0)
			{
				return true;
			}

		}

		Recibida = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
		puntero += sizeof(MDEntrada);

	}



	return false;
}


MDEntrada Entradas::BloquesW(unsigned int _Bloqueentero, unsigned int _Byrestante)
{

	MDEntrada Punteros;

	unsigned int totalB = _Bloqueentero;
	if (_Byrestante != 0) {
		totalB++;
	}

	bool BI1 = 0;
	bool BI2 = 0;
	bool BI3 = 0;

	if (totalB > 12)
	{
		BI1 = 1;
	}
	else if (totalB > 28)
	{
		BI2 = 1;
	}
	else if (totalB > 540)
	{
		BI3 = 1;
	}


	unsigned int SBD = (33308 * ED);
	unsigned int SBI1 = (2081 * ED);
	unsigned int SBI2 = (65 * ED);
	unsigned int SBI3 = ED;



	mapi->fromChar(file->read(sizeMetadata, mapi->getsizeof()));


	//escribir en los primeors 12 bloques de la entrada
	int bd = 0;

	for (unsigned int i = 0; i < SBD; i++)
	{
		if (mapi->BitDesponible(i))
		{
			if (bd < 12 && bd < totalB) {
				Punteros.ptrsBD[bd] = sizeMetadata + sizeMapaBits + (sizeof(MDEntrada) * ED) + (i * 4096);
				mapi->seton(i);
				bd++;
			}
			else
			{
				break;
			}
		}
	}

	unsigned int sizeB1 = sizeMetadata + sizeMapaBits + (sizeof(MDEntrada) * ED) + (((33308 * ED) * 4096));

	//Escribir en el bloque indirecto un nivel
	int bi1 = 0;
	if (totalB > 12)
	{
		for (unsigned int i = SBD; i < (SBI1 + SBD); i++)
		{
			if (mapi->BitDesponible(i))
			{
				Punteros.ptrsBDI[0] = (sizeB1 + ((i - SBD) * 64));
				mapi->seton(i);

				Bloqui->I11->fromcharBI1(file->read(Punteros.ptrsBDI[0], 64));

				for (unsigned int j = 0; j < SBD; j++)
				{
					if (mapi->BitDesponible(j))
					{
						if (bi1 < 16 && bi1 < (totalB - 12))
						{
							Bloqui->I11->I1[bi1] = sizeMetadata + sizeMapaBits + (sizeof(MDEntrada) * ED) + (j * 4096);
							bi1++;
							mapi->seton(j);

						}

						if (bi1 == ((totalB - 12)) || bi1 == 16)
						{
							break;
						}
					}
				}
				file->write(Bloqui->I11->tocharBI1(), Punteros.ptrsBDI[0], 64);
				break;
			}
		}
	}

	// escribir en el Bloque indirecto de 2 nivel
	unsigned int sizeBi2 = sizeMetadata + sizeMapaBits + (sizeof(MDEntrada) * ED) + (((33308 * ED) * 4096) + ((2081 * ED) * 64));
	int CantBD = 0;
	int cantbi1 = 0;
	int contador = 0;
	int contador2 = 0;
	if (totalB > 28)
	{
		for (unsigned int i = (SBI1 + SBD); i < (SBI1 + SBD + SBI2); i++)
		{
			if (mapi->BitDesponible(i))
			{

				Punteros.ptrsBDI[1] = (sizeBi2 + ((i - (SBI1 + SBD)) * 128));
				mapi->seton(i);
				Bloqui->I22->fromcharBI2(file->read(Punteros.ptrsBDI[1], 128));
				CantBD = totalB - 12 - 16; // bloques directos que va a utilizar en cada bi1 son 16
				cantbi1 = CantBD / 16;// cantidad de bloques indirectos a utilizar solo puede utilizar 32

				if ((CantBD % 16) != 0)
				{
					cantbi1++;
				}

				for (unsigned int j = SBD; j < (SBI1 + SBD); j++)
				{
					if (mapi->BitDesponible(j))
					{

						if (contador < cantbi1 && contador < 32)
						{
							Bloqui->I22->I2[contador] = (sizeB1 + ((j - SBD) * 64));
							Bloqui->I11->fromcharBI1(file->read(Bloqui->I22->I2[contador], 64));
							mapi->seton(j);
							for (unsigned int b = 0; b < SBD; b++)
							{
								if (mapi->BitDesponible(b))
								{
									if (contador2 < CantBD && contador2 < 16)
									{
										Bloqui->I11->I1[contador2] = sizeMetadata + sizeMapaBits + (sizeof(MDEntrada) * ED) + (b * 4096);
										contador2++;
										mapi->seton(b);

									}

									if (contador2 == CantBD || contador2 == 16)
									{
										CantBD -= contador2;
										contador2 = 0;
										break;
									}
								}
							}
							file->write(Bloqui->I11->tocharBI1(), Bloqui->I22->I2[contador], 64);
							contador++;
						}

						if (contador == cantbi1 || contador == 32)
						{
							cantbi1 -= contador;
							break;
						}

					}
				}

				file->write(Bloqui->I22->tocharBI2(), Punteros.ptrsBDI[1], 128);

				break;
			}
		}
	}

	// escribir en el Bloque indirecto de 3 nivel
	unsigned int sizeBi3 = sizeMetadata + sizeMapaBits + (sizeof(MDEntrada) * ED) + ((((33308 * ED) * 4096) + (2081 * ED) * 64) + ((65 * ED) * 128));
	int Bd = totalB - 12 - 16 - 512;
	int BI2n = (Bd % 512) == 0 ? (Bd / 512) : (Bd / 512) + 1;
	int Bi1n = (Bd % 16) == 0 ? (Bd / 16) : (Bd / 16) + 1;

	int contadorBi2 = 0;
	int contadorBi1 = 0;
	int contadorBld = 0;


	if (totalB > 540 && totalB < 33308)
	{
		for (unsigned int i = (SBI1 + SBD + SBI2); i < (SBI1 + SBD + SBI2 + SBI3); i++)
		{
			if (mapi->BitDesponible(i))
			{
				Punteros.ptrsBDI[2] = (sizeBi3 + ((i - (SBI1 + SBD + SBI2)) * 256));
				mapi->seton(i);
				Bloqui->I33->fromcharBI3(file->read(Punteros.ptrsBDI[2], 256));
				for (unsigned int a = (SBI1 + SBD); a < (SBI1 + SBD + SBI2); a++) // I2
				{
					if (mapi->BitDesponible(a))
					{
						if (contadorBi2 < BI2n && contadorBi2 < 64) {
							Bloqui->I33->I3[contadorBi2] = (sizeBi2 + ((a - (SBI1 + SBD)) * 128));
							Bloqui->I22->fromcharBI2(file->read(Bloqui->I33->I3[contadorBi2], 128));

							for (unsigned int b = SBD; b < (SBI1 + SBD); b++) // i1
							{
								if (mapi->BitDesponible(b))
								{
									if (contadorBi1 < Bi1n && contadorBi1 < 32)
									{
										Bloqui->I22->I2[contadorBi1] = (sizeB1 + ((b - SBD) * 64));
										Bloqui->I11->fromcharBI1(file->read(Bloqui->I22->I2[contadorBi1], 64));
										for (unsigned int c = 0; c < SBD; c++)
										{
											if (mapi->BitDesponible(c))
											{
												if (contadorBld < 16 && contadorBld < Bd)
												{
													Bloqui->I11->I1[contadorBld] = sizeMetadata + sizeMapaBits + (sizeof(MDEntrada) * ED) + (c * 4096);
													contadorBld++;

												}

												if (contadorBld == 16 || contadorBld == Bd)
												{
													Bd -= contadorBld;
													contadorBld = 0;
													break;
												}


											}

										}
										file->write(Bloqui->I11->tocharBI1(), Bloqui->I22->I2[contadorBi1], 64);
										contadorBi1++;

									}

									if (contadorBi1 == 32 || contadorBi1 == Bi1n)
									{
										Bi1n -= contadorBi1;
										contadorBi1 = 0;
										break;
									}

								}

							}
							file->write(Bloqui->I22->tocharBI2(), Bloqui->I33->I3[contadorBi2], 128);
							contadorBi2++;
						}

						if (contadorBi2 == BI2n || contadorBi2 == 64)
						{
							BI2n -= contadorBi2;
							contadorBi2 = 0;
							break;
						}
					}
				}
				file->write(Bloqui->I33->tocharBI3(), Punteros.ptrsBDI[2], 256);
				//Bloqui->I33->fromcharBI3(file->read(Punteros.ptrsBDI[2], 256));
				break;
			}
		}
	}



	file->write(mapi->toChar(), sizeMetadata, mapi->getsizeof());

	return Punteros;
}

void Entradas::WriteArchivo(MDEntrada _Archivo)
{
	DataFile* file3 = new DataFile(_Archivo.nombreEntrada);

	file3->open("R");

	unsigned int tamanio = SizeArchivo(_Archivo.nombreEntrada);
	unsigned int bloques = tamanio / 4096;
	unsigned int resatente = tamanio % 4096;
	bool hayrestante = false;
	if (resatente != 0)
	{
		hayrestante = true;
		bloques++;
	}


	unsigned int puntero = 0;
	unsigned int completar = tamanio - resatente;

	for (int i = 0; i < 12; i++) {

		if (_Archivo.ptrsBD[i] != 0) {

			if (i < (bloques - 1) || !hayrestante) {
				file->write(file3->read(puntero, 4096), _Archivo.ptrsBD[i], 4096);
				puntero += 4096;
			}
			else
			{
				file->write(file3->read(puntero, resatente), _Archivo.ptrsBD[bloques], resatente);
			}
		}
		else
		{
			break;
		}
	}

	// bloque indirecto de 1 nivel
	if (_Archivo.ptrsBDI[0] != 0)
	{
		Bloqui->I11->fromcharBI1(file->read(_Archivo.ptrsBDI[0], 64));

		for (int a = 0; a < 16; a++)
		{

			if (Bloqui->I11->I1[a] != 0) {

				if (a < (bloques - 12 - 1) || !hayrestante) {
					file->write(file3->read(puntero, 4096), Bloqui->I11->I1[a], 4096);
					puntero += 4096;
				}
				else
				{
					file->write(file3->read(puntero, resatente), Bloqui->I11->I1[a], resatente);
				}


			}
			else
			{
				break;
			}
		}
	}



	//bloque indirecto de segundo nivel 
	// 32 BI1 (Bi1 -  16BD)
	// 512 Bd

	if (_Archivo.ptrsBDI[1] != 0)
	{
		Bloqui->I22->fromcharBI2(file->read(_Archivo.ptrsBDI[1], 128));
		for (int b = 0; b < 32; b++)
		{
			if (Bloqui->I22->I2[b] != 0) {

				Bloqui->I11->fromcharBI1(file->read(Bloqui->I22->I2[b], 64));
				for (int c = 0; c < 16; c++) {

					if (Bloqui->I11->I1[c] != 0) {

						if (c < (bloques - 29 - (b * 16)) || !hayrestante) {
							file->write(file3->read(puntero, 4096), Bloqui->I11->I1[c], 4096);
							puntero += 4096;
						}
						else
						{
							file->write(file3->read(puntero, resatente), Bloqui->I11->I1[c], resatente);
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	if (_Archivo.ptrsBDI[2] != 0)
	{
		Bloqui->I33->fromcharBI3(file->read(_Archivo.ptrsBDI[2], 256));
		for (int e = 0; e < 64; e++)
		{
			if (Bloqui->I33->I3[e] != 0)
			{
				Bloqui->I22->fromcharBI2(file->read(Bloqui->I33->I3[e], 128));
				for (int b = 0; b < 32; b++)
				{
					if (Bloqui->I22->I2[b] != 0) {

						Bloqui->I11->fromcharBI1(file->read(Bloqui->I22->I2[b], 64));
						for (int c = 0; c < 16; c++) {

							if (Bloqui->I11->I1[c] != 0) {

								if (c < (bloques - 541 - (e * 512) - (b * 16)) || !hayrestante) {
									file->write(file3->read(puntero, 4096), Bloqui->I11->I1[c], 4096);
									puntero += 4096;
								}
								else
								{
									file->write(file3->read(puntero, resatente), Bloqui->I11->I1[c], resatente);
								}
							}
							else
							{
								break;
							}
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}

		}

	}



	file3->close();
}

void Entradas::Export(const char* _ArchivoEx)
{
	MDEntrada exportada = Existe(_ArchivoEx);

	if (strcmp(exportada.nombreEntrada, "NotDefined") == 0)
	{
		return;
	}

	DataFile* nuevoF = new DataFile(_ArchivoEx);
	file->open("RW");
	nuevoF->open("W");
	unsigned int tamanio = exportada.tamañoentrada;
	unsigned int bloques = tamanio / 4096;
	unsigned int resatente = tamanio % 4096;
	bool hayrest = false;
	if (resatente != 0)
	{
		hayrest = true;
		bloques++;
	}
	unsigned int puntero = 0;

	// bloques directos
	for (int i = 0; i < 12; i++) {

		if (exportada.ptrsBD[i] != 0) {

			if (i < (bloques - 1) || !hayrest) {

				nuevoF->write(file->read(exportada.ptrsBD[i], 4096), puntero, 4096);
				puntero += 4096;

			}
			else
			{
				nuevoF->write(file->read(exportada.ptrsBD[bloques], resatente), puntero, resatente);
			}
		}
	}

	if (exportada.ptrsBDI[0] != 0)
	{
		Bloqui->I11->fromcharBI1(file->read(exportada.ptrsBDI[0], 64));
		for (int i = 0; i < 16; i++) {

			if (Bloqui->I11->I1[i] != 0) {

				if (i < (bloques - 13) || !hayrest) {
					nuevoF->write(file->read(Bloqui->I11->I1[i], 4096), puntero, 4096);
					puntero += 4096;
				}
				else
				{
					nuevoF->write(file->read(Bloqui->I11->I1[i], resatente), puntero, resatente);
				}
			}
			else
			{
				break;
			}

		}
	}

	//bloque indirecto de segundo nivel
	if (exportada.ptrsBDI[1] != 0)
	{
		Bloqui->I22->fromcharBI2(file->read(exportada.ptrsBDI[1], 128));
		for (int b = 0; b < 32; b++)
		{
			if (Bloqui->I22->I2[b] != 0) {

				Bloqui->I11->fromcharBI1(file->read(Bloqui->I22->I2[b], 64));
				for (int c = 0; c < 16; c++) {

					if (Bloqui->I11->I1[c] != 0) {

						if (c < (bloques - 29 - (b * 16)) || !hayrest) {
							nuevoF->write(file->read(Bloqui->I11->I1[c], 4096), puntero, 4096);
							puntero += 4096;
						}
						else
						{
							nuevoF->write(file->read(Bloqui->I11->I1[c], resatente), puntero, resatente);
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}


	if (exportada.ptrsBDI[2] != 0)
	{
		Bloqui->I33->fromcharBI3(file->read(exportada.ptrsBDI[2], 256));
		for (int e = 0; e < 64; e++)
		{
			if (Bloqui->I33->I3[e] != 0)
			{
				Bloqui->I22->fromcharBI2(file->read(Bloqui->I33->I3[e], 128));
				for (int b = 0; b < 32; b++)
				{
					if (Bloqui->I22->I2[b] != 0) {

						Bloqui->I11->fromcharBI1(file->read(Bloqui->I22->I2[b], 64));
						for (int c = 0; c < 16; c++) {

							if (Bloqui->I11->I1[c] != 0) {

								if (c < (bloques - 541 - (e * 512) - (b * 16)) || !hayrest) {
									nuevoF->write(file->read(Bloqui->I11->I1[c], 4096), puntero, 4096);
									puntero += 4096;
								}
								else
								{
									nuevoF->write(file->read(Bloqui->I11->I1[c], resatente), puntero, resatente);
								}
							}
							else
							{
								break;
							}
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}

		}

	}

	nuevoF->close();

	file->close();


}

void Entradas::Eliminar(const char* _EntradaEliminar)
{

	file->open("RW");

	MDEntrada eliminado;
	unsigned int puntero = sizeMetadata + sizeMapaBits;
	eliminado = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
	puntero += sizeof(MDEntrada);

	for (int i = 0; i < ED; i++)
	{
		if (strcmp(eliminado.nombreEntrada, _EntradaEliminar) == 0) {

			if (eliminado.esArchivo)
			{
				ElemiArchivo(eliminado);
				break;
			}
			else
			{
				ElemiCarpeta(eliminado);
				break;
			}

		}
		eliminado = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
		puntero += sizeof(MDEntrada);
	}


	file->close();
}

void Entradas::ElemiCarpeta(MDEntrada _Carpeta)
{
	MDEntrada inicial;

	MDEntrada eliminado;
	unsigned int puntero = sizeMetadata + sizeMapaBits;
	unsigned int puntero2 = sizeMetadata + sizeMapaBits;
	eliminado = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
	puntero += sizeof(MDEntrada);

	if (strcmp(_Carpeta.nombreEntrada, Ahora.nombreEntrada) == 0)
	{
		cout << "No pueden eleminar esta carpeta ya que estas dentor de ella";
		return;
	}
	else {



		// caso que la carpta no tenga hijos
		if (Ahora.indPrimerhijo == _Carpeta.indice)
		{
			if (Ahora.indice == 0) {
				if (_Carpeta.indPrimerhijo == 0)
				{

					if (_Carpeta.indHermanoDerecho == 0)
					{
						inicial.indice = _Carpeta.indice;
						file->write(reinterpret_cast<char*>(&inicial), (puntero2 + ((inicial.indice - 1) * sizeof(MDEntrada))), sizeof(MDEntrada));
					}
					else
					{
						if (Ahora.indPrimerhijo == _Carpeta.indice) {
							Ahora.indPrimerhijo = _Carpeta.indHermanoDerecho;
							inicial.indice = _Carpeta.indice;
							file->write(reinterpret_cast<char*>(&inicial), (puntero2 + ((inicial.indice - 1) * sizeof(MDEntrada))), sizeof(MDEntrada));
						}
						else
						{
							MDEntrada eliminado2;
							unsigned int puntero = sizeMetadata + sizeMapaBits;
							eliminado2 = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
							puntero += sizeof(MDEntrada);
							for (int i = 0; i < ED; i++)
							{
								if (eliminado2.indPadre == Ahora.indice)
								{
									if (eliminado2.indHermanoDerecho == _Carpeta.indice) {
										eliminado2.indHermanoDerecho = _Carpeta.indHermanoDerecho;
										file->write(reinterpret_cast<char*>(&eliminado2), (puntero2 + ((eliminado2.indice - 1) * sizeof(MDEntrada))), sizeof(MDEntrada));
										break;
									}
								}
							}
							eliminado2 = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
							puntero += sizeof(MDEntrada);
						}

					}
				}
				else
				{
					MDEntrada eliminado2;
					unsigned int puntero = sizeMetadata + sizeMapaBits;
					eliminado2 = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
					puntero += sizeof(MDEntrada);
					for (int i = 0; i < ED; i++)
					{
						if (eliminado2.indPadre == _Carpeta.indice)
						{
							if (eliminado2.esArchivo)
							{
								ElemiArchivo(eliminado2);
							}
							else
							{
								ElemiCarpeta(eliminado2);
							}
						}
					}
					eliminado2 = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
					puntero += sizeof(MDEntrada);
				}
			}
			else
			{
				if (_Carpeta.indPrimerhijo == 0) {
					if (Ahora.indPrimerhijo == _Carpeta.indice)
					{
						Ahora.indPrimerhijo = _Carpeta.indHermanoDerecho;
						inicial.indice = _Carpeta.indice;
						file->write(reinterpret_cast<char*>(&inicial), (puntero + ((inicial.indice - 1) * sizeof(MDEntrada))), sizeof(MDEntrada));
					}

					else {
						for (int i = 0; i < ED; i++)
						{
							if (eliminado.indHermanoDerecho == _Carpeta.indice)
							{
								eliminado.indHermanoDerecho = _Carpeta.indHermanoDerecho;
								file->write(reinterpret_cast<char*>(&eliminado), (puntero + ((eliminado.indice - 1) * sizeof(MDEntrada))), sizeof(MDEntrada));
								inicial.indice = _Carpeta.indice;
								file->write(reinterpret_cast<char*>(&inicial), (puntero + ((inicial.indice - 1) * sizeof(MDEntrada))), sizeof(MDEntrada));

								break;
							}
						}
						eliminado = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
						puntero += sizeof(MDEntrada);
					}
				}
				else
				{
					MDEntrada eliminado2;
					unsigned int puntero = sizeMetadata + sizeMapaBits;
					eliminado2 = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
					puntero += sizeof(MDEntrada);
					for (int i = 0; i < ED; i++)
					{
						if (eliminado2.indPadre == _Carpeta.indice)
						{
							if (eliminado2.esArchivo)
							{
								ElemiArchivo(eliminado2);
							}
							else
							{
								ElemiCarpeta(eliminado2);
							}
						}
					}
					eliminado2 = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
					puntero += sizeof(MDEntrada);
				}
			}

		}


	}


}

void Entradas::ElemiArchivo(MDEntrada _Archivo)
{


	MDEntrada eliminado;
	unsigned int puntero = sizeMetadata + sizeMapaBits;
	eliminado = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
	puntero += sizeof(MDEntrada);


	for (int i = 0; i < ED; i++)
	{
		if (strcmp(eliminado.nombreEntrada, "NotDefined") != 0)
		{
			if (Ahora.indice != 0) {
				if (Ahora.indice == eliminado.indice) {
					if (eliminado.indice == _Archivo.indPadre) {
						// padre - primer hijo
						if (eliminado.indPrimerhijo == _Archivo.indice)
						{

							eliminado.indPrimerhijo == _Archivo.indHermanoDerecho;
							file->write(reinterpret_cast<char*>(&eliminado), (puntero + ((eliminado.indice - 1) * sizeof(MDEntrada))), sizeof(MDEntrada));
							break;

						}
						else if (eliminado.indHermanoDerecho == _Archivo.indice)
						{
							eliminado.indHermanoDerecho == _Archivo.indHermanoDerecho;
							file->write(reinterpret_cast<char*>(&eliminado), (puntero + ((eliminado.indice - 1) * sizeof(MDEntrada))), sizeof(MDEntrada));
							break;

						}

					}
				}
			}
			else
			{
				if (eliminado.indHermanoDerecho == _Archivo.indice)
				{
					eliminado.indHermanoDerecho == _Archivo.indHermanoDerecho;
					file->write(reinterpret_cast<char*>(&eliminado), (puntero + ((eliminado.indice - 1) * sizeof(MDEntrada))), sizeof(MDEntrada));
					break;
				}

			}


		}

		eliminado = fromcharentrada(file->read(puntero, sizeof(MDEntrada)));
		puntero += sizeof(MDEntrada);
	}


	//elemininar punteros
	//Bloques directos
	Bloques* Blqui2 = new Bloques();

	unsigned int mapaon = sizeMapaBits + sizeMetadata + (sizeof(MDEntrada) * ED);
	mapi->fromChar(file->read(sizeMetadata, sizeMapaBits));


	for (int a = 0; a < 12; a++)
	{
		if (_Archivo.ptrsBD[a] != 0)
		{
			file->write(Blqui2->BDD->tocharBD(), _Archivo.ptrsBD[a], 4096);
			mapaon = (_Archivo.ptrsBD[a] - mapaon) / 4096;
			mapi->setoff(mapaon);
			_Archivo.ptrsBD[a] = 0;



		}
		else
		{
			break;
		}
	}

	//BI1{}

	if (_Archivo.ptrsBDI[0] != 0) {
		Blqui2->I11->fromcharBI1(file->read(_Archivo.ptrsBDI[0], 64));
		for (int b = 0; b < 16; b++)
		{
			if (Blqui2->I11->I1[b] != 0)
			{
				file->write(Blqui2->BDD->tocharBD(), Blqui2->I11->I1[b], 4096);
				mapaon = (Blqui2->I11->I1[b] - mapaon) / 4096;
				mapi->setoff(mapaon);
				Blqui2->I11->I1[b] = 0;
			}
			else
			{
				break;
			}
		}

		file->write(Blqui2->I11->tocharBI1(), _Archivo.ptrsBDI[0], 64);
		mapaon += ((ED * 33308) * 4096);
		mapaon = (_Archivo.ptrsBDI[0] - mapaon) / 64;
		mapi->setoff(mapaon);

	}
	_Archivo.ptrsBDI[0] = 0;

	//BI2
	mapaon = sizeMapaBits + sizeMetadata + (sizeof(MDEntrada) * ED);
	if (_Archivo.ptrsBDI[1] != 0) {
		Blqui2->I22->fromcharBI2(file->read(_Archivo.ptrsBDI[1], 128));
		for (int b = 0; b < 32; b++)
		{
			if (Blqui2->I22->I2[b] != 0)
			{
				Blqui2->I11->fromcharBI1(file->read(Blqui2->I22->I2[b], 64));
				for (int b = 0; b < 16; b++)
				{
					if (Blqui2->I11->I1[b] != 0)
					{
						file->write(Blqui2->BDD->tocharBD(), Blqui2->I11->I1[b], 4096);
						mapaon = sizeMapaBits + sizeMetadata + (sizeof(MDEntrada) * ED);
						mapaon = (Blqui2->I11->I1[b] - mapaon) / 4096;
						mapi->setoff(mapaon);
						Blqui2->I11->I1[b] = 0;
					}
					else
					{
						break;
					}
				}
				file->write(Blqui2->I11->tocharBI1(), Blqui2->I22->I2[b], 64);
				mapaon += ((ED * 33308) * 4096);
				mapaon = (Blqui2->I22->I2[b] - mapaon) / 64;
				mapi->setoff(mapaon);
				Blqui2->I22->I2[b] = 0;
			}
			else
			{
				break;
			}
		}
		file->write(Blqui2->I22->tocharBI2(), _Archivo.ptrsBDI[1], 128);
		mapaon = sizeMapaBits + sizeMetadata + (sizeof(MDEntrada) * ED);
		mapaon += ((ED * 33308) * 4096) + ((ED * 2081) * 64);
		mapaon = (_Archivo.ptrsBDI[1] - mapaon) / 128;
		mapi->setoff(mapaon);
	}
	_Archivo.ptrsBDI[1] = 0;

	//BI3
	if (_Archivo.ptrsBDI[2] != 0)
	{
		Blqui2->I33->fromcharBI3(file->read(_Archivo.ptrsBDI[2], 256));
		for (int c = 0; c < 64; c++)
		{
			if (Blqui2->I33->I3[c] != 0)
			{
				Blqui2->I22->fromcharBI2(file->read(Blqui2->I33->I3[c], 128));
				for (int b = 0; b < 32; b++)
				{
					if (Blqui2->I22->I2[b] != 0)
					{
						Blqui2->I11->fromcharBI1(file->read(Blqui2->I22->I2[b], 64));
						for (int b = 0; b < 16; b++)
						{
							if (Blqui2->I11->I1[b] != 0)
							{
								file->write(Blqui2->BDD->tocharBD(), Blqui2->I11->I1[b], 4096);
								mapaon = sizeMapaBits + sizeMetadata + (sizeof(MDEntrada) * ED);
								mapaon = (Blqui2->I11->I1[b] - mapaon) / 4096;
								mapi->setoff(mapaon);
								Blqui2->I11->I1[b] = 0;
							}
							else
							{
								break;
							}
						}
						file->write(Blqui2->I11->tocharBI1(), Blqui2->I22->I2[b], 64);
						mapaon += ((ED * 33308) * 4096);
						mapaon = (Blqui2->I22->I2[b] - mapaon) / 64;
						mapi->setoff(mapaon);
						Blqui2->I22->I2[b] = 0;
					}
					else
					{
						break;
					}
				}
				file->write(Blqui2->I22->tocharBI2(), Blqui2->I33->I3[c], 128);
				mapaon = sizeMapaBits + sizeMetadata + (sizeof(MDEntrada) * ED);
				mapaon += ((ED * 33308) * 4096) + ((ED * 2081) * 64);
				mapaon = (Blqui2->I33->I3[c] - mapaon) / 128;
				mapi->setoff(mapaon);
				Blqui2->I33->I3[c] = 0;
			}
		}
		file->write(Blqui2->I33->tocharBI3(), _Archivo.ptrsBDI[2], 256);
		mapaon = sizeMapaBits + sizeMetadata + (sizeof(MDEntrada) * ED);
		mapaon += ((ED * 33308) * 4096) + ((ED * 2081) * 64) + ((65 * ED) * 128);
		mapaon = (_Archivo.ptrsBDI[2] - mapaon) / 256;
		mapi->setoff(mapaon);
	}
	_Archivo.ptrsBDI[2] = 0;

	MDEntrada a;
	a.indice = _Archivo.indice;
	_Archivo = a;

	file->write(reinterpret_cast<char*>(&_Archivo), (sizeMapaBits + sizeMetadata + ((_Archivo.indice - 1) * 192)), 192);


}

