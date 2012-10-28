#ifndef GLWIDGET_H_
#define GLWIDGET_H_

#include <QGLWidget>

/*!
Since OpenGL function calls are only possible when we have made an QGLContext
the current one, this class aids in querying certain OpenGL information.
It is not used to actually draw something.
 */
class GLHelperWidget : public QGLWidget
{
    Q_OBJECT
public:
    /*! Constructs a GLHelperWidget */
    GLHelperWidget(QWidget * parent = 0)
        : QGLWidget(parent)
    {
    }

    /*! Wrapper for const GLubyte * glGetString(GLenum name) */
    QString getString(GLenum name)
    {
        QString ret;
        makeCurrent();
        const GLubyte * string = glGetString(name);
        if (string)
            ret = QString((const char*) string);
        return ret;
    }

protected:
    /*! Will be called once during OpenGL initialzation phase. */
    void initializeGL() {
//        glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    }

    /*! Will be called everytime the widget is resized. */
    void resizeGL(int width, int height) {
//        glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    }

    /*! Will be called everytime a redraw is requested. */
    void paintGL() {
//        glClear(GL_COLOR_BUFFER_BIT);
    }
};

#endif /*GLWIDGET_H_*/
