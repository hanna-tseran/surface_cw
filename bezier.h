#ifndef BEZIER_H
#define BEZIER_H

#include <QGLWidget>
#include <glm.hpp>
#include <vector>
using std::vector;

typedef vector<vector<glm::vec3>> Image;

class Bezier
{

private:
    const double C3[4] = {1.0, 3.0, 3.0, 1.0};
    const int N = 3;
    const int U_STRIDE = 3;
    const int V_STRIDE = 3;//12;

    vector< vector<glm::vec3> > ctrlPoints;
    int uStride;
    int vStride;

    Image getImage(vector< vector<float> > &height);

public:
    Bezier();

    void setCtrlPoints(GLfloat ctrlPoints[4][4][3]);
    double bernstein(double t, vector<GLfloat> &ctrl);
    Image generate();

    int getUStride() const;
//    void setUStride(int value);
    int getVStride() const;
//    void setVStride(int value);
    int getN() const;
};

#endif // BEZIER_H
