#include "DataFile.h"

#include "DataFile.h"



DataFile::DataFile() : path(nullptr)
{

}
DataFile::DataFile(const char* _path) : path(_path)
{
}
DataFile::~DataFile()
{
    delete path;
    file.close();
}
void DataFile::open(const char* _openFormat) //openformat R: lectura W: escritura RW: lectura y escritura
{
    //std::cout << this->path;
    if (strcmp(_openFormat, "R") == 0)
        openMode = std::ios_base::in;
    else if (strcmp(_openFormat, "W") == 0)
        openMode = std::ios_base::out;
    else if (strcmp(_openFormat, "RW") == 0)
        openMode = std::ios_base::in | std::ios_base::out;


    file.open(path, openMode | std::ios::binary);

    if (!file)
    {
        std::cout << this->path;
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }
}
void DataFile::close()
{
    file.close();
}
void DataFile::write(char* _data, unsigned int _position, unsigned int _length)
{
    file.seekp(_position, std::ios::beg);
    // std::cout << std::endl <<file.tellp() << std::endl;
    file.write(_data, _length);
}

char* DataFile::read(unsigned int _position, unsigned int _length)
{
    char* dataReaded = new char[_length];

    file.seekg(_position, std::ios::beg);
    file.read(dataReaded, _length);

    return dataReaded;
}



bool DataFile::isEof()
{
    return file.eof();
}

bool DataFile::Existe()
{
    if (this->path == nullptr)
    {
        //std::cout << "Este archivo no existe";
        return false;
    }
    else {

        file.open(this->path, std::ios_base::in | std::ios::binary);

        if (!file.is_open())
        {
            // std::cout << "Este archivo no existe";
            return false;
        }
    }

    file.close();

    return true;
}

long DataFile::tellp()
{
    return file.tellp();
}

long DataFile::tellg()
{
    return file.tellg();
}

unsigned int DataFile::sizeArchivo()
{
    file.seekg(0, std::ios::end);
    return file.tellg();
}

