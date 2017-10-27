#ifndef AGUA_H
#define AGUA_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include <limits>
#include <iostream>
#include <memory>
class Agua: public QOpenGLExtraFunctions
{
public:
    Agua( QOpenGLWidget * _glWidget);
    ~Agua();
    std :: unique_ptr < QVector4D [] > vertices ;
    std :: unique_ptr < unsigned int [] > indices ;
    std :: unique_ptr < QVector4D [] > colors = nullptr ;
    QOpenGLWidget * glWidget ;
    unsigned int numVertices ;
    unsigned int numFaces ;
    GLuint vao = 0;
    GLuint vboVertices = 0;
    GLuint vboIndices = 0;
    GLuint vboColors = 0;
    GLuint shaderProgram = 0;
    void createVBOs () ;
    void createShaders () ;
    void destroyVBOs () ;
    void destroyShaders () ;
    void drawAgua () ;
    void criaAgua();
    QMatrix4x4 modelMatrix ;
    QVector3D midPoint ;
    double invDiag ;

};

#endif // AGUA_H
