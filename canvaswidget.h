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

class CanvasWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent=0);
    ~CanvasWidget();

protected:

    static const int EXPIRATION_TIME = 16;
    static const int MAP_SIZE = 256;//1024;//256
    static const int PATCH_SIZE = 4;
    static const int PATCHES_NUM = (MAP_SIZE - 1) / (PATCH_SIZE - 1);
    static const int SURFACE_STRIDE = 8;
    string MAP_NAME = "heightmap-small.bmp";

    int width;
    int height;
    QTimer* timer;
    bool useArcBall;
    QPoint curMousePt;
    QPoint lastMousePt;
    float xRotAngle;
    float yRotAngle;
    float xCurAngle;
    float yCurAngle;
    float curScale;
    Bezier bezierPatch;
    Patch patch;
    //Surface surface;
    Patch surfaceImage;


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
