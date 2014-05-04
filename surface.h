#ifndef SURFACE_H
#define SURFACE_H

#include "stdafx.h"

class Surface
{
private:
    vector<vector<Patch>> patches;
    int n;
    int m;
    const int PATCH_SIZE = 4;//10;//Stride*N+1//4;
    Patch image;

public:
    Surface();
    Surface(int n, int m);
    void addPatch(Patch patch, int i, int j) ;
    Patch getImage();
};

#endif // SURFACE_H
