#include "bmp.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

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

std::ofstream& operator<<(
    std::ofstream& out,
    ByteString& bytes
) {
    for (const auto& byte : bytes) {
        out << static_cast<char>(byte);
    }
    return out;
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

    _colorTableSize = _header.dataOffset - 0x36;
    _colorTable = new ColorTableItem[_colorTableSize / 4];
    fin.read(reinterpret_cast<char*>(_colorTable), _colorTableSize);

    fin >> _pixelData;
}

BMP::~BMP() {
    delete[] _colorTable;
}

ByteString BMP::get_raw_string() const {
    std::basic_stringstream<Byte> rawStrStream;
    rawStrStream.write(reinterpret_cast<const Byte*>(&_header), 14);
    rawStrStream.write(reinterpret_cast<const Byte*>(&_infoHeader), 40);
    rawStrStream.write(reinterpret_cast<const Byte*>(_colorTable), _colorTableSize);
    rawStrStream << _pixelData;
    return rawStrStream.str();
}
