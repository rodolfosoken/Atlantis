#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include "agua.h"
#include "terra.h"
#include "canhao.h"
#include "bala.h"
#include "nave.h"

class OpenGLWidget : public QOpenGLWidget , protected
QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    std :: shared_ptr < Terra > modelTerra = nullptr ;
    std :: shared_ptr < Agua > modelAgua = nullptr ;
    std:: shared_ptr<Canhao> modelCanhaoEsq = nullptr;
    std:: shared_ptr<Canhao> modelCanhaoDir = nullptr;
    std:: shared_ptr<Canhao> modelCanhaoCent = nullptr;

    std ::list <std::shared_ptr <Bala>> listBalas;
    std ::list <std::shared_ptr <Nave>> listNaves;
    void desenhaCenario();
    unsigned int pontuacao=0;


protected:
    void initializeGL () ;
    void resizeGL ( int width , int height ) ;
    void paintGL () ;

private:
    QTimer timer;
    QTimer timerGerador;
    bool timerFlag = false ;

signals:
    void aumentaPlacar(int pontos);

public slots:
    void keyPressEvent(QKeyEvent *event);
    void geraNave();
};

#endif // OPENGLWIDGET_H
