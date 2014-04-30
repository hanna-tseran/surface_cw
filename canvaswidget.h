#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QGLWidget>
#include <QKeyEvent>
#include <QPoint>
#include <QTimer>
#include <QMouseEvent>
//#include <QVector3D>
#include <cmath>
//#include <QMatrix4x4>
//#include <QMatrix3x3>
#include <gtc/matrix_transform.hpp>

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
    glm::vec3 rotAxis;
    glm::mat4 modelview;

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
