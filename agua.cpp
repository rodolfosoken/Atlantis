#include "agua.h"

Agua::Agua(QOpenGLWidget * _glWidget)
{
    glWidget = _glWidget ;
    glWidget -> makeCurrent () ;
    initializeOpenGLFunctions () ;
}

Agua::~Agua(){
    destroyVBOs () ;
    destroyShaders () ;
}
void Agua::destroyVBOs(){
    glDeleteBuffers (1 , & vboVertices ) ;
    glDeleteBuffers (1 , & vboIndices ) ;
    glDeleteVertexArrays (1 , & vao ) ;
    vboVertices = 0;
    vboIndices = 0;
    vao = 0;
}

void Agua:: destroyShaders ()
{
    glDeleteProgram ( shaderProgram ) ;
}

void Agua:: createVBOs ()
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

void Agua:: createShaders ()
{
    // makeCurrent ();
    destroyShaders () ;
    QString vertexShaderFile (":/shaders/vshader_agua.glsl") ;
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


void Agua:: drawAgua()
{
    modelMatrix.setToIdentity () ;
    glBindVertexArray ( vao ) ;
    glUseProgram ( shaderProgram ) ;
    GLuint locModelMatrix = glGetUniformLocation( shaderProgram , "model") ;
    glUniformMatrix4fv( locModelMatrix , 1 , GL_FALSE , modelMatrix.data() ) ;
    glDrawElements ( GL_TRIANGLES , numFaces * 3 , GL_UNSIGNED_INT ,   0) ;


}


void Agua:: criaAgua(){
    int numQuadrados = 5;
    numVertices = 4*numQuadrados;
    numFaces = numVertices/2; //quantidade de triangulos
    vertices = std :: make_unique < QVector4D [] >(numVertices) ;
    indices = std :: make_unique < unsigned int [] >((numVertices/2)* 3) ;

    //vertices de agua
    vertices [0] = QVector4D ( -1 , -0.75 ,0 ,1) ;
    vertices [1] = QVector4D ( 1 , -0.75 ,0 ,1) ;
    vertices [2] = QVector4D ( 1 , -0.25 ,0 ,1) ;
    vertices [3] = QVector4D ( -1 , -0.25 ,0 ,1) ;


    // topologia de quadrado da agua
    indices [0] = 0;
    indices [1] = 1;
    indices [2] = 2;
    indices [3] = 2;
    indices [4] = 3;
    indices [5] = 0;
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
        drawAgua();
    }

}
