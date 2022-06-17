#ifndef GDSFEATUREEXTRACTOR_DATASTRUCTURES_H
#define GDSFEATUREEXTRACTOR_DATASTRUCTURES_H

#include <iostream>
#include <cstdlib>

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <climits>
#include <cfloat>
#include <cassert>


#include "gdsFileParser.h"

using namespace std;

class Coordinate {
public:
    double x = INT_MAX, y = INT_MAX;
};

class Box {
public:
    unsigned short layer = USHRT_MAX;
    unsigned short type = USHRT_MAX;
    vector<Coordinate> coordinates;
};

class Text {
public:
    unsigned short layer = USHRT_MAX;
    unsigned short type = USHRT_MAX;
    vector<Coordinate> coordinates;
    string textString;
};

class Sref {
public:
    string name;
    short Strans = SHRT_MAX;
    double angle = DBL_MAX;
    unsigned short type = USHRT_MAX;
    vector<Coordinate> coordinates;

};

class Boundary {
public:
    unsigned short layer = USHRT_MAX;
    unsigned short type = USHRT_MAX;
    vector<Coordinate> coordinates;

};

class Path {
public:
    unsigned short layer = USHRT_MAX;
    unsigned short type = USHRT_MAX;
    double width = DBL_MAX;
    vector<Coordinate> coordinates;
    vector<unsigned short> extension;
};

class Structure {
public:
    string name;
    vector<Box> boxes;
    vector<Text> texts;
    vector<Sref> srefs;
    vector<Boundary> boundaries;
    vector<Path> paths;
    unordered_map<string, size_t> srefName_map, textName_map;

};
class Circuit{
public:
    long int cellCount = 0;
    int combinationalRatio = 0;
    int ioPadCount = 0;
    int averageFanIn = 0;
    int averageBoundingBox = 0;
    int averageTopologicalOrder = 0;

    int x_min = INT_MAX, y_min = INT_MAX, x_max = -INT_MAX, y_max = -INT_MAX;
    vector<vector<Path>> connectedPaths;

};
class GDS : public gdsfp::gdsFileParser {
private:
    // box: 0, boundary: 1, path: 2, text: 3, Sref: 4, end element and not assiged: -1
    int currentElement = INT_MAX;
    string libName;
    double userUnit = DBL_MAX, dbUnit = DBL_MAX;
    unsigned short version = SHRT_MAX;
    vector<Structure> structures;
    Circuit circuit;
    // Features


    void setLayer(unsigned short layerNum);

    Structure &getLastStructure();

    Box &getLastBox();

    Text &getLastText();

    Sref &getLastSref();

    Boundary &getLastBoundary();

    Path &getLastPath();


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
        Boundary boundary;
        Structure &s = this->getLastStructure();
        s.boundaries.push_back(boundary);
    };

    void onParsedPathStart() override {
        cout << "onParsedPathStart" << endl;
        this->currentElement = 2;
        Structure &s = this->getLastStructure();
        Path path;
        s.paths.push_back(path);
    };

    void onParsedBoxStart() override {
        cout << "onParsedBoxStart" << endl;
        this->currentElement = 0;
        Box box;
        Structure &s = getLastStructure();
        s.boxes.push_back(box);
    };

    void onParsedEndElement() override {
        cout << "onParsedEndElement" << endl;
        this->currentElement = -1;
    };

    void onParsedEndStructure() override {
        cout << "onParsedEndStructure" << endl;
    };

    void onParsedEndLib() override {
        cout << "onParsedEndLib" << endl;
    };

    void onParsedColumnsRows(unsigned short columns,
                             unsigned short rows) override {
        cout << "Columns: " << columns << " Rows: " << rows << endl;
    };

    void onParsedPathType(unsigned short pathType) override {
        cout << "onParsedPathType" << endl;
        assert(this->currentElement == 2);
        Path &p = this->getLastPath();
        p.type = pathType;
    };

    void onParsedStrans(short strans) override {
        cout << "onParsedStrans" << endl;
        Sref &sref = this->getLastSref();
        sref.Strans = strans;
    };

    void onParsedPresentation(short font, short valign,
                              short halign) override {
        cout << "Font: " << font << endl;
        cout << "Valign: " << valign << endl;
        cout << "Halign: " << halign << endl;
    };

    //
    void onParsedNodeStart() override {
        cout << "Node start" << endl;
    };

    void onParsedTextStart() override {
        cout << "onParsedTextStart" << endl;
        this->currentElement = 3;
        Text t;
        Structure &s = getLastStructure();
        s.texts.push_back(t);
    };

    void onParsedSrefStart() override {
        cout << "onParsedSrefStart" << endl;
        this->currentElement = 4;
        Sref sref;
        Structure &s = this->getLastStructure();
        s.srefs.push_back(sref);
    };

    //
    void onParsedArefStart() override {
        cout << "Aref start" << endl;
    };

    void onParsedSname(const char *sname) override {
        cout << "onParsedSname" << endl;
        assert(this->currentElement == 4);
        Sref &sref = this->getLastSref();
        sref.name = sname;

        Structure &s = this->getLastStructure();
        s.srefName_map[sname] = s.srefs.size()-1;
    };

    void onParsedString(const char *str) override {
        cout << "onParsedString" << endl;
        if (this->currentElement == 3) {
            Text &text = this->getLastText();
            text.textString = str;

            Structure &s = this->getLastStructure();
            s.textName_map[str] = s.texts.size() - 1;
        } else {
            // any case?
            assert(0);
        }
    };

    //
    void onParsedPropValue(const char *propValue) override {
        cout << "Prop Value: " << propValue << endl;
    };

    void onParsedXY(int count, int x[], int y[]) override {
        cout << "onParsedXY" << endl;

        for (int i = 0; i < count; ++i) {
            Coordinate coordinate;
            coordinate.x = ((double) x[i]) * this->userUnit;
            coordinate.y = ((double) y[i]) * this->userUnit;

            if (this->currentElement == 0) {
                // if current element is Box
                Box &box = getLastBox();
                box.coordinates.push_back(coordinate);
            } else if (this->currentElement == 1) {
                // if current element is Boundary
                Boundary &boundary = this->getLastBoundary();
                boundary.coordinates.push_back(coordinate);
            } else if (this->currentElement == 2) {
                // if current element is path
                Path &path = getLastPath();
                path.coordinates.push_back(coordinate);
            } else if (this->currentElement == 3) {
                // if current element is Text
                Text &t = this->getLastText();
                t.coordinates.push_back(coordinate);
            } else if (this->currentElement == 4) {
                // if current element is Sref
                Sref &sref = this->getLastSref();
                sref.coordinates.push_back(coordinate);
            } else {
                // any case?
                assert(0);
            }
        }
    };

    void onParsedLayer(unsigned short layer) override {
        cout << "onParsedLayer " << endl;
        this->setLayer(layer);
    };

    void onParsedWidth(int width) override {
        cout << "onParsedWidth" << endl;
        if (this->currentElement == 2) {
            // current element: path
            Path &path = this->getLastPath();
            path.width = width * this->userUnit;
        } else {
            // any case?
            assert(0);
        }
    };

    void onParsedDataType(unsigned short dataType) override {
        cout << "onParsedDataType" << endl;
        if (this->currentElement == 1) {
            // current element: boundary
            Boundary &boundary = this->getLastBoundary();
            boundary.type = dataType;
        } else if (this->currentElement == 2) {
            Text& text = this->getLastText();
            text.type = dataType;
        } else if (this->currentElement == 3) {
            // current element: Sref
            Sref &sref = this->getLastSref();
            sref.type = dataType;
        } else {
            // any case?
            assert(0);
        }
    };

    void onParsedTextType(unsigned short textType) override {
        cout << "onParsedTextType" << endl;
        Text &text = this->getLastText();
        text.type = textType;
    };

    void onParsedAngle(double angle) override {
        cout << "onParsedAngle" << endl;
        Sref &sref = this->getLastSref();
        sref.angle = angle;
    };

    void onParsedMag(double mag) override {
        cout << "Mag: " << mag << endl;
    };

    void onParsedBeginExtension(unsigned short bext) override {
        cout << "onParsedBeginExtension" << endl;
        Path &path = this->getLastPath();
        path.extension.push_back(bext);
    };

    void onParsedEndExtension(unsigned short eext) override {
        cout << "onParsedEndExtension" << endl;
        assert(this->currentElement == 2);
        Path &path = this->getLastPath();
        path.extension.push_back(eext);
    };

    //
    void onParsedPropertyNumber(unsigned short propNum) override {
        cout << "Property Number: " << propNum << endl;
    };

    //
    void onParsedNodeType(unsigned short nodeType) override {
        cout << "Node Type: " << nodeType << endl;
    };

    void onParsedBoxType(unsigned short boxType) override {
        cout << "onParsedBoxType " << endl;
        Box &b = getLastBox();
        b.type = boxType;
    };

public:
    void extractFeatures();

    void getCellCount();

    void connectPaths();

    void getDieSize();

    void getIOpads();
};


#endif //GDSFEATUREEXTRACTOR_DATASTRUCTURES_H
