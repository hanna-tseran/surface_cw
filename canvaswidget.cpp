#include "canvaswidget.h"

CanvasWidget::CanvasWidget(QWidget *parent) :
    QGLWidget(parent),
    timer(new QTimer(this))
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
            vector< vector<int> > patchHeightMap;

            for (int iP = 0; iP < PATCH_SIZE; ++iP) {
                vector<int> row;
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix ();
    //glLoadIdentity();
    glScalef(curScale, curScale, curScale);
    glRotatef(glm::degrees(xCurAngle), 0.0, 1.0, 0.0);
    glRotatef(glm::degrees(yCurAngle), 1.0, 0.0, 0.0);
//    if (useArcBall) {
//        glRotatef(glm::degrees(xRotAngle), 0.0, 1.0, 0.0);
//        glRotatef(glm::degrees(yRotAngle), 1.0, 0.0, 0.0);
//        xCurAngle = xRotAngle;
//        yCurAngle = yRotAngle;
//    }
//    else {
//        glRotatef(glm::degrees(xCurAngle), 0.0, 1.0, 0.0);
//        glRotatef(glm::degrees(yCurAngle), 1.0, 0.0, 0.0);
//    }

    for (i = 0; i <= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); i += SURFACE_STRIDE) {
        glBegin(GL_LINE_STRIP);
        for (j = 0; j<= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); j += SURFACE_STRIDE) {
            glVertex3f(surfaceImage[i][j].x, surfaceImage[i][j].y, surfaceImage[i][j].z);
        }
        glEnd();
    }
    for (j = 0; j <= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); j += SURFACE_STRIDE) {
        glBegin(GL_LINE_STRIP);
        for (i = 0; i<= PATCHES_NUM * bezierPatch.getN() * bezierPatch.getStride(); i += SURFACE_STRIDE) {
            glVertex3f(surfaceImage[i][j].x, surfaceImage[i][j].y, surfaceImage[i][j].z);
        }
        glEnd();
    }

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
        glOrtho(-MAP_SIZE, MAP_SIZE*(GLfloat)w/(GLfloat)h, -MAP_SIZE, MAP_SIZE,
                -curScale*fmax(300.0, MAP_SIZE + 50.0), curScale*fmax(300.0, MAP_SIZE + 50.0));
        width = MAP_SIZE*(GLfloat)w/(GLfloat)h;
        height = MAP_SIZE;
    }
    else {
        glOrtho(-MAP_SIZE ,
                MAP_SIZE*(GLfloat)w/(GLfloat)h, -MAP_SIZE, MAP_SIZE,
                -10*fmax(300.0, MAP_SIZE + 50.0), 10*fmax(300.0, MAP_SIZE + 50.0));
        width = MAP_SIZE*(GLfloat)w/(GLfloat)h;
        height = MAP_SIZE;
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
            surfaceImage = surface.modifyHeight(glm::vec3(lastOGLPt.x, lastOGLPt.y, lastMousePt.ry() - curMousePt.ry()));
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
//        while (curOGLPt.x < -width || curOGLPt.x > width ||
//               curOGLPt.y < -height || curOGLPt.y > height) {
//            curOGLPt = lastOGLPt = getOpenGLCoord(curMousePt);
//        }
        modifyHeight = true;
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
        //surfaceImage = surface.modifyHeight(glm::vec3(lastOGLPt.x, lastOGLPt.y, temp_total));
        //int total = temp_total;
        //updateGL();
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

glm::vec3 CanvasWidget::getOpenGLCoord(QPoint point2D) {
   // glLoadIdentity();

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glColor3f(1.0, 1.0, 1.0);

    glPushMatrix ();
    //glLoadIdentity();
    //glLoadIdentity();
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
    //winX = (float)viewport[3] - (float)point2D.rx();
    winY = (float)viewport[3] - (float)point2D.ry();
    glReadPixels( point2D.rx(), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    //glReadPixels( point2D.rx(), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    while (winZ >= 1) {
        winZ -= 0.5;
    }

    while (winZ <= -1) {
        winZ += 0.5;
    }

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);


    //glLoadIdentity();
    glPopMatrix ();
    //glFlush();
    //updateGL();

    return glm::vec3(posY, posX, posZ);
    //return glm::vec3(posX, posY, posZ);
}
