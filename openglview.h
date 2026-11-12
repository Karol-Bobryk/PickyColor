#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H

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

private:
    QColor bg_color;

    QOpenGLShaderProgram *shader_program;
    QOpenGLShader *vertex_shader;
    QOpenGLShader *frag_shader;
    // ebo
    QOpenGLBuffer *vbo;
    QOpenGLBuffer *ibo; // index buffer obj
    GLuint ebo;

    QOpenGLVertexArrayObject *vao;

    std::string vertex_shader_code;
    std::string frag_shader_code;
};

#endif // OPENGLVIEW_H
