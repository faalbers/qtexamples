#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glew.h>
#include <QtOpenGL/QtOpenGL>
#include <QtOpenGL/QGLWidget>
#include "ShaderUtils.h"

struct attributes {
    GLfloat coord3d[3];
    GLfloat v_color[3];
};

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = NULL);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void setLMouseRotation(int dx, int dy);
    void setRMouseRotation(int dx, int dy);
    void animUpdate();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QVector<QVector3D>  vertices;
    QVector<QVector3D> vertexNormals;
    QVector<QVector3D> vertexColors;
    QVector<GLushort> elements;

    QVector3D   modelPos;
    QVector3D   modelRot;
    QVector3D   modelScl;
    QVector3D   camPos;
    QVector3D   camLookAt;
    QVector3D   camUp;
    qreal       camAngle;
    qreal       camAspect;
    qreal       camNear;
    qreal       camFar;

    QPoint lastPos;
    
    QTimer              *animTimer;
    unsigned long int   animCount;

    GLfloat mvp[16];

    GLfloat             *cube_vertices;
    long unsigned int   cube_vertices_count;
    GLushort            *cube_indices;
    long unsigned int   cube_indices_count;
    GLfloat             *cube_colors;
    long unsigned int   cube_colors_count;
    GLfloat             *cube_normals;
    long unsigned int   cube_normals_count;
    GLuint vbo_cube_vertices, vbo_cube_normals, vbo_cube_colors, ibo_cube_indices;

    QVector<QVector3D>  monkeyVertices;
    QVector<QVector3D> monkeyVertexNormals;
    QVector<GLushort> monkeyElements;

    GLfloat             *triangle_vertices;
    long unsigned int   triangle_vertices_count;

    GLuint program;
    GLint attribute_coord3d, attribute_v_color, uniform_mvp;

    GLuint vbo_triangle_attributes;

    void createObj(const char *filename);
    void createGeom();

    void initShadedTriangle();
    void drawShadedTriangle() const;

    void initShadedBufferedBox();
    void drawShadedBufferedBox() const;
    void animShadedBufferedBox();

    void initShadedBox();
    void drawShadedBox() const;

    void initBufferedBox();
    void drawBufferedBox() const;

    void initUnbuffered();
    void drawUnbuffered() const;

    void initBox();
    void drawBox() const;

    void initBoxy();
    void drawBoxy() const ;

    void setMvp();

    void startAnim();
    void stopAnim();
    void animGLUpdate();

};

#endif
