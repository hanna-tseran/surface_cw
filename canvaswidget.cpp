#include "canvaswidget.h"

CanvasWidget::CanvasWidget(QWidget *parent) :
    QGLWidget(parent),
    timer(new QTimer(this)), ctrlpoints {
{{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0},
{0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}},
{{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0},
{0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}},
{{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0},
{0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}},
{{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0},
{0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
                                 }
{
    timer->start(EXPIRATION_TIME);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    useArcBall = false;
    xCurAngle = 0.0;
    yCurAngle = 0.0;
    xRotAngle = 0.0;
    yRotAngle = 0.0;
    curScale = 1.0;

    bezierPatch.setCtrlPoints(ctrlpoints);
    image = bezierPatch.generate();
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

    for (i = 0; i <= bezierPatch.getN() * bezierPatch.getVStride(); ++i) {
        glBegin(GL_LINE_STRIP);
        for (j = 0; j<= bezierPatch.getN() * bezierPatch.getUStride(); ++j) {
            glVertex3f(image[i][j].x, image[i][j].y, image[i][j].z);
        }
        glEnd();
    }
    for (j = 0; j <= bezierPatch.getN() * bezierPatch.getUStride(); ++j) {
        glBegin(GL_LINE_STRIP);
        for (i = 0; i<= bezierPatch.getN() * bezierPatch.getVStride(); ++i) {
            glVertex3f(image[i][j].x, image[i][j].y, image[i][j].z);
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
    if (w <= h)
        glOrtho(-4.0, 4.0, -4.0*(GLfloat)h/(GLfloat)w,
                4.0*(GLfloat)h/(GLfloat)w, -4.0, 4.0);
    else
        glOrtho(-4.0*(GLfloat)w/(GLfloat)h,
                4.0*(GLfloat)w/(GLfloat)h, -4.0, 4.0, -4.0, 4.0);
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
