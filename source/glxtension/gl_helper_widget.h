/*
glxtension - Show OpenGL information
Copyright (C) 2013 Konrad Kleine

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
	Q_UNUSED(width);
	Q_UNUSED(height);
//        glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    }

    /*! Will be called everytime a redraw is requested. */
    void paintGL() {
//        glClear(GL_COLOR_BUFFER_BIT);
    }
};

#endif /*GLWIDGET_H_*/
