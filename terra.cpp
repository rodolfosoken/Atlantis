#include "terra.h"

Terra::Terra(QOpenGLWidget * _glWidget)
{
    glWidget = _glWidget ;
    glWidget -> makeCurrent () ;
    initializeOpenGLFunctions () ;
}

Terra::~Terra(){
    destroyVBOs () ;
    destroyShaders () ;
}

void Terra::destroyVBOs(){
    glDeleteBuffers (1 , & vboVertices ) ;
    glDeleteBuffers (1 , & vboIndices ) ;
    glDeleteVertexArrays (1 , & vao ) ;
    vboVertices = 0;
    vboIndices = 0;
    vao = 0;
}

void Terra:: destroyShaders ()
{
    glDeleteProgram ( shaderProgram ) ;
}

void Terra:: createVBOs ()
{
    glWidget -> makeCurrent () ;
    destroyVBOs () ;
    glGenVertexArrays (1 , & vao ) ;
    glBindVertexArray ( vao ) ;
    glGenBuffers (1 , & vboVertices ) ;
    glBindBuffer ( GL_ARRAY_BUFFER , vboVertices ) ;
    glBufferData ( GL_ARRAY_BUFFER , numVertices * sizeof ( QVector4D )
                   , vertices . get () , GL_STATIC_DRAW ) ;

    glVertexAttribPointer (0 , 4 , GL_FLOAT , GL_FALSE , 0 , nullptr ) ;
    glEnableVertexAttribArray (0) ;
    glGenBuffers (1 , & vboIndices ) ;
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vboIndices ) ;
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER , numFaces * 3 * sizeof (
                       unsigned int ) , indices . get () , GL_STATIC_DRAW ) ;
}

void Terra:: createShaders ()
{
    // makeCurrent ();
    destroyShaders () ;
    QString vertexShaderFile (":/shaders/vshader_terra.glsl") ;
    QString fragmentShaderFile (":/shaders/fshader1.glsl") ;
    QFile vs ( vertexShaderFile ) ;
    QFile fs ( fragmentShaderFile ) ;
    vs . open ( QFile :: ReadOnly | QFile :: Text ) ;
    fs . open ( QFile :: ReadOnly | QFile :: Text ) ;
    QTextStream streamVs (& vs ) , streamFs (& fs ) ;
    QString qtStringVs = streamVs . readAll () ;
    QString qtStringFs = streamFs . readAll () ;
    std :: string stdStringVs = qtStringVs . toStdString () ;
    std :: string stdStringFs = qtStringFs . toStdString () ;
    // Create an empty vertex shader handle
    GLuint vertexShader = 0;
    vertexShader = glCreateShader( GL_VERTEX_SHADER ) ;
    // Send the vertex shader source code to GL
    const GLchar * source = stdStringVs . c_str () ;
    glShaderSource ( vertexShader , 1 , & source , 0) ;
    // Compile the vertex shader
    glCompileShader ( vertexShader ) ;
    GLint isCompiled = 0;
    glGetShaderiv ( vertexShader , GL_COMPILE_STATUS , & isCompiled ) ;
    if ( isCompiled == GL_FALSE )
    {
        GLint maxLength = 0;
        glGetShaderiv ( vertexShader , GL_INFO_LOG_LENGTH , &
                        maxLength ) ;
        // The maxLength includes the NULL character
        std :: vector < GLchar > infoLog ( maxLength ) ;
        glGetShaderInfoLog ( vertexShader , maxLength , & maxLength , &
                             infoLog [0]) ;
        qDebug ("%s", & infoLog [0]) ;
        glDeleteShader ( vertexShader ) ;
        return ;
    }
    // Create an empty fragment shader handle
    GLuint fragmentShader = 0;
    fragmentShader = glCreateShader ( GL_FRAGMENT_SHADER ) ;
    // Send the fragment shader source code to GL
    // Note that std :: string ’s . c_str is NULL character terminated .
    source = stdStringFs . c_str () ;
    glShaderSource ( fragmentShader , 1 , & source , 0) ;
    // Compile the fragment shader
    glCompileShader ( fragmentShader ) ;
    glGetShaderiv ( fragmentShader , GL_COMPILE_STATUS , & isCompiled )
            ;
    if ( isCompiled == GL_FALSE )
    {
        GLint maxLength = 0;
        glGetShaderiv ( fragmentShader , GL_INFO_LOG_LENGTH , &
                        maxLength ) ;
        std :: vector < GLchar > infoLog ( maxLength ) ;
        glGetShaderInfoLog ( fragmentShader , maxLength , & maxLength ,
                             & infoLog [0]) ;
        qDebug ("%s", & infoLog [0]) ;
        glDeleteShader ( fragmentShader ) ;
        glDeleteShader ( vertexShader ) ;
        return ;
    }
    shaderProgram = glCreateProgram () ;
    // Attach our shaders to our program
    glAttachShader ( shaderProgram , vertexShader ) ;
    glAttachShader ( shaderProgram , fragmentShader ) ;
    // Link our program
    glLinkProgram ( shaderProgram ) ;
    // Note the different functions here : glGetProgram * insteadof glGetShader *.
    GLint isLinked = 0;
    glGetProgramiv ( shaderProgram , GL_LINK_STATUS , ( int *) &
                     isLinked ) ;
    if ( isLinked == GL_FALSE )
    {
        GLint maxLength = 0;
        glGetProgramiv ( shaderProgram , GL_INFO_LOG_LENGTH , &
                         maxLength ) ;
        // The maxLength includes the NULL character
        std :: vector < GLchar > infoLog ( maxLength ) ;
        glGetProgramInfoLog ( shaderProgram , maxLength , & maxLength ,
                              & infoLog [0]) ;
        qDebug ("%s", & infoLog [0]) ;
        glDeleteProgram ( shaderProgram ) ;
        glDeleteShader ( vertexShader ) ;
        glDeleteShader ( fragmentShader ) ;
        return ;
    }
    glDetachShader ( shaderProgram , vertexShader ) ;
    glDetachShader ( shaderProgram , fragmentShader ) ;
    glDeleteShader ( vertexShader ) ;
    glDeleteShader ( fragmentShader ) ;
    vs . close () ;
    fs . close () ;
}


void Terra:: drawTerra()
{
    modelMatrix.setToIdentity () ;
    glBindVertexArray ( vao ) ;
    glUseProgram ( shaderProgram ) ;
    GLuint locModelMatrix = glGetUniformLocation( shaderProgram , "model") ;
    glUniformMatrix4fv( locModelMatrix , 1 , GL_FALSE , modelMatrix.data() ) ;
    glDrawElements ( GL_TRIANGLES , numFaces * 3 , GL_UNSIGNED_INT ,   0) ;


}



void Terra::criaTerra(){
    numQuadrados = 7;
    numVertices = 4*numQuadrados;
    numFaces = numVertices/2; //quantidade de triangulos
    vertices = std :: make_unique < QVector4D [] >(numVertices) ;
    indices = std :: make_unique < unsigned int [] >((numVertices/2)* 3) ;

    // chao abaixo da agua
    vertices [0] = QVector4D ( -1 , -1 ,0 ,1) ;
    vertices [1] = QVector4D ( 1 , -1 ,0 ,1) ;
    vertices [2] = QVector4D ( 1 , -0.75 ,0 ,1) ;
    vertices [3] = QVector4D ( -1 , -0.75 ,0 ,1) ;

    //vertices torre direita
    vertices [4] = QVector4D ( 0.85 , -1 ,0 ,1) ;
    vertices [5] = QVector4D ( 1 , -1 ,0 ,1) ;
    vertices [6] = QVector4D ( 1 , -0.15 ,0 ,1) ;
    vertices [7] = QVector4D ( 0.85 , -0.15 ,0 ,1) ;

    //vertice torre esquerda
    vertices [8] = QVector4D ( -1 , -1 ,0 ,1) ;
    vertices [9] = QVector4D ( -0.85 , -1 ,0 ,1) ;
    vertices [10] = QVector4D ( -0.85 , -0.15 ,0 ,1) ;
    vertices [11] = QVector4D ( -1 , -0.15 ,0 ,1) ;

    //vertices torre centro
    vertices [12] = QVector4D ( -0.10 , -1 ,0 ,1) ;
    vertices [13] = QVector4D ( 0.10 , -1 ,0 ,1) ;
    vertices [14] = QVector4D ( 0.10 , -0.15 ,0 ,1) ;
    vertices [15] = QVector4D ( -0.10 , -0.15 ,0 ,1) ;

    //mote ao lado esq do centro
    vertices [16] = QVector4D ( -0.20 , -1 ,0 ,1) ; //canto inf esq
    vertices [17] = QVector4D ( -0.10 , -1 ,0 ,1) ;  //canto inf dir
    vertices [18] = QVector4D ( -0.10 , -0.40 ,0 ,1) ;//canto sup dir
    vertices [19] = QVector4D ( -0.20 , -0.40 ,0 ,1) ; //canto sup esq

    vertices [20] = QVector4D ( 0.20 , -1 ,0 ,1) ; //canto inf esq
    vertices [21] = QVector4D ( 0.10 , -1 ,0 ,1) ;  //canto inf dir
    vertices [22] = QVector4D ( 0.10 , -0.40 ,0 ,1) ;//canto sup dir
    vertices [23] = QVector4D ( 0.20 , -0.40 ,0 ,1) ; //canto sup esq

    //monte ao lado torre esquerda 7
    vertices [24] = QVector4D ( -0.85 , -1 ,0 ,1) ;
    vertices [25] = QVector4D ( -0.75 , -1 ,0 ,1) ;
    vertices [26] = QVector4D ( -0.75 , -0.40 ,0 ,1) ;
    vertices [27] = QVector4D ( -0.85 , -0.40 ,0 ,1) ;


    // topologia de quadrado do chao
    indices [0] = 0;
    indices [1] = 1;
    indices [2] = 2;
    indices [3] = 2;
    indices [4] = 3;
    indices [5] = 0;

    //topologia de quadrados da torre direita
    indices [6] = 4;
    indices [7] = 5;
    indices [8] = 6;
    indices [9] = 6;
    indices [10] = 7;
    indices [11] = 4;

    //topologia torre esquerda
    indices [12] = 8;
    indices [13] = 9;
    indices [14] = 10;
    indices [15] = 10;
    indices [16] = 11;
    indices [17] = 8;

    //topologia torre esquerda
    indices [18] = 12;
    indices [19] = 13;
    indices [20] = 14;
    indices [21] = 14;
    indices [22] = 15;
    indices [23] = 12;

    //topologia mote ao lado esq do centro
    indices [24] = 16;
    indices [25] = 17;
    indices [26] = 18;
    indices [27] = 18;
    indices [28] = 19;
    indices [29] = 16;

    //topologia mote ao lado esq do centro
    indices [30] = 20;
    indices [31] = 21;
    indices [32] = 22;
    indices [33] = 22;
    indices [34] = 23;
    indices [35] = 20;

    indices [36] = 24;
    indices [37] = 25;
    indices [38] = 26;
    indices [39] = 26;
    indices [40] = 27;
    indices [41] = 24;



    if ( numVertices > 0)
    {
        float minLim = std :: numeric_limits <float>:: lowest () ;
        float maxLim = std :: numeric_limits <float>:: max () ;
        QVector4D max ( minLim , minLim , minLim , 1.0) ;
        QVector4D min ( maxLim , maxLim , maxLim , 1.0) ;
        for ( unsigned int i = 0; i < numVertices ; ++ i )
        {
            max . setX ( std :: max ( max.x(), vertices[i][0] ) ) ;
            max . setY ( std :: max ( max.y(), vertices[i][1] ) ) ;
            max . setZ ( std :: max ( max.z(), vertices[i][2] ) ) ;
            min . setX ( std :: min ( min.x(), vertices[i][0] ) ) ;
            min . setY ( std :: min ( min.y(), vertices[i][1] ) ) ;
            min . setZ ( std :: min ( min.z(), vertices[i][2] ) ) ;
        }


        this -> midPoint = QVector3D (( min + max ) * 0.5) ;
        this -> invDiag = 2.0 / ( max - min ) . length () ;

        createVBOs () ;
        createShaders () ;
        drawTerra();
    }

}

