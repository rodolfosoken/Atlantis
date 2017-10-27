#ifndef BALA_H
#define BALA_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include <limits>
#include <iostream>
#include <memory>

#define ESQUERDA 0
#define DIREITA 1
#define CENTRO 2


class Bala: public QOpenGLExtraFunctions
{
public:
    Bala( QOpenGLWidget * _glWidget);
    ~Bala();
    std :: unique_ptr < QVector4D [] > vertices ;
    std :: unique_ptr < unsigned int [] > indices ;
    std :: unique_ptr < QVector4D [] > colors = nullptr ;
    QOpenGLWidget * glWidget ;
    unsigned int numVertices ;
    unsigned int numQuadrados ;
    unsigned int numFaces ;
    unsigned int tipoBala;
    GLuint vao = 0;
    GLuint vboVertices = 0;
    GLuint vboIndices = 0;
    GLuint vboColors = 0;
    GLuint shaderProgram = 0;
    void createVBOs () ;
    void createShaders () ;
    void destroyVBOs () ;
    void destroyShaders () ;
    void drawBala () ;
    void criaBala(int locBala);
    void moveBala();
    QMatrix4x4 modelMatrix ;
    QVector3D midPoint ;
    double invDiag ;
    float posX=0,posY=0;
    void transladarModel(float dx,float dy);
    void flipY();
    void destruirBala();
};

#endif // BALA_H
