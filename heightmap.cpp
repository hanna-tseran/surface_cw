#include "heightmap.h"

HeightMap::HeightMap()
{
}

vector< vector<int> > HeightMap::LoadRawFile(string fileName, int mapSize) {
    fileName = "test.bmp";
    vector <vector<int> > heightMap;
    QImage image(fileName.c_str());
    if ( !image.isNull() )
    {
      const unsigned char* pHeightMap = (const unsigned char*)image.bits();
      //int tempMapSize = 250;
      for (int i = 0; i < mapSize; ++i) {
          vector<int> row;
          row.resize(mapSize);
          for (int j = 0; j < mapSize; ++j) {
              row[j] = (unsigned int)(pHeightMap[i + (j * mapSize)]);
          }
          heightMap.push_back(row);
      }
    }


//    fileName = "heightmap.bmp";
//    vector <vector<int> > heightMap;
//    int fileSize = mapSize * mapSize;
//    BYTE* pHeightMap;
//    FILE* pFile = NULL;
//    pFile = fopen(fileName.c_str(), "rb");
//    if ( pFile != NULL ) {
//        fread( pHeightMap, 1, fileSize, pFile );
//        int result = ferror( pFile );
//        if (result) {
//            int tempMapSize = 16;
//            for (int i = 0; i < tempMapSize; ++i) {
//                vector<int> row;
//                row.resize(tempMapSize);
//                for (int j = 0; j < tempMapSize; ++j) {
//                    row[j] = pHeightMap[i + (j * mapSize)];
//                }
//                heightMap.push_back(row);
//            }
//        }
//        fclose(pFile);
//    }
    return heightMap;
}
