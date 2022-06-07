#ifndef GDSFEATUREEXTRACTOR_DATASTRUCTURES_H
#define GDSFEATUREEXTRACTOR_DATASTRUCTURES_H

#include <iostream>
#include <vector>

using namespace std;

class Box {
public:
    short layer = 0;
    short type = 0;
};

class Structure {
public:
    string name;
    vector<Box> boxes;
};


#endif //GDSFEATUREEXTRACTOR_DATASTRUCTURES_H
