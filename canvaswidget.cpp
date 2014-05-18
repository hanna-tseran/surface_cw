#include "canvaswidget.h"

CanvasWidget::CanvasWidget(QWidget *parent) :
    QGLWidget(parent),
    timer(new QTimer(this)),
    colors{Color (0.996f, 0.0f, 0.996f),
           Color (0.831f, 0.0f, 0.996f),
           Color (0.666f, 0.0f, 0.996f),
           Color (0.498f, 0.0f, 0.996f),
           Color (0.333f, 0.0f, 0.996f),
           Color (0.164f, 0.0f, 0.996f),
           Color (0.0f, 0.0f, 0.996f),
           Color (0.0f, 0.164f, 0.996f),
           Color (0.0f, 0.329f, 0.996f),
           Color (0.0f, 0.498f, 0.996f),
           Color (0.0f, 0.662f, 0.996f),
           Color (0.0f, 0.831f, 0.996f),
           Color (0.0f, 0.996f, 0.996f),
           Color (0.0f, 0.996f, 0.831f),
           Color (0.0f, 0.996f, 0.666f),
           Color (0.0f, 0.996f, 0.498f),
           Color (0.0f, 0.996f, 0.333f),
           Color (0.0f, 0.996f, 0.164f),
           Color (0.0f, 0.996f, 0.0f),
           Color (0.164f, 0.996f, 0.0f),
           Color (0.329f, 0.996f, 0.0f),
           Color (0.498f, 0.996f, 0.0f),
           Color (0.662f, 0.996f, 0.0f),
           Color (0.831f, 0.996f, 0.0f),
           Color (0.996f, 0.996f, 0.0f),
           Color (0.996f, 0.831f, 0.0f),
           Color (0.996f, 0.666f, 0.0f),
           Color (0.996f, 0.498f, 0.0f),
           Color (0.996f, 0.333f, 0.0f),
           Color (0.996f, 0.164f, 0.0f),
           Color (0.996f, 0.0f, 0.0f)
        /*Color(0.294f, 0.0f, 0.51f), Color(0.0f, 0.0f, 1.0f), Color(0.165f, 0.0f, 1.0f), Color(0.0f, 0.667f, 1.0f),
           Color(0.0f, 0.788f, 0.341f), Color(0.0f, 1.0f, 0.251f), Color(0.333f, 1.0f, 0.0f),
           Color(0.678f, 1.0f, 0.184f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 0.835f, 0.0f),
           Color(1.0f, 0.647f, 0.0f), Color(1.0f, 0.416f, 0.0f), Color(1.0f, 0.0f, 0.0f)*/}
{
    timer->start(EXPIRATION_TIME);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    useArcBall = false;
    modifyHeight = false;
    xCurAngle = 0.0;
    yCurAngle = 0.0;
    xRotAngle = 0.0;
    yRotAngle = 0.0;
    curScale = 1.0;

    temp_total = 0;

    HeightMap hm;
    vector< vector<int> > heightMap = hm.LoadRawFile(MAP_NAME, MAP_SIZE);
    //surface = Surface(PATCHES_NUM, PATCHES_NUM);
    surface.setNM(PATCHES_NUM, PATCHES_NUM);
    int iCoef = 0;
    int jCoef = 0;
    for (int i = 0; i < PATCHES_NUM; ++i) {
        jCoef = 0;
        for (int j = 0; j < PATCHES_NUM; ++j) {
            HeightMapPatch patchHeightMap;

            for (int iP = 0; iP < PATCH_SIZE; ++iP) {
                vector<float> row;
                row.resize(PATCH_SIZE);
                for (int jP = 0; jP < PATCH_SIZE; ++jP) {
                    row[jP] = heightMap[i*PATCH_SIZE + iP + iCoef][j*PATCH_SIZE + jP + jCoef];
                }
                patchHeightMap.push_back(row);
            }
            bezierPatch.setHeight(patchHeightMap);
            surface.addHeightMap(patchHeightMap, i, j);
            surface.addPatch(bezierPatch.generateByHeight(), i, j);
            --jCoef;
        }
        --iCoef;
    }
    surfaceImage = surface.getImage();
}

CanvasWidget::~CanvasWidget() {
    delete timer;
}


void CanvasWidget::paintGL(void)
{
    int i = 0;
    int j = 0;

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // glLoadIdentity();
    //glColor3f(0.0, 0.0, 0.0);
    //glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix ();
    glScalef(curScale, curScale, curScale);
    glRotatef(glm::degrees(xCurAngle), 0.0, 1.0, 0.0);
    glRotatef(glm::degrees(yCurAngle), 1.0, 0.0, 0.0);
    int color;

    for (i = 0; i <= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); i += SURFACE_STRIDE) {
        glBegin(GL_LINE_STRIP);
        for (j = 0; j<= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); j += SURFACE_STRIDE) {
            color = GLfloat(surfaceImage[i][j].z) / 255.0f * colors.size();
            glColor3f(colors[color].red, colors[color].green, colors[color].blue);
            glVertex3f(surfaceImage[i][j].x + xCenter - MAP_SIZE/2, surfaceImage[i][j].y + yCenter - MAP_SIZE/2, surfaceImage[i][j].z);
        }
        glEnd();
    }
    for (j = 0; j <= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); j += SURFACE_STRIDE) {
        glBegin(GL_LINE_STRIP);
        for (i = 0; i<= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); i += SURFACE_STRIDE) {
            color = GLfloat(surfaceImage[i][j].z) / 255.0f * colors.size();
            glColor3f(colors[color].red, colors[color].green, colors[color].blue);
            glVertex3f(surfaceImage[i][j].x + xCenter - MAP_SIZE/2, surfaceImage[i][j].y + yCenter - MAP_SIZE/2, surfaceImage[i][j].z);
        }
        glEnd();
    }
    //glTranslatef(0, 0, 0);

//    for (i = 0; i <= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); i += SURFACE_STRIDE) {
//        glBegin(GL_LINE_STRIP);
//        for (j = 0; j<= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); j += SURFACE_STRIDE) {
//            glVertex3f(surfaceImage[i][j].x, surfaceImage[i][j].y, surfaceImage[i][j].z);
//        }
//        glEnd();
//    }
//    for (j = 0; j <= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); j += SURFACE_STRIDE) {
//        glBegin(GL_LINE_STRIP);
//        for (i = 0; i<= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); i += SURFACE_STRIDE) {
//            glVertex3f(surfaceImage[i][j].x, surfaceImage[i][j].y, surfaceImage[i][j].z);
//        }
//        glEnd();
//    }



    //glTranslatef(3*width/4, 3*height/4, 0);
    //glTranslatef(-width/4, -height/4, 0);
   // glTranslatef(-width, -height, 0);

//    glColor3f(1.0, 0.0, 0.0);

//    for(int i = 0; i < selectedPatches.size(); ++i) {
//        int x = selectedPatches[i].x();
//        int y = selectedPatches[i].y();
//        if (x >= 0 && x < surfaceImage.size() && y >= 0 && y < surfaceImage[0].size()) {

//            glBegin(GL_QUADS);
//            glVertex3f(surfaceImage[x][y].x, surfaceImage[x][y].y, surfaceImage[x][y].z);
//            glVertex3f(surfaceImage[x + SURFACE_STRIDE][y].x,
//                    surfaceImage[x+SURFACE_STRIDE][y].y, surfaceImage[x+SURFACE_STRIDE][y].z);
//            glVertex3f(surfaceImage[x+SURFACE_STRIDE][y+SURFACE_STRIDE].x,
//                    surfaceImage[x+SURFACE_STRIDE][y+SURFACE_STRIDE].y,
//                    surfaceImage[x+SURFACE_STRIDE][y+SURFACE_STRIDE].z);
//            glVertex3f(surfaceImage[x][y+SURFACE_STRIDE].x,
//                    surfaceImage[x][y+SURFACE_STRIDE].y, surfaceImage[x][y+SURFACE_STRIDE].z);
//            glEnd();
//        }
//    }

    glPopMatrix ();
    glFlush();
}

void CanvasWidget::initializeGL(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
}

void CanvasWidget::resizeGL(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        glOrtho(-MAP_SIZE ,
                       MAP_SIZE*(GLfloat)w/(GLfloat)h, -MAP_SIZE, MAP_SIZE,
                       -10*fmax(1000.0, MAP_SIZE + 50.0), 10*fmax(1000.0, MAP_SIZE + 50.0));
//        glOrtho(-MAP_SIZE, MAP_SIZE*(GLfloat)w/(GLfloat)h, -MAP_SIZE, MAP_SIZE,
//                -curScale*fmax(300.0, MAP_SIZE + 50.0), curScale*fmax(300.0, MAP_SIZE + 50.0));
//        glOrtho(-20, MAP_SIZE*(GLfloat)w/(GLfloat)h + 20, -20, MAP_SIZE + 20,
//                -curScale*fmax(1000.0, MAP_SIZE + 50.0), curScale*fmax(1000.0, MAP_SIZE + 50.0));
        width = MAP_SIZE*(GLfloat)w/(GLfloat)h;
        height = MAP_SIZE;
        depth = 10*fmax(300.0, MAP_SIZE + 50.0);
    }
    else {
        glOrtho(-MAP_SIZE*(GLfloat)w/(GLfloat)h/2 ,
                       MAP_SIZE*(GLfloat)w/(GLfloat)h/2, -MAP_SIZE/2, MAP_SIZE/2,
                                       -10*fmax(1000.0, MAP_SIZE + 50.0), 10*fmax(1000.0, MAP_SIZE + 50.0));
        width = MAP_SIZE*(GLfloat)w/(GLfloat)h;
        height = MAP_SIZE;
        depth = 10*fmax(300.0, MAP_SIZE + 50.0);
        xMin = -MAP_SIZE*(GLfloat)w/(GLfloat)h/2;
        xMax = MAP_SIZE*(GLfloat)w/(GLfloat)h/2;
        xCenter = 0;
        yMin = -MAP_SIZE/2;
        yMax = MAP_SIZE/2;
        yCenter = 0;
//        glOrtho(0 ,
//                       MAP_SIZE*(GLfloat)w/(GLfloat)h, 0, MAP_SIZE,
//                       -10*fmax(1000.0, MAP_SIZE + 50.0), 10*fmax(1000.0, MAP_SIZE + 50.0));
////        glOrtho(-MAP_SIZE ,
////                MAP_SIZE*(GLfloat)w/(GLfloat)h, -MAP_SIZE, MAP_SIZE,
////                -10*fmax(300.0, MAP_SIZE + 50.0), 10*fmax(300.0, MAP_SIZE + 50.0));
////        glOrtho(-20, MAP_SIZE*(GLfloat)w/(GLfloat)h + 20, -20, MAP_SIZE + 20,
////                -curScale*fmax(1000.0, MAP_SIZE + 50.0), curScale*fmax(1000.0, MAP_SIZE + 50.0));
//        width = MAP_SIZE*(GLfloat)w/(GLfloat)h;
//        height = MAP_SIZE;
//        depth = 10*fmax(300.0, MAP_SIZE + 50.0);
//        xMin = 0;
//        xMax = MAP_SIZE*(GLfloat)w/(GLfloat)h;
//        xCenter = GLfloat(xMax - xMin) / 2;
//        yMin = 0;
//        yMax = MAP_SIZE;
//        yCenter = GLfloat(yMax - yMin) / 2;
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void CanvasWidget::update() {
    if (curMousePt != lastMousePt) {
        if (useArcBall) {
            //xRotAngle += (curMousePt.x() - lastMousePt.x())/50.0;
            //yRotAngle += (curMousePt.y() - lastMousePt.y())/50.0;

            xCurAngle += (curMousePt.x() - lastMousePt.x())/50.0;
            yCurAngle += (curMousePt.y() - lastMousePt.y())/50.0;

            lastMousePt = curMousePt;
        }
        else if(modifyHeight) {
            //glm::vec3 lastVec = getOpenGLCoord(lastMousePt);
            //glm::vec3 curVec = getOpenGLCoord(curMousePt);
            //curOGLPt = getOpenGLCoord(curMousePt);
            temp_total += lastMousePt.ry() - curMousePt.ry();
            surfaceImage = surface.modifyHeight(glm::vec3(lastOGLPt.x, lastOGLPt.y, (lastMousePt.ry() - curMousePt.ry()) > 0 ? 1 : -1), 4);
            //surfaceImage = surface.modifyHeight(glm::vec3(lastVec.x, lastVec.y, lastMousePt.ry() - curMousePt.ry()));
            //lastOGLPt.z = curOGLPt.z;
            lastMousePt = curMousePt;

        }
        updateGL();
    }
}

void CanvasWidget::keyPressEvent( QKeyEvent *e) {
    switch (e->key()) {
    case 27:
        exit(0);
        break;
    }
}

void CanvasWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        curMousePt = lastMousePt = event->pos();
        useArcBall = true;
    }
    else if (event->button() == Qt::RightButton) {
        curMousePt = lastMousePt = event->pos();
        curOGLPt = lastOGLPt = getOpenGLCoord(curMousePt);
        modifyHeight = true;
        selectPatch = true;


        //addSelectedPatch(event->pos());
        //updateGL();
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* event) {
    if ((event->buttons() & Qt::LeftButton) && useArcBall) {
        curMousePt = event->pos();
    }
    else if ((event->buttons() & Qt::RightButton) && modifyHeight) {
        curMousePt = event->pos();
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        useArcBall = false;
    }
    else  if (event->button() == Qt::RightButton) {
        modifyHeight = false;
        //selectPatch = true;
    }
}

void CanvasWidget::wheelEvent(QWheelEvent* event) {
    QPoint numDegrees = event->angleDelta();

    if (!numDegrees.isNull()) {
        if (numDegrees.y() > 0) {
            curScale += 0.1;
            updateGL();
        }
        else if (numDegrees.y() < 0) {
            curScale -= 0.1;
            updateGL();
        }
    }
}

void CanvasWidget::addSelectedPatch(QPoint point2D) {
    glm::vec3 realPoint = getOpenGLCoord(point2D);
    QPoint upperLeft;
    upperLeft.rx() = floor(float(realPoint.x) / float(SURFACE_STRIDE));
    upperLeft.ry() = floor(float(realPoint.y) / float(SURFACE_STRIDE));
    selectedPatches.push_back(upperLeft);
}

glm::vec3 CanvasWidget::getOpenGLCoord(QPoint point2D) {

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix ();
    glScalef(curScale, curScale, curScale);
    glRotatef(glm::degrees(xCurAngle), 0.0, 1.0, 0.0);
    glRotatef(glm::degrees(yCurAngle), 1.0, 0.0, 0.0);

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)point2D.rx();
    winY = (float)viewport[3] - (float)point2D.ry();
    glReadPixels( point2D.rx(), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    while (winZ >= 1) {
        winZ -= 0.5;
    }

    GLdouble objX1,objY1,objZ1;
    GLdouble objX2,objY2,objZ2;
    gluUnProject(winX, winY,0.0f,
                 modelview, projection, viewport,
                 &objX1 , &objY1 , &objZ1 );
    gluUnProject(winX, winY, 1.0f,
                 modelview, projection, viewport,
                 &objX2 , &objY2 , &objZ2 );

    if(objZ1 == objZ2)
    {
        return glm::vec3(0, 0, 0);
    }
    float fac = (objZ2-height)/(objZ2-objZ1);
    posX = (objX2 + objX1)/2;//(objX2 - (objX2-objX1)*fac);
    posY = (objY2 + objY1)/2;;//(objY2 - (objY2-objY1)*fac);

//    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);


    glPopMatrix ();

    return glm::vec3(posY - yCenter + MAP_SIZE/2, posX - xCenter + MAP_SIZE/2, posZ);
}

//glm::vec3 CanvasWidget::getOpenGLCoord(QPoint point2D) {
//    //winX = (float)point2D.rx();
//    //winY = (float)viewport[3] - (float)point2D.ry();

//    glLoadIdentity();
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glEnable( GL_DEPTH_TEST );
//    glDepthMask(1);
//    glDepthFunc( GL_LEQUAL );
//    //glDepthMask( GL_TRUE );
//    //GLfloat zerodepth;
//    //glReadPixels(315, 320, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zerodepth);
//    glPushMatrix ();
//    glRotatef(glm::degrees(xCurAngle), 0.0f, 1.0f, 0.0f);
//    glRotatef(glm::degrees(yCurAngle), 1.0f, 0.0f, 0.0f);
//    //glScalef(curScale, curScale, curScale);



//    int i = 0;
//    int j = 0;

//    for (i = 0; i <= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); i += SURFACE_STRIDE) {
//        glBegin(GL_LINE_STRIP);
//        for (j = 0; j<= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); j += SURFACE_STRIDE) {
//            glVertex3f(surfaceImage[i][j].x, surfaceImage[i][j].y, surfaceImage[i][j].z);
//        }
//        glEnd();
//    }
//    for (j = 0; j <= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); j += SURFACE_STRIDE) {
//        glBegin(GL_LINE_STRIP);
//        for (i = 0; i<= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); i += SURFACE_STRIDE) {
//            glVertex3f(surfaceImage[i][j].x, surfaceImage[i][j].y, surfaceImage[i][j].z);
//        }
//        glEnd();
//    }


//    GLint viewport[4];
//    GLdouble modelview[16];
//    GLdouble projection[16];
//    GLfloat winX, winY, winZ;
//    GLdouble posX, posY, posZ;

//    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
//    glGetDoublev( GL_PROJECTION_MATRIX, projection );
//    glGetIntegerv( GL_VIEWPORT, viewport );


//    //glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &newdepth);
//    //unproject(x,y, newdepth/zerodepth, ...);

//    winX = (float)point2D.rx();
//    winY = (float)viewport[3] - (float)point2D.ry();
//    glReadPixels( point2D.rx(), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
////    while (winZ >= 1) {
////        winZ -= 0.01;
////    }

////    while (winZ <= -1) {
////        winZ += 0.5;
////    }
////    winZ /=  zerodepth;
//    while (winZ >= 1) {
//        winZ -= 0.5;
//    }

//    //gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

////    GLdouble objX1,objY1,objZ1;
////    GLdouble objX2,objY2,objZ2;
////    gluUnProject(winX, winY,0.0f,
////            modelview, projection, viewport,
////            &objX1 , &objY1 , &objZ1 );
////    gluUnProject(winX, winY, 1.0f,
////                 modelview, projection, viewport,
////            &objX2 , &objY2 , &objZ2 );

////    if(objZ1 == objZ2)
////    {
////        return glm::vec3(0, 0, 0);
////    }
////    float fac = (objZ2-height)/(objZ2-objZ1);
////    posX = (objX2 - (objX2-objX1)*fac);
////    posY = (objY2 - (objY2-objY1)*fac);

//    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

//    glPopMatrix ();

//    return glm::vec3(posY, posX, posZ);
//}
