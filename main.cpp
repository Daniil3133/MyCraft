#include <QApplication>

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>



#include <iostream>

static const char* vertsource =
  "#version 330\n"
  "layout(location=0) in vec3 VertexPosition;\n"
  "layout(location=1) in vec3 VertexColor;\n"
  "out vec3 Color;\n"
  "void main()\n"
  "{\n"
  "    Color = VertexColor;\n"
  "    gl_Position = vec4(VertexPosition, 1.0);\n"
  "}\n";

static const char* fragsource[] =
  {"#version 330\n"
  "in vec3 Color\n;"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "    FragColor = vec4(Color, 1.0);\n"
  "}\n"};


class myWindow: public QOpenGLWidget, private QOpenGLFunctions_3_3_Core
{
    GLuint vaoHandle;
    GLuint vboHandle[2];
public:
    myWindow():QOpenGLWidget()
    {
        //this->makeCurrent();
    }
    ~myWindow()
    {
    }
    void initializeGL() override
    {
        initializeOpenGLFunctions();
        initGeometry();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0,0,0,1);

        initShaders();

    }
    void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vaoHandle);

        glDrawArrays(GL_TRIANGLES, 0, 3);

    }
    void resizeGL(int w, int h) override
    {

    }

    void initShaders()
    {
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        if(vertShader == 0)
        {
            std::cerr << "create vertex shader failed" << std::endl;
        }
        glShaderSource(vertShader,1,&vertsource,NULL);
        glCompileShader(vertShader);

        GLint result;
        glGetShaderiv(vertShader,GL_COMPILE_STATUS, &result);
        if(GL_FALSE == result)
        {
            std::cerr << "vertex shader compilation failed" << std::endl;
            GLint logLen;
            glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLen);
            if(logLen > 0)
            {
                char* log = new char[logLen];
                GLint written;
                glGetShaderInfoLog(vertShader, logLen, &written, log);
                std::cout << "Shader log: " << log << std::endl;
                delete[] log;
            }
        }

//-----------------------------------------------------------------------------------
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        if(fragShader == 0)
        {
            std::cerr << "create fragment shader failed" << std::endl;
        }
        glShaderSource(fragShader,1,fragsource,NULL);
        glCompileShader(fragShader);

        glGetShaderiv(fragShader,GL_COMPILE_STATUS, &result);
        if(GL_FALSE == result)
        {
            std::cerr << "fragment shader compilation failed" << std::endl;
            GLint logLen;
            glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLen);
            if(logLen > 0)
            {
                char* log = new char[logLen];
                GLint written;
                glGetShaderInfoLog(fragShader, logLen, &written, log);
                std::cout << "Shader log: " << log << std::endl;
                delete[] log;
            }
        }
//-----------------------------------------------------------------------------------
        GLint programHandle = glCreateProgram();
        if(programHandle == 0)
        {
            std::cerr << "Error creating program" << std::endl;
            exit(1);
        }
        glAttachShader(programHandle, vertShader);
        glAttachShader(programHandle, fragShader);

        glLinkProgram(programHandle);

        GLint status;
        glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
        if(status == GL_FALSE)
        {
            std::cerr << "Failed to link shader program" << std::endl;
            GLint logLen;
            glGetProgramiv(programHandle,GL_INFO_LOG_LENGTH, &logLen);
            if(logLen > 0)
            {
                char* log = new char[logLen];
                GLsizei written;
                glGetProgramInfoLog(programHandle, logLen, &written, log);
                std::cerr << "Program log: " << log << std::endl;
                delete[] log;
            }
        }else
        {
            glUseProgram(programHandle);
        }
        /*if(program.addShaderFromSourceCode(QOpenGLShader::Vertex,vertsource))
        {
            std::cerr << "add vertex shader failed" << std::endl;
            close();
        }
        if(program.addShaderFromSourceCode(QOpenGLShader::Fragment,fragsource))
        {
            std::cerr << "add fragment shader failed" << std::endl;
            close();
        }
        if(program.link())
        {
            std::cerr << "link shader failed" << std::endl;
            close();
        }*/
    }
    void initGeometry()
    {
        float positionData[] ={
          -0.8f,-0.8f,0.0f,
            0.8f,-0.8f,0.0f,
            0.0f,0.8f,0.0f,};
        float colorData[] = {
            1.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,0.0f,1.0f};
        //Создать и заполнить буферные обьекты

        glGenBuffers(2,vboHandle);
        GLuint positionBufferHandle = vboHandle[0];
        GLuint colorBufferHandle = vboHandle[1];

        //Заполнить буфер координат
        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float),
                     positionData, GL_STATIC_DRAW);

        //Заполнить буфер цветов
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float),
                     colorData, GL_STATIC_DRAW);

        //Создать обьект массива вершин
        glGenVertexArrays(1, &vaoHandle);
        glBindVertexArray(vaoHandle);

        //Активировать массивы вершинных атрибутов
        glEnableVertexAttribArray(0);   //Координаты вершин
        glEnableVertexAttribArray(1);   //Цвет вершин

        //Закрепить индекс 0 за буфером с координатами
        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        //Закрепить индекс 1 за буфером с цветом
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glBindVertexArray(0);
    }
};




int main(int argc, char** argv)
{
    QApplication a(argc,argv);



    myWindow w;

    w.show();

    return a.exec();
}
