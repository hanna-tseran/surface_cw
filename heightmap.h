#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "stdafx.h"

class HeightMap
{
public:
    HeightMap();
    vector< vector<int> > LoadRawFile(string fileName, int mapSize);
};

#endif // HEIGHTMAP_H
