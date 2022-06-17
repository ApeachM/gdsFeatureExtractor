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
    } else if (this->currentElement == 2) {
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
    this->getIOpads();
}

void GDS::getCellCount() {
    for (const auto &structure: this->structures) {
        for (int j = 0; j < structure.srefs.size(); ++j) {
            this->circuit.cellCount += this->circuit.cellCount + 1;
        }
    }
}

void GDS::getDieSize() {
    // See the Boxes, and get min, max coordinates
    for (const auto& s : this->structures) {
        for (const auto& box : s.boxes) {
            for (auto coordinate : box.coordinates) {
                if (coordinate.x < this->circuit.x_min) {
                    this->circuit.x_min = coordinate.x;
                }
                if (coordinate.x > this->circuit.x_max) {
                    this->circuit.x_max = coordinate.x;
                }
                if (coordinate.y < this->circuit.y_min) {
                    this->circuit.y_min = coordinate.y;
                }
                if (coordinate.y > this->circuit.y_max) {
                    this->circuit.y_max = coordinate.y;
                }
            }
        }
    }
};


void GDS::getIOpads() {
    this->getDieSize();
    vector<Path*> ioPaths;
    for (int k = 0; k < this->structures.size(); ++k) {
        Structure& structure = this->structures[k];
        for (int i = 0; i < structure.paths.size(); ++i) {
            Path& path = structure.paths[i];
            for (int j = 0; j < path.coordinates.size(); ++j) {
                Coordinate coordinate = path.coordinates[j];
                if (coordinate.x <= this->circuit.x_min) {
                    ioPaths.push_back(&path);
                } else if (coordinate.x >= this->circuit.x_max){
                    ioPaths.push_back(&path);
                }
                if (coordinate.y <= this->circuit.y_min) {
                    ioPaths.push_back(&path);
                } else if (coordinate.y >= this->circuit.y_max) {
                    ioPaths.push_back(&path);
                }
            }
        }
    }
    this->circuit.ioPadCount = ioPaths.size();
}


void GDS::connectPaths() {

}
