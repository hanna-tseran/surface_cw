#include "heightmap.h"

HeightMap::HeightMap()
{
}

vector< vector<int> > HeightMap::LoadRawFile(string fileName, int mapSize) {
    fileName = "heightmap.raw";
    vector <vector<int> > heightMap;
    int fileSize = mapSize * mapSize;
    BYTE* pHeightMap;
    FILE* pFile = NULL;
    pFile = fopen(fileName.c_str(), "rb");
    if ( pFile != NULL ) {
        fread( pHeightMap, 1, fileSize, pFile );
        int result = ferror( pFile );
        if (result) {
            mapSize = 4;
            for (int i = 0; i < mapSize; ++i) {
                vector<int> row;
                row.resize(mapSize);
                for (int j = 0; j < mapSize; ++j) {
                    row[j] = pHeightMap[i + (j * mapSize)];
                }
                heightMap.push_back(row);
            }
        }
        fclose(pFile);
    }
    return heightMap;
}
