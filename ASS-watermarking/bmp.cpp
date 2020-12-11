#include "bmp.hpp"

#include <iostream>
#include <fstream>

// read till eof
std::ifstream& operator>>(
    std::ifstream& in,
    ByteString& bytes
) {
    while (!in.eof()) {
        bytes += static_cast<Byte>(in.get());
    }
    return in;
}

BMP::BMP(const std::string& bmpFilePath) :
    _header {},
    _infoHeader {},
    _colorTable {nullptr},
    _pixelData {},
    _numColors {0}
{
    std::ifstream fin { bmpFilePath };
    fin.read(reinterpret_cast<char*>(&_header), 14);

    fin.read(reinterpret_cast<char*>(&_infoHeader), 40);
    _numColors = 1U << _infoHeader.bitsPerPixel;

    size_t colorTableSize { _header.dataOffset - 0x36 };
    _colorTable = new ColorTableItem[colorTableSize / 4];
    fin.read(reinterpret_cast<char*>(_colorTable), colorTableSize);

    fin >> _pixelData;
}

BMP::~BMP() {
    delete[] _colorTable;
}
