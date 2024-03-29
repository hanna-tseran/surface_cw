#ifndef BEZIER_H
#define BEZIER_H

#include "stdafx.h"

class Bezier
{

private:
    const double C3[4] = {1.0, 3.0, 3.0, 1.0};
    const int N = 3;
    const int STRIDE = 1;

    vector< vector<glm::vec3> > ctrlPoints;
    vector< vector<int> > height;

    Patch getPatch(vector< vector<float> > &height);

public:
    Bezier();

    void setCtrlPoints(GLfloat ctrlPoints[4][4][3]);
    void setHeight(HeightMapPatch height);
    double bernstein(double t, vector<GLfloat> &ctrl);
    Patch generate();
    Patch generateByHeight();



    int getN() const;
    int getStride() const;
};

#endif // BEZIER_H
