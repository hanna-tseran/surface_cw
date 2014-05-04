#include "surface.h"

Surface::Surface()
{
}

Surface::Surface(int n, int m) : n(n), m(m)
{
    for (int i = 0; i < n; ++i) {
        vector<Patch> row;
        row.resize(m);
        patches.push_back(row);
    }
}


void Surface::addPatch(Patch patch, int i, int j) {
    patches[i][j] = patch;
}

Patch Surface::getImage() {
    if(image.size() != 0) {
        return image;
    }

    for (int i = 0; i < n * (PATCH_SIZE - 1) + 1; ++i) {
        vector<glm::vec3> row;
        row.resize(m * (PATCH_SIZE - 1) + 1);
        image.push_back(row);
    }

    for (int iSurf = 0; iSurf < n; ++iSurf) {
        for (int jSurf = 0; jSurf < m; ++jSurf) {
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
