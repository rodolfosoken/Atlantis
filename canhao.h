#ifndef CANHAO_H
#define CANHAO_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include <limits>
#include <iostream>
#include <memory>

#define CANHAOESQ 0
#define CANHAODIR 1
#define CANHAOCENT 2



class Canhao : public QOpenGLExtraFunctions
{
public:
    Canhao( QOpenGLWidget * _glWidget);
    ~Canhao();
    std :: unique_ptr < QVector4D [] > vertices ;
    std :: unique_ptr < unsigned int [] > indices ;
    std :: unique_ptr < QVector4D [] > colors = nullptr ;
    QOpenGLWidget * glWidget ;
    unsigned int numQuadrados;
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
    void drawCanhao (const int &tipoCanhao) ;
    void drawCanhao();
    void criaCanhao(const int &tipoCanhao);
    QMatrix4x4 modelMatrix ;
    QVector3D midPoint ;
    double invDiag ;

protected:
    void canhaoLateral();
    void canhaoCentral();
};

#endif // CANHAO_H
