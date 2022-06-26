#include "Menu.h"
#include <iostream>
#include "APIDisc.h"
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "Entradas.h"

using namespace std;

const char* Menu::CrearDisco(const char* _diskname)
{

	const char* terminacion = ".bin";
	char* s = new char[strlen(_diskname) + strlen(terminacion) + 1];
	strcpy_s(s, strlen(_diskname) + 1, _diskname);
	strcat_s(s, strlen(_diskname) + strlen(terminacion) + 1, terminacion);

	return s;

}

void Menu::MenuPrincipal()
{
	APIDisc _api;
	Entradas _entry;


	//& direccion de memoeria ya que queremos saber donde esta para 
	//el nuevo elemento en esa direccion de memoria

	//* devuelver lo que hay en esa direccion de memoria

	string comand = " ";
	int codigo = 0;
	string opcion = " ";
	string discoopen = " ";
	string ED = " ";
	string directory = "";
	string filenamePath = "";
	string file = "";

	string nombredisk = " ";


	int a = 0;


	bool menu1 = false;
	bool menu2 = false;


	while (comand.compare("$salir") != 0)
	{
		if (menu1 == false) {
			cout << "\n * * * Sala de control * * * " << endl
				<< " Escriba alguno de los sigueintes comandos empezando con un $ " << endl
				<< " createdisk [namedisk] " << endl
				<< " opendisck namedisk " << endl
				<< " salir " << endl
				<< " R/: ";
			cin >> comand;

			if (comand.compare("$createdisk") == 0)
			{
				cin >> nombredisk;
				cin >> ED;
			}
			if (comand.compare("$opendisk") == 0)
			{
				cin >> nombredisk;
			}




		}
		else if (menu1 && !menu2)
		{
			cout << "\n\n * * * Sala de control * * * " << "                                  Directorio actual - " << _entry.Directorio() << endl
				<< "Escriba alguno de los sigueintes comandos empezando con un $ " << endl
				<< " mkdir [namedisk] " << endl
				<< " import [namedisk] " << endl
				<< " export [namedisk] " << endl
				<< " cd [namedisk] " << endl
				<< " rm [namedisk] " << endl
				<< " ls " << endl
				<< " salir " << endl
				<< " R/: ";
			cin >> comand;

			if (comand.compare("$mkdir") == 0)
			{
				cin >> directory;
				//cout << directory;
			}
			else if (comand.compare("$cd") == 0)
			{
				cin >> directory;
			}
			else if (comand.compare("$import") == 0)
			{
				cin >> directory;
			}
			else if (comand.compare("$export") == 0)
			{
				cin >> directory;
			}
			else if (comand.compare("$rm") == 0)
			{
				cin >> directory;
			}
			else if (comand.compare("$salir") == 0)
			{
				break;
			}
			else if (comand.compare("$bs") == 0)
			{
				cin >> directory;
			}
		}


		if (comand.compare("$createdisk") == 0)
		{


			//discoopen = nombre(comand, 1);
			a = stoi(ED, nullptr, 10);

			_api.Nuevodisco(CrearDisco(nombredisk.c_str()));
			_entry.nuevaEntradas(CrearDisco(nombredisk.c_str()), a);

			//cout << nombredisk;
			if (!_api.Existe())
			{


				//cout << a << '\n';


				_api.saveMetadata(nombredisk.c_str(), a);
				_entry.ActualizarAhora(CrearDisco(nombredisk.c_str()), CrearDisco(nombredisk.c_str()));
				_api.readMetadata();
				menu1 = true;
			}
			else
			{
				cout << "\nEste Disco ya existe c:\n";
				cout << "\n Ingrese un nombre diferente porfis c:\n";

			}



		}
		else if (comand.compare("$opendisk") == 0 && menu1 == false)
		{
			DataFile* prueba = new DataFile(CrearDisco(nombredisk.c_str()));

			if (prueba->Existe()) {

				_api.Nuevodisco(CrearDisco(nombredisk.c_str()));


				_entry.nuevaEntradas(CrearDisco(nombredisk.c_str()), _api.CntED());
				//_api.readMetadata();
				_entry.ActualizarAhora(CrearDisco(nombredisk.c_str()), CrearDisco(nombredisk.c_str()));


				menu1 = true;
			}
			else
			{
				cout << "\nEste Disco no existe c:\n";
				cout << "\n Ingrese nombre de disco c:\n";
			}

		}
		else if (comand.compare("$mkdir") == 0)
		{

			_entry.EntradaNueva(directory.c_str(), 0);


		}
		else if (comand.compare("$cd") == 0)
		{
			_entry.ActualizarAhora(directory.c_str(), CrearDisco(nombredisk.c_str()));
		}
		else if (comand.compare("$ls") == 0)
		{
			_entry.hijos();


		}
		else if (comand.compare("$rm") == 0)
		{
			_entry.Eliminar(directory.c_str());

		}

		else if (comand.compare("$import") == 0)
		{
			_entry.EntradaNueva(directory.c_str(), 1);

		}

		else if (comand.compare("$export") == 0)
		{
			_entry.Export(directory.c_str());

		}
		else if (comand.compare("$bs") == 0)
		{
			_entry.Existe(directory.c_str());
		}
		else if (comand.compare("$salir") == 0)
		{
			break;
		}

	}

}

string Menu::comando(string _comando)
{
	vector<string> comand{};
	char delimiter = ' ';

	istringstream sstream(_comando);
	string word;
	while (std::getline(sstream, word, delimiter)) {
		word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
		comand.push_back(word);
	}


	const auto& s = comand.begin();
	string con = *s;

	return con;
}

string Menu::nombre(string _text, int _posicion)
{

	vector<string> comand{};
	char delimiter = ' ';

	istringstream sstream(_text);
	string word;
	while (std::getline(sstream, word, delimiter)) {
		word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
		comand.push_back(word);
	}


	const auto& s = comand.at(_posicion);
	string con = s;

	return con;
}

Menu::Menu()
{
}
