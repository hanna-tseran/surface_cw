#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include "stdafx.h"
#include "bezier.h"
#include "surface.h"
#include "heightmap.h"

#include <QKeyEvent>
#include <QPoint>
#include <QTimer>
#include <QMouseEvent>
#include <GL/glu.h>

//struct PatchBounds {
//    QPoint upperLeft;
//    QPoint upperRight;
//    QPoint bottomLeft;
//    QPoint bottomRight;
//};

struct Color {
    GLfloat red;
    GLfloat green;
    GLfloat blue;

    Color( GLfloat red, GLfloat green, GLfloat blue) : red(red), green(green), blue(blue) {}
};

class CanvasWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent=0);
    ~CanvasWidget();

protected:

    static const int EXPIRATION_TIME = 16;
    static const int MAP_SIZE = 256;//64;//1024;//256
    static const int PATCH_SIZE = 4;
    static const int PATCHES_NUM = (MAP_SIZE - 1) / (PATCH_SIZE - 1);
    static const int SURFACE_STRIDE = 1;//1;//8;
    string MAP_NAME = "small-plane.bmp";//"heightmap.bmp";//"heightmap.bmp";//"heightmap-small.bmp";//"test64.bmp";//"heightmap.bmp";
    vector<Color> colors;

    int xMin;
    int xMax;
    int yMin;
    int yMax;
    int xCenter;
    int yCenter;
    int width;
    int height;
    int depth;
    QTimer* timer;
    bool useArcBall;
    bool modifyHeight;
    bool selectPatch;
    bool modifyPatchHeight;
    QPoint curMousePt;
    QPoint lastMousePt;
    glm::vec3 lastOGLPt;
    glm::vec3 curOGLPt;
    float xRotAngle;
    float yRotAngle;
    float xCurAngle;
    float yCurAngle;
    float curScale;
    Bezier bezierPatch;
    Patch patch;
    Surface surface = Surface();
    Patch surfaceImage;
    //vector<PatchBounds> selectedPatches;
    vector<QPoint> selectedPatches;

    int temp_total;

    glm::vec3 getOpenGLCoord(QPoint);
    void addSelectedPatch(QPoint);
    //void addSelectedPatch(QPoint);

    void paintGL();
    void initializeGL(void);
    void resizeGL( int w, int h);

    void keyPressEvent( QKeyEvent *e);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);

signals:

public slots:
    void update();

};

#endif // CANVASWIDGET_H
