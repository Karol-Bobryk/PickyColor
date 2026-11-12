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

GLushort indices[] =
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
	3, 4, 0,
	// up
	4, 5, 6,
	4, 6, 7,
	// down
	0, 1, 2,
	0, 2, 3,
};
// gay prism
// GLfloat vertices[] =
// {
//     // base
//     -0.5f, -0.5f, -0.5f,
//      0.5f, -0.5f, -0.5f,
//      0.5f, -0.5f,  0.5f,
//     -0.5f, -0.5f,  0.5f,

//     // apex
//      0.0f,  0.5f,  0.0f,
// };

// GLushort indices[] =
// {
//     // front
//     0, 1, 4,

//     // right
//     1, 2, 4,

//     // back
//     2, 3, 4,

//     // left
//     3, 0, 4,

//     // bottom
//     0, 1, 2,
//     0, 2, 3,
// };
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

    glDisable(GL_CULL_FACE);
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

    vbo->allocate(vertices, sizeof(vertices));

    if (!ibo->create())
        qInfo() << "failed to create the IBO";
    ibo->bind();

    ibo->allocate(indices, sizeof(indices));

    shader_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(float));
    shader_program->enableAttributeArray(0);

    shader_program->setAttributeBuffer(1, GL_FLOAT, 0, 3, 3 * sizeof(float));
    shader_program->enableAttributeArray(1);

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

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(*indices), GL_UNSIGNED_SHORT, nullptr);

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

    QMatrix4x4 projection;
    projection.perspective(45.0f, float(width()) / float(height()), 0.1f, 300.0f);

    QMatrix4x4 view;
    view.translate(0.0f, 0.0f, -3.0f);

    QMatrix4x4 model;
    float angle = isDragged ? angle_rot_tmp : angle_rot;
    float x = isDragged ? x_rot_tmp : x_rot;
    float y = isDragged ? y_rot_tmp : y_rot;

    model.rotate(angle, x, y, 0.0);

    return projection * view * model;
}

void OpenGLView::mousePressEvent(QMouseEvent *event)
{
    if (event->type() != QMouseEvent::MouseButtonPress)
        return;

    x_rot_tmp = x_rot;
    y_rot_tmp = y_rot;
    z_rot_tmp = z_rot;
    angle_rot_tmp = angle_rot;

    isDragged = true;
    drag_origin = event->position();
}

void OpenGLView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->type() != QMouseEvent::MouseMove)
        return;

    if (drag_origin.isNull())
        return;

    QLineF dragVec(drag_origin, event->position());

    float denominator = fmin(width(), height());

    constexpr float scaling_factor = 0.1;

    constexpr float deadzone = 25;

    if (dragVec.length() < deadzone)
        angle_rot_tmp = 0;
    else
        angle_rot_tmp = fmodf((dragVec.length() / denominator) * scaling_factor * 360.0
                                  + angle_rot_tmp,
                              360.0);

    QLineF u_vec = dragVec.unitVector();

    y_rot_tmp = (u_vec.x2() - drag_origin.x());
    x_rot_tmp = (u_vec.y2() - drag_origin.y());

    qInfo() << u_vec;
    update();
}

void OpenGLView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->type() != QMouseEvent::MouseButtonRelease)
        return;

    x_rot = x_rot_tmp;
    y_rot = y_rot_tmp;
    z_rot = z_rot_tmp;
    angle_rot = angle_rot_tmp;

    x_rot_tmp = 0;
    y_rot_tmp = 0;
    z_rot_tmp = 0;
    angle_rot_tmp = 0;

    drag_origin.setX(0);
    drag_origin.setY(0);

    isDragged = false;

    update();
}

OpenGLView::~OpenGLView()
{
    makeCurrent();

    delete vbo;
    delete vao;
    delete ibo;
    delete shader_program;
    delete vertex_shader;
    delete frag_shader;

    doneCurrent();
}