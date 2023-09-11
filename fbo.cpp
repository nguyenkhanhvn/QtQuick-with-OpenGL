#include "fbo.h"

#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <qopenglshaderprogram.h>
#include <qopenglfunctions.h>

#include <QTime>
#include <QList>
#include <QOpenGLFunctions>
#include <QPaintEngine>
#include <qmath.h>


GLfloat  vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
        0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
};

// Indices for vertices order
GLuint  indices[] =
    {
        0, 3, 5, // Lower left triangle
        3, 2, 4, // Upper triangle
        5, 4, 1 // Lower right triangle
};
//        QOpenGLVertexArrayObject vao;
//        QOpenGLBuffer vbo(QOpenGLBuffer::VertexBuffer);
//        QOpenGLBuffer ebo(QOpenGLBuffer::IndexBuffer);


class DataRenderer : protected QOpenGLFunctions
{
public:
    DataRenderer(): vbo(QOpenGLBuffer::VertexBuffer), ebo(QOpenGLBuffer::IndexBuffer) {}
    ~DataRenderer() {}

    void render() {
        //        glDepthMask(true);

        glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

        m_program.bind();
        vao.bind();

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, indices);

        vao.release();
        m_program.release();
    }
    void initialize() {
        initializeOpenGLFunctions();


        const char *vertShaderSrc =
            "attribute vec3 aPosition;"
            "void main()"
            "{"
            "    gl_Position = vec4(aPosition, 1.0);"
            "}";
        const char *fragShaderSrc =
            "void main()"
            "{"
            "    gl_FragColor = vec4(0.5, 0.2, 0.9, 1.0);"
            "}";
        m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertShaderSrc);
        m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragShaderSrc);
        m_program.link();
        m_program.bind();

        vao.create();
        vbo.create();
        vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
//        ebo.create();
//        ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);

        vao.bind();

        vbo.bind();
        vbo.allocate(vertices, sizeof(vertices));

//        ebo.bind();
//        ebo.allocate(indices, sizeof(indices));


        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3);
        m_program.enableAttributeArray(0);

        vbo.release();
        vao.release();
//        ebo.release();
    }

private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ebo;
};

class FboRenderer : public QQuickFramebufferObject::Renderer
{
public:
    FboRenderer() {
        data.initialize();
    }

    void render() override {
        data.render();
        update();
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    DataRenderer data;

};

QQuickFramebufferObject::Renderer *FBO::createRenderer() const
{
    return new FboRenderer();
}
