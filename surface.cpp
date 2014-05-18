#include "surface.h"
void Surface::setNM(int n, int m) {
    this->n = n;
    this->m = m;
    for (int i = 0; i < n; ++i) {
        vector<Patch> row;
        row.resize(m);
        patches.push_back(row);

        vector<HeightMapPatch> heightRow;
        heightRow.resize(m);
        rawHeights.push_back(heightRow);
    }
}

Surface::Surface()
{
    recalculate = false;
}

Surface::Surface(int n, int m) : n(n), m(m){
    recalculate = false;
    for (int i = 0; i < n; ++i) {
        vector<Patch> row;
        row.resize(m);
        patches.push_back(row);

        vector<HeightMapPatch> heightRow;
        heightRow.resize(m);
        rawHeights.push_back(heightRow);
    }
}


void Surface::addPatch(Patch patch, int i, int j) {
    patches[i][j] = patch;
}

void Surface::addHeightMap(HeightMapPatch hmp, int i, int j) {
    rawHeights[i][j] = hmp;
}

Patch Surface::getImage() {
    if(image.size() != 0 && !recalculate) {
        return image;
    }

    for (int i = 0; i < n * (PATCH_SIZE - 1) + 1; ++i) {
        vector<glm::vec3> row;
        row.resize(m * (PATCH_SIZE - 1) + 1);
        image.push_back(row);
    }

    for (int iSurf = 0; iSurf < n; ++iSurf) {
        for (int jSurf = 0; jSurf < m; ++jSurf) {
            if(iSurf == 5 && jSurf == 7 ) {
                int a = 5;
            }
            for (int i = 0; i < PATCH_SIZE; ++i) {
                for (int j = 0; j < PATCH_SIZE; ++j) {
                    image[iSurf*(PATCH_SIZE-1) + i][jSurf*(PATCH_SIZE-1) + j] = patches[iSurf][jSurf][i][j];

                    image[iSurf*(PATCH_SIZE-1) + i][jSurf*(PATCH_SIZE-1) + j].x += jSurf * 3;//ACTUAL_PATCH_SIZE(DEGREE) - 1
                    image[iSurf*(PATCH_SIZE-1) + i][jSurf*(PATCH_SIZE-1) + j].y += iSurf * 3;


//                    image[iSurf*(PATCH_SIZE-1) + i][jSurf*(PATCH_SIZE-1) + j].x = i*PATCH_SIZE + patches[iSurf][jSurf][i][j].x;
//                    image[iSurf*(PATCH_SIZE-1) + i][jSurf*(PATCH_SIZE-1) + j].y = j*PATCH_SIZE + patches[iSurf][jSurf][i][j].y;
//                    image[iSurf*(PATCH_SIZE-1) + i][jSurf*(PATCH_SIZE-1) + j].z = patches[iSurf][jSurf][i][j].z;


                    if (iSurf > 0 && jSurf > 0) {
                        if (i == 0 && j == 0) {
                            float sum = 0.0;
                            sum += patches[iSurf][jSurf][i][j].z;
                            sum += patches[iSurf - 1][jSurf][PATCH_SIZE - 1][j].z;
                            sum += patches[iSurf][jSurf - 1][i][PATCH_SIZE - 1].z;
                            sum += patches[iSurf - 1][jSurf - 1][PATCH_SIZE - 1][PATCH_SIZE - 1].z;
                            image[iSurf*(PATCH_SIZE-1) + i][jSurf*(PATCH_SIZE-1) + j].z = sum / 4.0;
                            continue;
                        }
                        if (i == 0 && j != 0) {
                            float sum = 0.0;
                            sum += patches[iSurf][jSurf][i][j].z;
                            sum += patches[iSurf][jSurf - 1][i][PATCH_SIZE - 1].z;
                            image[iSurf*(PATCH_SIZE-1) + i][jSurf*(PATCH_SIZE-1) + j].z = sum / 2.0;
                             continue;
                        }
                        if (i != 0 && j == 0) {
                            float sum = 0.0;
                            sum += patches[iSurf][jSurf][i][j].z;
                            sum += patches[iSurf - 1][jSurf][PATCH_SIZE - 1][j].z;
                            image[iSurf*(PATCH_SIZE-1) + i][jSurf*(PATCH_SIZE-1) + j].z = sum / 2.0;
                             continue;
                        }
                    }
                }
            }
        }
    }
    return image;
}

Patch Surface::modifyHeight(glm::vec3 point, int radius) {
    int xCoord = 0;
    int yCoord = 0;
    int zDiff = point.z;

    ///////////////////////
    ///very stupid!!!

    ////Important!!!
    if (image[0][0].x > point.x || image[0][image[0].size()-1].x < point.x) {
        return getImage();
    }

    //find x
    while (xCoord < image[0].size()-1 && !(image[0][xCoord].x <= point.x && image[0][xCoord+1].x > point.x)) {
        ++xCoord;
    }

    ////Important!!!
    if (image[0][0].y > point.y || image[image[0].size()-1][0].y < point.y) {
        return getImage();
    }

    //find y
    while (yCoord < image.size() && !(image[yCoord][0].y <= point.y && image[yCoord+1][0].y > point.y)) {
        ++yCoord;
    }

    ///very stupid!!!
    ///////////////////////

    //find patches and indexies in them
    int xPatch = xCoord / (PATCH_SIZE - 1);
    int xInPatch;
    if (xPatch > patches[0].size()) {
        --xPatch;
        xInPatch = PATCH_SIZE - 1;
    }
    else {
        xInPatch = xCoord % (PATCH_SIZE - 1);
    }

    int yPatch = yCoord / (PATCH_SIZE - 1);
    int yInPatch;
    if (yPatch > patches.size()) {
        --yPatch;
        yInPatch = PATCH_SIZE - 1;
    }
    else {
        yInPatch = yCoord % (PATCH_SIZE - 1);
    }

    int iGl0 = fmax(0, xPatch-radius);
    int iGln = fmin(patches.size(), xPatch + radius);
    int jGl0 = fmax(0, yPatch-radius);
    int jGln = fmin(patches[0].size(), yPatch + radius);
    for (int iGl = iGl0; iGl < iGln; ++iGl) {
        for (int jGl = jGl0; jGl < jGln; ++jGl) {
            float iCoef = fmin((iGl-iGl0), (iGln - iGl));
            float jCoef = fmin((jGl-jGl0), (jGln - jGl));
            for (int i = 0; i < PATCH_SIZE - 1; ++i) {
                for (int j = 0; j < PATCH_SIZE - 1; ++j) {
//                    float iCoef = (1+fmin((iGl-iGl0) /PATCH_SIZE, (PATCH_SIZE - (iGl-iGl0)) /PATCH_SIZE));
//                    float jCoef = (1+fmin((jGl-jGl0) /PATCH_SIZE, (PATCH_SIZE - (jGl-jGl0)) /PATCH_SIZE));
                    //float coef =  (1+fmin(iGl /PATCH_SIZE, (PATCH_SIZE - iGl)) /PATCH_SIZE)* (1+fmin(jGl /PATCH_SIZE, (PATCH_SIZE - jGl) /PATCH_SIZE));
                    rawHeights[iGl][jGl][i][j] += zDiff + 0.3*fmin(iCoef, jCoef);
                    if (i == 0 && (iGl-iGl0 > 0 || iGl == iGl0 && iGl > 0)) {
                        rawHeights[iGl - 1][jGl][PATCH_SIZE - 1][j] += zDiff + 0.3*fmin(iCoef, jCoef);
                    }
                    if (j == 0 && (jGl-jGl0 > 0 || jGl == jGl0 && jGl > 0)) {
                        rawHeights[iGl][jGl - 1][i][PATCH_SIZE - 1] += zDiff + 0.3*fmin(iCoef, jCoef);
                    }
                }
            }
            Bezier bezierPatch;
            bezierPatch.setHeight(rawHeights[iGl][jGl]);
            patches[iGl][jGl] = bezierPatch.generateByHeight();
        }
    }

//    xPatch = 5;
//    yPatch = 7;
//    xInPatch = 1;
//    yInPatch = 2;
//    zDiff = 83;

    //HeightMapPatch oldHMP = rawHeights[xPatch][yPatch];
//    rawHeights[xPatch][yPatch][xInPatch][yInPatch] += zDiff;
//    Bezier bezierPatch;
//    bezierPatch.setHeight(rawHeights[xPatch][yPatch]);
//    //HeightMapPatch newHMP = rawHeights[xPatch][yPatch];
//    //Patch oldP = patches[xPatch][yPatch];
//    //Patch newP = bezierPatch.generateByHeight();
//    patches[xPatch][yPatch] = bezierPatch.generateByHeight();

    ///////////////////////
    ///very stupid!!!
    recalculate = true;
    return getImage();
    ///very stupid!!!
    ///////////////////////
}
