#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QGLWidget>
#include <QKeyEvent>
#include <QPoint>
#include <QTimer>
#include <QMouseEvent>
#include <cmath>
#include <gtc/matrix_transform.hpp>
#include "bezier.h"

typedef vector<vector<glm::vec3>> Image;

class CanvasWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent=0);
    ~CanvasWidget();

protected:

    static int const EXPIRATION_TIME = 16;

    int width;
    int height;
    GLfloat ctrlpoints[4][4][3];
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
    Image image;

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
