#include "DataStructures.h"

void GDS::setLayer(unsigned short layerNum) {
    if (this->currentElement == 0) {
        // current element: Box
        Structure &s = this->structures[this->structures.size() - 1];
        Box &b = s.boxes[s.boxes.size() - 1];
        b.layer = layerNum;
    }
}


Structure &GDS::getLastStructure() {
    Structure &s = this->structures[this->structures.size() - 1];
    return s;
}

Box &GDS::getLastBox() {
    Structure& s = getLastStructure();
    Box& b = s.boxes[s.boxes.size() - 1];
    return b;
}
