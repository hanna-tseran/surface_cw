#ifndef BEZIER_H
#define BEZIER_H

#include "stdafx.h"

class Bezier
{

private:
    const double C3[4] = {1.0, 3.0, 3.0, 1.0};
    const int N = 3;
    const int U_STRIDE = 1;//3;
    const int V_STRIDE = 1;//3;//12;

    vector< vector<glm::vec3> > ctrlPoints;
    vector< vector<int> > height;
    int uStride;
    int vStride;

    Patch getPatch(vector< vector<float> > &height);

public:
    Bezier();

    void setCtrlPoints(GLfloat ctrlPoints[4][4][3]);
    void setHeight(vector< vector<int> > height);
    double bernstein(double t, vector<GLfloat> &ctrl);
    Patch generate();
    Patch generateByHeight();

    int getUStride() const;
//    void setUStride(int value);
    int getVStride() const;
//    void setVStride(int value);
    int getN() const;
};

#endif // BEZIER_H
