/*
 * MIT License
 *
 * Copyright (c) 2017 EDDR Software, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Changes:
 * 2017-01-01: First & Last Name: What you did.
 * 2017-06-10: Kevin Nesmith: Initial contribution.
 *
 */

#include "GDS.h"
#include "gdsCalmaRecords.h"
#include "DataStructures.h"
#include <math.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

namespace GDS {
    unsigned long p_256[8] = {0x1,
                              0x100,
                              0x10000,
                              0x1000000,
                              0x100000000,
                              0x10000000000,
                              0x1000000000000,
                              0x100000000000000
    };

    short GDS::readShort(std::ifstream *input) {
        unsigned char p[2];
        input->read((char *) &p, sizeof(p));
        return ((p[0] << 8) | p[1]);
    }

    short GDS::readShort(stringstream *input) {
        unsigned char p[2];
        input->read((char *) &p, sizeof(p));
        return ((p[0] << 8) | p[1]);
    }

    double GDS::readDouble(stringstream *input) {
        short neg = 1;
        unsigned char p[8];
        int exp = 0;
        double man = 0.0;
        input->read((char *) &p, sizeof(p));

        if (p[0] > 127) {
            neg = -1;
            exp = p[0] - 192;
        } else {
            exp = p[0] - 64;
        }

        for (int i = 1; i < 8; ++i) {
            man += (double) p[i] / (double) p_256[i];
        }

        return man * pow(16, exp) * neg;
    }

    void GDS::readString(stringstream *input, string *str) {
        string temp = input->str();

        for (string::iterator it = temp.begin(); it != temp.end(); ++it) {
            if ((*it) < 32 || (*it) > 127) { // We only want viewable characters.
                continue;
            }

            (*str) += (*it);
        }
    }

    int GDS::readInt(stringstream *input) {
        unsigned char p[4];
        input->read((char *) &p[0], sizeof(p));
        return ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
    }

    unsigned int GDS::readUInt(stringstream *input) {
        char p[4];
        input->read((char *) &p[0], sizeof(p));
        return ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
    }

    void GDS::readTimeStamp(stringstream *input, short *year,
                            short *month, short *day, short *hour,
                            short *minute, short *sec) {
        (*year) = readShort(input);
        (*month) = readShort(input);
        (*day) = readShort(input);
        (*hour) = readShort(input);
        (*minute) = readShort(input);
        (*sec) = readShort(input);

        if ((*year) < 1000) {
            (*year) += 1900;
        }
    }

    void GDS::readHeader(stringstream *input) {
        short gdsVersion = readShort(input);
        onParsedGDSVersion(gdsVersion);
        this->version = gdsVersion;
    }

    void GDS::readModTimeStamp(stringstream *input) {
        short year, month, day, hour, minute, sec;
        readTimeStamp(input, &year, &month, &day, &hour, &minute, &sec);
        onParsedModTime(year, month, day, hour, minute, sec);
    }

    void GDS::readAccessTimeStamp(stringstream *input) {
        short year, month, day, hour, minute, sec;
        readTimeStamp(input, &year, &month, &day, &hour, &minute, &sec);
        onParsedAccessTime(year, month, day, hour, minute, sec);
    }

    void GDS::readLibName(stringstream *input) {
        std::string libname;
        readString(input, &libname);
        onParsedLibName(libname.c_str());
        this->libName = libname;
    }

    void GDS::readUnits(stringstream *input) {
        double uu = readDouble(input);
        double db = readDouble(input);
        onParsedUnits(uu, db);
        this->userUnit = uu;
        this->dbUnit = db;
    }

    void GDS::readStrName(stringstream *input) {
        string strName;
        readString(input, &strName);
        onParsedStrName(strName.c_str());

        Structure structure;
        structure.name = strName;
        this->structures.push_back(structure);
    }

    void GDS::readBoundary(stringstream *input) {
        onParsedBoundaryStart();
    }

    void GDS::readPath(stringstream *input) {
        onParsedPathStart();
    }

    void GDS::readBox(stringstream *input) {
        onParsedBoxStart();
        Box box;
        this->structures[structures.size() - 1].boxes.push_back(box);
        this->currentElement = 0;
    }

    void GDS::readEndElement(stringstream *input) {
        onParsedEndElement();
    }

    void GDS::readEndStructure(stringstream *input) {
        onParsedEndStructure();
    }

    void GDS::readEndLib(stringstream *input) {
        onParsedEndLib();
    }

    void GDS::readColumnRow(stringstream *input) {
        unsigned short column, row;
        column = readShort(input);
        row = readShort(input);
        onParsedColumnsRows(column, row);
    }

    void GDS::readPathType(stringstream *input) {
        unsigned short path;
        path = readShort(input);
        onParsedPathType(path);
    }

    void GDS::readStrans(stringstream *input) {
        short strans;
        strans = readShort(input);
        onParsedStrans(strans);
    }

    void GDS::readPresentation(stringstream *input) {
        short present;
        present = readShort(input);
        short font, valign, halign;
        font = (present & (0x10 | 0x20 | 0x30));
        valign = (present & (0x04 | 0x08));
        halign = (present & (0x01 | 0x02));
        onParsedPresentation(font, valign, halign);
    }

    void GDS::readNode(stringstream *input) {
        onParsedNodeStart();
    }

    void GDS::readText(stringstream *input) {
        onParsedTextStart();
    }

    void GDS::readSref(stringstream *input) {
        onParsedSrefStart();
    }

    void GDS::readAref(stringstream *input) {
        onParsedArefStart();
    }

    void GDS::readSname(stringstream *input) {
        string sname;
        readString(input, &sname);
        onParsedSname(sname.c_str());
    }

    void GDS::readString(stringstream *input) {
        string str;
        readString(input, &str);
        onParsedString(str.c_str());
    }

    void GDS::readPropValue(stringstream *input) {
        string prop;
        readString(input, &prop);
        onParsedPropValue(prop.c_str());
    }

    void GDS::readXY(stringstream *input) {
        input->seekp(0, ios::end);
        unsigned int length = input->tellp();
        int count = length / 8;
        int x[count];
        int y[count];

        for (int i = 0; i < count; ++i) {
            x[i] = readInt(input);
            y[i] = readInt(input);
        }

        onParsedXY(count, x, y);
    }

    void GDS::readLayer(stringstream *input) {
        unsigned short layer;
        layer = readShort(input);
        onParsedLayer(layer);
        this->setLayer(layer);
    }

    void GDS::readWidth(stringstream *input) {
        int width;
        width = readInt(input);
        onParsedWidth(width);
    }

    void GDS::readDataType(stringstream *input) {
        unsigned short dataType;
        dataType = readShort(input);
        onParsedDataType(dataType);
    }

    void GDS::readTextType(stringstream *input) {
        unsigned short textType;
        textType = readShort(input);
        onParsedTextType(textType);
    }

    void GDS::readAngle(stringstream *input) {
        double angle;
        angle = readDouble(input);
        onParsedAngle(angle);
    }

    void GDS::readMag(stringstream *input) {
        double mag;
        mag = readDouble(input);
        onParsedMag(mag);
    }

    void GDS::readBeginExtension(stringstream *input) {
        unsigned int bext;
        bext = readInt(input);
        onParsedBeginExtension(bext);
    }

    void GDS::readEndExtension(stringstream *input) {
        unsigned int eext;
        eext = readInt(input);
        onParsedEndExtension(eext);
    }

    void GDS::readPropertyNumber(stringstream *input) {
        unsigned short propNum;
        propNum = readShort(input);
        onParsedPropertyNumber(propNum);
    }

    void GDS::readNodeType(stringstream *input) {
        unsigned short nodeType;
        nodeType = readShort(input);
        onParsedNodeType(nodeType);
    }

    void GDS::readBoxType(stringstream *input) {
        unsigned short boxType;
        boxType = readShort(input);
        onParsedBoxType(boxType);
        Structure &s = structures[structures.size() - 1];
        s.boxes[s.boxes.size() - 1].type = boxType;
    }

    void GDS::parseBuffer(stringstream *input) {
        char recType, dataType;
        input->read(&recType, sizeof(recType));
        input->read(&dataType, sizeof(dataType));

        switch (recType) {
            case HEADER:
                readHeader(input);
                break;

            case BGNLIB:
                readModTimeStamp(input);
                readAccessTimeStamp(input);
                break;

            case LIBNAME:
                readLibName(input);
                break;

            case BGNSTR:
                readModTimeStamp(input);
                readAccessTimeStamp(input);
                break;

            case UNITS:
                readUnits(input);
                break;

            case STRNAME:
                readStrName(input);
                break;

            case BOUNDARY:
                readBoundary(input);
                break;

            case PATH:
                readPath(input);
                break;

            case ENDEL:
                readEndElement(input);
                break;

            case ENDSTR:
                readEndStructure(input);
                break;

            case ENDLIB:
                readEndLib(input);
                break;

            case COLROW:
                readColumnRow(input);
                break;

            case PATHTYPE:
                readPathType(input);
                break;

            case STRANS:
                readStrans(input);
                break;

            case PRESENTATION:
                readPresentation(input);
                break;

            case TEXT:
                readText(input);
                break;

            case SREF:
                readSref(input);
                break;

            case AREF:
                readAref(input);
                break;

            case SNAME:
                readSname(input);
                break;

            case STRING:
                readString(input);
                break;

            case PROPVALUE:
                readPropValue(input);
                break;

            case XY:
                readXY(input);
                break;

            case LAYER:
                readLayer(input);
                break;

            case WIDTH:
                readWidth(input);
                break;

            case DATATYPE:
                readDataType(input);
                break;

            case TEXTTYPE:
                readTextType(input);
                break;

            case ANGLE:
                readAngle(input);
                break;

            case MAG:
                readMag(input);
                break;

            case BGNEXTN:
                readBeginExtension(input);
                break;

            case ENDEXTN:
                readEndExtension(input);
                break;

            case PROPATTR:
                readPropertyNumber(input);
                break;

            case NODE:
                readNode(input);
                break;

            case NODETYPE:
                readNodeType(input);
                break;

            case BOX:
                readBox(input);
                break;

            case BOXTYPE:
                readBoxType(input);
                break;

            default:
                break;
        }
    }

    int GDS::parse(const char *filePath) {
        std::ifstream gdsFile(filePath, ios::in | ios::binary);

        if (gdsFile.is_open()) {
            stringstream stream(ios::in | ios::out | ios::binary);
            unsigned int total = 0;

            do {
                stream.str("");
                unsigned short length = readShort(&gdsFile);

                if (length == 0) {
                    break;  // We have reached the end of the file.
                }

                short sub = sizeof(length);
                char buffer[length - sub];
                gdsFile.read((char *) &buffer, sizeof(buffer));
                total += length;
                stream.write(buffer, length - sub);
                parseBuffer(&stream);
            } while (gdsFile.good());
        } else {
            cerr << "Error: something is wrong with the file." << endl;
            return 1;
        }

        return 0;
    }


    /* Additional Functions for Feature extraction*/

    void GDS::setLayer(short layer) {
        if (this->currentElement == 0){
            Structure &s = this->structures[this->structures.size() - 1];
            Box &b = s.boxes[s.boxes.size() - 1];
            b.layer = layer;
        }
    }
} // End namespace GDS
