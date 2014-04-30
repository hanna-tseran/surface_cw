#include "mainwindow.h"
#include <QApplication>
#include <stdlib.h>
#include <qgl.h>
#include <QGLWidget>
#include <QKeyEvent>
#include "canvaswidget.h"

int main(int argc, char** argv)
{
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication a(argc, argv );

    if ( !QGLFormat::hasOpenGL() ) {
        qWarning( "This system does not support OpenGL.  Exiting.");
        return -1;
    }

    CanvasWidget canvas;
    a.setActiveWindow(&canvas);
    canvas.show();
    return a.exec();
}
