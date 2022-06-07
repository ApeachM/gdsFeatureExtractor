#ifndef GDSFEATUREEXTRACTOR_DATASTRUCTURES_H
#define GDSFEATUREEXTRACTOR_DATASTRUCTURES_H

#include <iostream>
#include <cstdlib>

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <iomanip>

#include <vector>
#include <climits>
#include <cfloat>

#include "gdsFileParser.h"

using namespace std;

class Coordinate {
public:
    int x = INT_MAX, y = INT_MAX;
};

class Box {
public:
    unsigned short layer = USHRT_MAX;
    unsigned short type = USHRT_MAX;
    vector<Coordinate> coordinates;
};

class Structure {
public:
    string name;
    vector<Box> boxes;
};

class GDS : public gdsfp::gdsFileParser {
private:
    int currentElement = INT_MAX;  // 0: box, 1: boundary, 2: path, -1: end element and not assiged
    string libName;
    double userUnit = DBL_MAX, dbUnit = DBL_MAX;
    unsigned short version = SHRT_MAX;
    vector<Structure> structures;

    void setLayer(unsigned short layerNum);

    Structure &getLastStructure();

    Box &getLastBox();

protected:
    void onParsedGDSVersion(unsigned short gdsVersion) override {
        cout << "onParsedGDSVersion" << endl;
        this->version = gdsVersion;
    };

    void onParsedModTime(short year, short month, short day,
                         short hour, short minute, short sec) override {
        cout << "onParsedModTime" << endl;
    };

    void onParsedAccessTime(short year, short month, short day,
                            short hour, short minute, short sec) override {
        cout << "onParsedAccessTime" << endl;
    };

    void onParsedLibName(const char *libname) override {
        cout << "onParsedLibName" << endl;
        this->libName = libname;
    };

    void onParsedUnits(double userUnits, double dbUnits) override {
        cout << "onParsedUnits" << endl;
        this->userUnit = userUnits;
        this->dbUnit = dbUnits;
    };

    void onParsedStrName(const char *strName) override {
        cout << "onParsedStrName" << endl;
        Structure s;
        s.name = strName;
        this->structures.push_back(s);
    };

    void onParsedBoundaryStart() override {
        cout << "onParsedBoundaryStart" << endl;
        this->currentElement = 1;
    };

    void onParsedPathStart() override {
        cout << "onParsedPathStart" << endl;
        this->currentElement = 2;
    };

    void onParsedBoxStart() override {
        cout << "onParsedBoxStart" << endl;
        this->currentElement = 0;
        Box b;
        Structure &s = getLastStructure();
        s.boxes.push_back(b);
    };

    void onParsedEndElement() override {
        cout << "onParsedEndElement" << endl;
        this->currentElement = -1;
    };

    void onParsedEndStructure() override {
        cout << "Structure end" << endl;
    };

    void onParsedEndLib() override {
        cout << "Lib end" << endl;
    };

    void onParsedColumnsRows(unsigned short columns,
                             unsigned short rows) override {
        cout << "Columns: " << columns << " Rows: " << rows << endl;
    };

    void onParsedPathType(unsigned short pathType) override {
        cout << "PathType: " << pathType << endl;
    };

    void onParsedStrans(short strans) override {
        cout << "Strans: " << strans << endl;
    };

    void onParsedPresentation(short font, short valign,
                              short halign) override {
        cout << "Font: " << font << endl;
        cout << "Valign: " << valign << endl;
        cout << "Halign: " << halign << endl;
    };

    void onParsedNodeStart() override {
        cout << "Node start" << endl;
    };

    void onParsedTextStart() override {
        cout << "Text start" << endl;
    };

    void onParsedSrefStart() override {
        cout << "Sref start" << endl;
    };

    void onParsedArefStart() override {
        cout << "Aref start" << endl;
    };

    void onParsedSname(const char *sname) override {
        cout << "Sname: " << sname << endl;
    };

    void onParsedString(const char *str) override {
        cout << "String: " << str << endl;
    };

    void onParsedPropValue(const char *propValue) override {
        cout << "Prop Value: " << propValue << endl;
    };

    void onParsedXY(int count, int x[], int y[]) override {
        cout << "onParsedXY" << endl;

        for (int i = 0; i < count; ++i) {
            Coordinate coordinate;
            coordinate.x = x[i];
            coordinate.y = y[i];

            if (this->currentElement == 0) {
                // if current element is Box
                Box &b = getLastBox();
                b.coordinates.push_back(coordinate);
            }
        }
    };

    void onParsedLayer(unsigned short layer) override {
        cout << "onParsedLayer " << endl;
        this->setLayer(layer);

    };

    void onParsedWidth(int width) override {
        cout << "Width: " << width << endl;
    };

    void onParsedDataType(unsigned short dataType) override {
        cout << "Data Type: " << dataType << endl;
    };

    void onParsedTextType(unsigned short textType) override {
        cout << "Text Type: " << textType << endl;
    };

    void onParsedAngle(double angle) override {
        cout << "Angle: " << angle << endl;
    };

    void onParsedMag(double mag) override {
        cout << "Mag: " << mag << endl;
    };

    void onParsedBeginExtension(unsigned short bext) override {
        cout << "Begin Extension: " << bext << endl;
    };

    void onParsedEndExtension(unsigned short eext) override {
        cout << "End Extension: " << eext << endl;
    };

    void onParsedPropertyNumber(unsigned short propNum) override {
        cout << "Property Number: " << propNum << endl;
    };

    void onParsedNodeType(unsigned short nodeType) override {
        cout << "Node Type: " << nodeType << endl;
    };

    void onParsedBoxType(unsigned short boxType) override {
        cout << "onParsedBoxType " << endl;
        Box &b = getLastBox();
        b.type = boxType;
    };

};


#endif //GDSFEATUREEXTRACTOR_DATASTRUCTURES_H
