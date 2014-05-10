#include "canvaswidget.h"

CanvasWidget::CanvasWidget(QWidget *parent) :
    QGLWidget(parent),
    timer(new QTimer(this))
{
    timer->start(EXPIRATION_TIME);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    useArcBall = false;
    xCurAngle = 0.0;
    yCurAngle = 0.0;
    xRotAngle = 0.0;
    yRotAngle = 0.0;
    curScale = 1.0;

    HeightMap hm;
    vector< vector<int> > heightMap = hm.LoadRawFile(MAP_NAME, MAP_SIZE);
    Surface surface = Surface(PATCHES_NUM, PATCHES_NUM);
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
    glScalef(curScale, curScale, curScale);
    if (useArcBall) {
        glRotatef(glm::degrees(xRotAngle), 0.0, 1.0, 0.0);
        glRotatef(glm::degrees(yRotAngle), 1.0, 0.0, 0.0);
        xCurAngle = xRotAngle;
        yCurAngle = yRotAngle;
    }
    else {
        glRotatef(glm::degrees(xCurAngle), 0.0, 1.0, 0.0);
        glRotatef(glm::degrees(yCurAngle), 1.0, 0.0, 0.0);
    }

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
    }
    else {
        glOrtho(-MAP_SIZE ,
                MAP_SIZE*(GLfloat)w/(GLfloat)h, -MAP_SIZE, MAP_SIZE,
                -10*fmax(300.0, MAP_SIZE + 50.0), 10*fmax(300.0, MAP_SIZE + 50.0));
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void CanvasWidget::update() {
    if (curMousePt != lastMousePt) {
        xRotAngle += (curMousePt.x() - lastMousePt.x())/50.0;
        yRotAngle += (curMousePt.y() - lastMousePt.y())/50.0;
        lastMousePt = curMousePt;

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
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* event) {
    if ((event->buttons() & Qt::LeftButton) && useArcBall) {
        curMousePt = event->pos();
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        useArcBall = false;
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
