#include "openglview.h"
#include <QFile>
#include <QMatrix4x4>

/* clang-format off */
GLfloat vertices[] =
{
	// lower
	-0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,  
	// upper
	-0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,  

};

GLfloat colors[] = // TODO: try calcing it in a shader, its pretty procedural +0.5 to each cord :V
{
	0.0f, 0.0f, 0.0f,  
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f,  
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,  
};

GLshort indices[] =
{
	// left
	0, 1, 4,
	1, 4, 5,
	// front
	1, 2, 5,
	2, 5, 6,
	// right
	2, 3, 6,
	3, 6, 7,
	// back
	3, 4, 7,
	4, 7, 8,
	// up
	4, 5, 6,
	4, 6, 7,
	// down
	0, 1, 2,
	0, 2, 3,
};
/* clang-format on */
OpenGLView::OpenGLView(QWidget *parent)
    : QOpenGLWidget{parent}

{}

void OpenGLView::initializeGL()
{
    shader_program = new QOpenGLShaderProgram(this);
    vertex_shader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    frag_shader = new QOpenGLShader(QOpenGLShader::Fragment, this);

    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vao = new QOpenGLVertexArrayObject(this);

    initializeOpenGLFunctions();

    initializeBgColorFromPalette();

    glEnable(GL_DEPTH_TEST);

    initializeShaders();

    attachShadersToProgram();
    if (!shader_program->link())
        qInfo() << "failed to link the shader program";

    if (!vao->create())
        qInfo() << "failed to create the VAO";
    vao->bind();

    if (!vbo->create())
        qInfo() << "failed to create the VBO";
    vbo->bind();

    vbo->allocate(vertices, sizeof(vertices)); // TODO: add ebo

    shader_program->enableAttributeArray(0);
    shader_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));

    shader_program->enableAttributeArray(1);
    shader_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));

    vbo->release();
    vao->release();
}

void OpenGLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!shader_program->bind())
        qInfo() << "shader program bounding failure";

    QMatrix4x4 mvp = getMVPMatrix();

    shader_program->setUniformValue("mvp", mvp);

    vao->bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);

    vao->release();

    shader_program->release();
}

void OpenGLView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void OpenGLView::initializeBgColorFromPalette()
{
    setBgColor(QWidget::palette().color(QPalette::Window));
    glClearColor(bg_color.redF(), bg_color.greenF(), bg_color.blueF(), bg_color.alphaF());
}

void OpenGLView::initializeShaders()
{
    if (!vertex_shader->compileSourceFile("../../vertex_shader.vert")) {
        qInfo() << "failed to open the vertex shader file";
        return;
    }

    if (!frag_shader->compileSourceFile("../../fragment_shader.frag")) {
        qInfo() << "failed to open the fragment shader file";
        return;
    }
}

void OpenGLView::attachShadersToProgram()
{
    if (!shader_program->addShader(vertex_shader)) {
        qInfo() << "failed to attach the vertex shader";
        return;
    }

    if (!shader_program->addShader(frag_shader)) {
        qInfo() << "failed to attach the frag shader";
        return;
    }
}

QMatrix4x4 OpenGLView::getMVPMatrix()
{
    static float rotation = 0; // TODO: delete funzies
    rotation = fmodf(rotation + 5, 360);

    QMatrix4x4 projection;
    projection.perspective(45.0f, float(width()) / float(height()), 0.1f, 300.0f);

    QMatrix4x4 view;
    view.translate(0.0f, 0.0f, -3.0f);

    QMatrix4x4 model;
    model.rotate(rotation, 1.0f, 1.0f, 1.0f);

    return projection * view * model;
}

OpenGLView::~OpenGLView()
{
    makeCurrent();

    delete vbo;
    delete vao;
    delete shader_program;
    delete vertex_shader;
    delete frag_shader;

    doneCurrent();
}