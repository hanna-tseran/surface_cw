#ifndef SURFACE_H
#define SURFACE_H

#include "stdafx.h"
#include "bezier.h"

class Surface
{
private:
    vector<vector<Patch>> patches;
    vector<vector<HeightMapPatch>> rawHeights;
    int n;
    int m;
    const int PATCH_SIZE = 4;//10;//Stride*N+1//4;
    Patch image;
    bool recalculate;

public:
    Surface();
    Surface(int n, int m);
    void addPatch(Patch patch, int i, int j) ;
    void addHeightMap(HeightMapPatch hmp, int i, int j) ;
    Patch getImage();
    void setNM(int n, int m);
    Patch modifyHeight(glm::vec3);
};

#endif // SURFACE_H
