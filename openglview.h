#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H

#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

class OpenGLView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLView(QWidget *parent = nullptr);
    ~OpenGLView();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

protected:
    void setBgColor(QColor bg_color) { this->bg_color = bg_color; }
    void initializeBgColorFromPalette();
    void initializeShaders();
    void attachShadersToProgram();
    QMatrix4x4 getMVPMatrix();

    // Mouse interaction overrides
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool isDragged = false;
    float x_rot = -1.0f;
    float y_rot = -1.0f;
    float z_rot = -1.0f;
    float angle_rot = -1.0f;

    float x_rot_tmp = 0.0f;
    float y_rot_tmp = 0.0f;
    float z_rot_tmp = 0.0f;
    float angle_rot_tmp = 0.0f;

    QColor bg_color;

    QOpenGLShaderProgram *shader_program;
    QOpenGLShader *vertex_shader;
    QOpenGLShader *frag_shader;
    // ebo
    QOpenGLBuffer *vbo;
    // TODO: add cbo (color)
    QOpenGLBuffer *ibo; // index buffer obj

    QOpenGLVertexArrayObject *vao;

    std::string vertex_shader_code;
    std::string frag_shader_code;

    QPointF drag_origin;
};

#endif // OPENGLVIEW_H
