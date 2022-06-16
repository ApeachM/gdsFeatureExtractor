#include "DataStructures.h"

void GDS::setLayer(unsigned short layerNum) {
    Structure &s = this->getLastStructure();
    if (this->currentElement == 0) {
        // current element: Box
        Box &b = s.boxes[s.boxes.size() - 1];
        b.layer = layerNum;
    } else if (this->currentElement == 1) {
        Boundary &boundary = s.boundaries[s.boundaries.size() - 1];
        boundary.layer = layerNum;
    } else if (this->currentElement == 2){
        // current element: Path
        Path &path = s.paths[s.paths.size() - 1];
        path.layer = layerNum;
    } else if (this->currentElement == 3) {
        // current element: Text
        Text &t = s.texts[s.texts.size() - 1];
        t.layer = layerNum;
    } else {
        // any case?
        assert(0);
    }
}


Structure &GDS::getLastStructure() {
    Structure &s = this->structures[this->structures.size() - 1];
    return s;
}

Box &GDS::getLastBox() {
    Structure &s = this->getLastStructure();
    Box &b = s.boxes[s.boxes.size() - 1];
    return b;
}

Text &GDS::getLastText() {
    Structure &s = this->getLastStructure();
    Text &t = s.texts[s.texts.size() - 1];
    return t;
}

Sref &GDS::getLastSref() {
    Structure &s = this->getLastStructure();
    Sref &sref = s.srefs[s.srefs.size() - 1];
    return sref;
}

Boundary &GDS::getLastBoundary() {
    Structure &s = this->getLastStructure();
    Boundary &boundary = s.boundaries[s.boundaries.size() - 1];
    return boundary;
}

Path &GDS::getLastPath() {
    Structure &s = this->getLastStructure();
    Path &p = s.paths[s.paths.size() - 1];
    return p;
}

void GDS::extractFeatures() {
    this->getCellCount();

}

void GDS::getCellCount() {
    for (const auto& structure : this->structures) {
        for (int j = 0; j < structure.srefs.size(); ++j) {
            this->cellCount += this->cellCount + 1;
        }
    }
}
