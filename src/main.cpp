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

#include <iostream>
#include <iomanip>

#include "GDS.h"

using namespace std;

#define SET_2W setfill('0') << setw(2)

class MyTestGDS : public GDS::GDS {
protected:
    void onParsedGDSVersion(unsigned short version) override {
        cout << "GDSII Version: " << version << endl;
    };

    void onParsedModTime(short year, short month, short day,
                                 short hour, short minute, short sec) override {
        cout << "Modified Time: " << endl;

        if (year == 0 && month == 0) {
            cout << "\tNot recorded." << endl;
        } else {
            cout << "\t" << year << "-" << SET_2W << month << "-" << SET_2W <<
                 day << " " << SET_2W << hour << ":" << SET_2W << minute <<
                 ":" << SET_2W << sec << endl;
        }
    };

    void onParsedAccessTime(short year, short month, short day,
                                    short hour, short minute, short sec) override {
        cout << "Accessed Time: " << endl;

        if (year == 0 && month == 0) {
            cout << "\tNot recorded." << endl;
        } else {
            cout << "\t" << year << "-" << SET_2W << month << "-" << SET_2W <<
                 day << " " << SET_2W << hour << ":" << SET_2W << minute <<
                 ":" << SET_2W << sec << endl;
        }
    };

    void onParsedLibName(const char *libName) override {
        cout << "LibName: " << libName << endl;
    };

    void onParsedUnits(double userUnits, double dbUnits) override {
        cout << "UserUnits: " << setprecision(9) << fixed << userUnits << endl;
        cout << "DBUnits: " << setprecision(9) << fixed << dbUnits << endl;
    };

    void onParsedStrName(const char *strName) override {
        cout << "StrName: " << strName << endl;
    };

    void onParsedBoundaryStart() override {
        cout << "Boundry start" << endl;
    };

    void onParsedPathStart() override {
        cout << "Path start" << endl;
    };

    void onParsedBoxStart() override {
        cout << "Box start" << endl;
    };

    void onParsedEndElement() override {
        cout << "Element end" << endl;
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
        cout << "XY: " << count << endl;

        for (int i = 0; i < count; ++i) {
            cout << "(" << x[i] << "," << y[i] << ")";
        }

        cout << endl;
    };

    void onParsedLayer(unsigned short layer) override {
        cout << "Layer: " << layer << endl;
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
        cout << "Box Type: " << boxType << endl;
    };
};


// ****************************************************************************
// main()
//
// This is the top level function that tests the parser.
// ****************************************************************************
int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Missing GDSII file as the only parameter." << endl;
        cerr << "Usage: ./testParser /path/to/file.gds" << endl;
        return 1;
    }

    MyTestGDS gds;
    return gds.parse(argv[1]);
}

