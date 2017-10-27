#include "openglwidget.h"
#include "math.h"

OpenGLWidget :: OpenGLWidget ( QWidget * parent ) : QOpenGLWidget (parent )
{

}

void OpenGLWidget :: initializeGL ()
{
    initializeOpenGLFunctions () ;

    qDebug (" OpenGL version : %s", glGetString ( GL_VERSION ) ) ;
    qDebug (" GLSL %s", glGetString ( GL_SHADING_LANGUAGE_VERSION ) ) ;

    glEnable ( GL_DEPTH_TEST ) ;

    connect (& timer , SIGNAL ( timeout () ) , this , SLOT ( update () ) ) ;
    timer . start (30) ;
    connect (& timerGerador , SIGNAL ( timeout () ) , this , SLOT ( geraNave()) ) ;
    timerGerador . start (std::rand()%5000+500) ;
    //timerGerador . start (3000) ;

}

void OpenGLWidget :: resizeGL (int w , int h )
{
    glViewport (0 ,0 ,w , h ) ;
}
void OpenGLWidget :: paintGL ()
{

    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;


    //Cria cenario se nao foi criado ainda
    if(!(modelTerra && modelAgua)){
        modelTerra = std :: make_shared <Terra>( this ) ;
        modelAgua = std :: make_shared <Agua>( this ) ;
        modelTerra -> criaTerra() ;
        modelAgua->criaAgua();
    }

    //cria canhoes se nao foi criado
    if(!(modelCanhaoDir && modelCanhaoEsq)){
        modelCanhaoEsq = std::make_shared<Canhao>(this);
        modelCanhaoDir = std::make_shared<Canhao>(this);
        modelCanhaoCent = std::make_shared<Canhao>(this);
        modelCanhaoEsq->criaCanhao(CANHAOESQ);
        modelCanhaoDir->criaCanhao(CANHAODIR);
        modelCanhaoCent->criaCanhao(CANHAOCENT);
    }

    //atualizacao das balas primeiro, ver colisao com naves
    if(!listBalas.empty()){
        foreach (std::shared_ptr <Bala> modelBala, listBalas) {
            modelBala->moveBala();
            modelBala->drawBala();

            //verifica colisao com as naves

            foreach (std::shared_ptr <Nave> nave, listNaves) {
                if((std::abs(nave->posY - modelBala->posY) < 0.09)&& std::abs(nave->posX - modelBala->posX)<0.09){
                    nave->destruirNave();
                    nave->drawNave();
                    listNaves.remove(nave);
                    if(nave->tipoNave==CENTRO)
                        pontuacao +=  500;
                    nave->~Nave();

                    modelBala->destruirBala();
                    modelBala->drawBala();
                    listBalas.remove(modelBala);
                    modelBala->~Bala();

                    pontuacao+=100;
                    makeCurrent();
                    emit aumentaPlacar(pontuacao);
                    update();

                }
            }

            //verifica se a bala ja esta fora da tela para remove la
            if(modelBala->posX > 1.5 || modelBala->posX < -1.5){
                modelBala->destruirBala();
                modelBala->drawBala();
                listBalas.remove(modelBala);
                modelBala->~Bala();
            }
        }
    }


    if(!listNaves.empty()){
        foreach (std::shared_ptr <Nave> modelNave, listNaves) {
            modelNave->moveNave();
            modelNave->drawNave();

            //verifica se a nave ja saiu da tela, entao apagar
            if(modelNave->posX > 1.5 || modelNave->posX < -1.5){
                modelNave->destruirNave();
                modelNave->drawNave();
                listNaves.remove(modelNave);
                modelNave->~Nave();
            }

        }
    }



    desenhaCenario();
}
void OpenGLWidget::desenhaCenario(){
    if((modelTerra && modelAgua)){
        modelTerra -> drawTerra();
        modelAgua->drawAgua();
        modelCanhaoDir->drawCanhao();
        modelCanhaoEsq->drawCanhao();
    }
    if(modelCanhaoCent)
        modelCanhaoCent->drawCanhao();

}

void OpenGLWidget::geraNave(){
    timerFlag = ! timerFlag ;
    if ( timerFlag ){
        std:: shared_ptr<Nave>  modelNave = std::make_shared <Nave>(this);
        int numRand = std::rand()%3;
        modelNave->criaNave(numRand);
        listNaves.push_back(modelNave);
    }
}

void OpenGLWidget::keyPressEvent( QKeyEvent * event){

    std::shared_ptr <Bala> modelBala;
    std::shared_ptr <Bala> modelBala1;
    std::shared_ptr <Bala> modelBala2;
    std::shared_ptr <Bala> modelBala3;

    switch (event->key()) {
    case Qt::Key_Left :
        modelBala = std :: make_shared <Bala>( this ) ;
        modelBala->criaBala(CANHAOESQ);
        listBalas.push_back(modelBala);
        break;
    case Qt::Key_Right:
        modelBala1 = std::make_shared <Bala>( this ) ;
        modelBala1->criaBala(CANHAODIR);
        listBalas.push_back(modelBala1);
        break;
    case Qt::Key_Up:
        modelBala2 = std :: make_shared <Bala>( this ) ;
        modelBala2->criaBala(CENTRO);
        listBalas.push_back(modelBala2);
        break;
    case Qt::Key_Space:
        modelBala3 = std :: make_shared <Bala>( this ) ;
        modelBala3->criaBala(CENTRO);
        listBalas.push_back(modelBala3);
        break;
    }

}
