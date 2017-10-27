#ifndef NAVE_H
#define NAVE_H

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

class Nave: public QOpenGLExtraFunctions
{
public:
    Nave( QOpenGLWidget * _glWidget);
    ~Nave();
    std :: unique_ptr < QVector4D [] > vertices ;
    std :: unique_ptr < unsigned int [] > indices ;
    std :: unique_ptr < QVector4D [] > colors = nullptr ;
    QOpenGLWidget * glWidget ;
    unsigned int numVertices ;
    unsigned int numQuadrados ;
    unsigned int numFaces ;
    unsigned int tipoNave;
    float posX=0,posY=0;
    GLuint vao = 0;
    GLuint vboVertices = 0;
    GLuint vboIndices = 0;
    GLuint vboColors = 0;
    GLuint shaderProgram = 0;
    void createVBOs () ;
    void createShaders () ;
    void destroyVBOs () ;
    void destroyShaders () ;
    void drawNave () ;
    void criaNave(int dirNave);
    void moveNave();
    QMatrix4x4 modelMatrix ;
    QVector3D midPoint ;
    double invDiag ;
    void transladarModel(float dx,float dy);
    void flipY();
    void destruirNave();
};


#endif // NAVE_H
