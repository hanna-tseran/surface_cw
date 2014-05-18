#include "bezier.h"


int Bezier::getStride() const
{
    return STRIDE;
}

//void Bezier::setSTRIDE(int value)
//{
//    STRIDE = value;
//}

//void Bezier::setSTRIDE(int value)
//{
//    STRIDE = value;
//}

int Bezier::getN() const
{
    return N;
}
Bezier::Bezier()
{
}

void Bezier::setCtrlPoints(GLfloat ctrlPoints[4][4][3]) {
    for (int i = 0; i <= N; ++i) {
        vector<glm::vec3> row;
        row.resize(N+1);
        this->ctrlPoints.push_back(row);
    }
    for (int i = 0; i <= N; ++i) {
        for (int j = 0; j <= N; ++j) {
            this->ctrlPoints[i][j].x = float(ctrlPoints[i][j][0]);
            this->ctrlPoints[i][j].y = float(ctrlPoints[i][j][1]);
            this->ctrlPoints[i][j].z = float(ctrlPoints[i][j][2]);
        }
    }
}


void Bezier::setHeight(HeightMapPatch height) {
    for (int i = 0; i <= N; ++i) {
        vector<int> row;
        row.resize(N+1);
        this->height.push_back(row);
    }
    for (int i = 0; i <= N; ++i) {
        for (int j = 0; j <= N; ++j) {
            this->height[i][j] = height[i][j];
        }
    }
}

//double Bezier::bernstein(double t, int i) {
//    return  C3[i] * glm::pow(t, double(i)) * glm::pow((1-t), double(N - i));
//}

double Bezier::bernstein(double t, vector<GLfloat> &ctrl) {
    double result = 0;
    for (int i = 0; i <= N; ++i) {
        result += ctrl[i] * C3[i] * glm::pow(t, double(i)) * glm::pow((1-t), double(N - i));
    }
    return  result;
}

Patch Bezier::generate() {
    vector< vector<float> > height;
    vector<float> ctrl;
    vector<float> preCalc;
    float curX;
    float curY;

    for (int i = 0; i <= N * STRIDE; ++i) {
        vector<float> row;
        row.resize((N+1) * STRIDE);
        height.push_back(row);
    }
    ctrl.resize(N+1);
    preCalc.resize(N+1);

    for (int v = 0; v <= N * STRIDE; ++v) {
        curY = float(v) / float(N * STRIDE);
        for (int u = 0; u <= N * STRIDE; ++u) {
            curX = float(u) / float(N * STRIDE);

            for (int i = 0; i <= N; ++i) {
                for (int j = 0; j <= N; ++j) {
                    ctrl[j] = ctrlPoints[i][j].z;
                }
                preCalc[i] = bernstein(curY, ctrl);
            }
            height[v][u] = bernstein(curX, preCalc);
        }
    }

    return getPatch(height);
}

Patch Bezier::generateByHeight() {
    vector< vector<float> > newHeight;
    vector<float> ctrl;
    vector<float> preCalc;
    float curX;
    float curY;

    for (int i = 0; i <= N * STRIDE; ++i) {
        vector<float> row;
        row.resize((N+1) * STRIDE);
        newHeight.push_back(row);
    }
    ctrl.resize(N+1);
    preCalc.resize(N+1);

    for (int v = 0; v <= N * STRIDE; ++v) {
        curX = float(v) / float(N * STRIDE);
        for (int u = 0; u <= N * STRIDE; ++u) {
            curY = float(u) / float(N * STRIDE);

            for (int i = 0; i <= N; ++i) {
                for (int j = 0; j <= N; ++j) {
                    ctrl[j] = height[i][j];
                }
                preCalc[i] = bernstein(curY, ctrl);
            }
            newHeight[v][u] = bernstein(curX, preCalc);
        }
    }
//    vector< vector<float> > newHeight;
//    vector<float> ctrl;
//    vector<float> preCalc;
//    float curX;
//    float curY;

//    for (int i = 0; i <= N * STRIDE; ++i) {
//        vector<float> row;
//        row.resize((N+1) * STRIDE);
//        newHeight.push_back(row);
//    }
//    ctrl.resize(N+1);
//    preCalc.resize(N+1);

//    for (int v = 0; v <= N * STRIDE; ++v) {
//        curY = float(v) / float(N * STRIDE);
//        for (int u = 0; u <= N * STRIDE; ++u) {
//            curX = float(u) / float(N * STRIDE);

//            for (int i = 0; i <= N; ++i) {
//                for (int j = 0; j <= N; ++j) {
//                    ctrl[j] = height[i][j];
//                }
//                preCalc[i] = bernstein(curY, ctrl);
//            }
//            newHeight[v][u] = bernstein(curX, preCalc);
//        }
//    }

    return getPatch(newHeight);
}


//int Bezier::getSTRIDE() const
//{
//    return STRIDE;
//}
Patch Bezier::getPatch(vector< vector<float> > &height) {
    Patch patch;
    int i = 0;
    int j = 0;
    for (i = 0; i <= N * STRIDE; ++i) {
        vector<glm::vec3> row;
        row.resize((N + 1) * STRIDE);
        patch.push_back(row);
    }

    for (i = 0; i < N; ++i) {
        float curY = i;
        float nextY = i+1;
        for (int k = 0; k < STRIDE; ++k) {
            float newY = curY + k * (nextY - curY) / STRIDE;
            for (j = 0; j <= N * STRIDE; ++j) {
                patch[i * STRIDE + k][j].y = newY;
            }
        }
    }
    for (j = 0; j <= N * STRIDE; ++j) {
        patch[i * STRIDE][j].y = i;
    }

    for (j = 0; j < N; ++j) {
        float curX = j;
        float nextX = j+1;
        for (int k = 0; k < STRIDE; ++k) {
            float newX = curX + k * (nextX - curX) / STRIDE;
            for (i = 0; i <= N * STRIDE; ++i) {
                patch[i][j * STRIDE + k].x = newX;
            }
        }
    }
    for (i = 0; i <= N * STRIDE; ++i) {
        patch[i][j * STRIDE].x = j;
    }

    for (i = 0; i <= N * STRIDE; ++i) {
        for (j = 0; j<= N * STRIDE; ++j) {
            patch[i][j].z = height[i][j];
        }
    }

    return patch;
}

//Patch Bezier::getPatch(vector< vector<float> > &height) {
//    Patch patch;
//    int i = 0;
//    int j = 0;
//    for (i = 0; i <= N * STRIDE; ++i) {
//        vector<glm::vec3> row;
//        row.resize((N + 1) * STRIDE);
//        patch.push_back(row);
//    }

//    for (i = 0; i < N; ++i) {
//        float curY = ctrlPoints[i][0][1];
//        float nextY = ctrlPoints[i+1][0][1];
//        for (int k = 0; k < STRIDE; ++k) {
//            float newY = curY + k * (nextY - curY) / STRIDE;
//            for (j = 0; j <= N * STRIDE; ++j) {
//                patch[i * STRIDE + k][j].y = newY;
//            }
//        }
//    }
//    for (j = 0; j <= N * STRIDE; ++j) {
//        patch[i * STRIDE][j].y = ctrlPoints[i][0][1];
//    }

//    for (j = 0; j < N; ++j) {
//        float curX = ctrlPoints[0][j][0];
//        float nextX = ctrlPoints[0][j+1][0];
//        for (int k = 0; k < STRIDE; ++k) {
//            float newX = curX + k * (nextX - curX) / STRIDE;
//            for (i = 0; i <= N * STRIDE; ++i) {
//                patch[i][j * STRIDE + k].x = newX;
//            }
//        }
//    }
//    for (i = 0; i <= N * STRIDE; ++i) {
//        patch[i][j * STRIDE].x = ctrlPoints[0][j][0];
//    }

//    for (i = 0; i <= N * STRIDE; ++i) {
//        for (j = 0; j<= N * STRIDE; ++j) {
//            patch[i][j].z = height[i][j];
//        }
//    }

//    return patch;
//}
