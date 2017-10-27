#ifndef TERRA_H
#define TERRA_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include <limits>
#include <iostream>
#include <memory>

class Terra : public QOpenGLExtraFunctions
{
public:
    Terra( QOpenGLWidget * _glWidget);
    ~Terra();
    std :: unique_ptr < QVector4D [] > vertices ;
    std :: unique_ptr < unsigned int [] > indices ;
    std :: unique_ptr < QVector4D [] > colors = nullptr ;
    QOpenGLWidget * glWidget ;
    unsigned int numVertices ;
    unsigned int numFaces ;
    unsigned int numQuadrados ;
    GLuint vao = 0;
    GLuint vboVertices = 0;
    GLuint vboIndices = 0;
    GLuint vboColors = 0;
    GLuint shaderProgram = 0;
    void createVBOs () ;
    void createShaders () ;
    void destroyVBOs () ;
    void destroyShaders () ;
    void drawTerra () ;
    void criaTerra();
    QMatrix4x4 modelMatrix ;
    QVector3D midPoint ;
    double invDiag ;

};

#endif // TERRA_H
