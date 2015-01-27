#include <math.h>
#include <cstdio>
#include "GLWidget.h"
#include <QTimer>
#include <QMouseEvent>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    //xRot = 0;
    //yRot = 0;
    //zRot = 0;

    createObj("/home/afrank/trees/users/afrank/tests/bin/qtopenglbox/monkeyb.obj");
    //createGeom();

    modelPos    = QVector3D(0.0, 0.0, -7.0);
    modelRot    = QVector3D(0.0, 0.0, 0.0);
    modelScl    = QVector3D(1.0, 1.0, 1.0);
    camPos      = QVector3D(0.0, 2.0, 0.0);
    camLookAt   = modelPos;
    camUp       = QVector3D(0.0, 1.0, 0.0);
    camAngle    = 35.0;
    camAspect   = 1.0;
    camNear     = 1.0;
    camFar      = 15.0;

    animTimer = new QTimer(this);
    animCount = 0;

    connect(animTimer, SIGNAL(timeout()), this, SLOT(animUpdate()));
    startAnim();
}

GLWidget::~GLWidget()
{
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_cube_vertices);
    glDeleteBuffers(1, &vbo_cube_normals);
    glDeleteBuffers(1, &vbo_cube_colors);
    glDeleteBuffers(1, &ibo_cube_indices);
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400,400);
}

static void qNormalizeAngle(qreal &angle)
{
    while (angle < 0.0)
        angle += 360.0;
    while (angle > 360.0)
        angle -= 360.0;
}

void GLWidget::setLMouseRotation(int dx, int dy)
{
    qreal xRot = modelRot.x() + (qreal) dy;
    qNormalizeAngle(xRot);
    modelRot.setX(xRot);

    qreal yRot = modelRot.y() + (qreal) dx;
    qNormalizeAngle(yRot);
    modelRot.setY(yRot);

    setMvp();
    updateGL();
}

void GLWidget::setRMouseRotation(int dx, int dy)
{
    qreal xRot = modelRot.x() + (qreal) dy;
    qNormalizeAngle(xRot);
    modelRot.setX(xRot);

    qreal zRot = modelRot.z() + (qreal) dx;
    qNormalizeAngle(zRot);
    modelRot.setZ(zRot);

    setMvp();
    updateGL();
}

// same as init_resources
void GLWidget::initializeGL()
{
    glClearColor(0.5, 0.5, 0.5, 1.0);

    //initShadedTriangle();
    //initShadedBufferedBox();
    //initShadedBox();
    //initBufferedBox();
    initUnbuffered();
    //initBox();
    //initBoxy();

    /*
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    */
}

// same as onDisplay
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //drawShadedTriangle();
    //drawShadedBufferedBox();
    //drawShadedBox();
    //drawBufferedBox();
    drawUnbuffered();
    //drawBox();
    //drawBoxy();
}

void GLWidget::animGLUpdate()
{
    animShadedBufferedBox();
}

static void loadObj(const char *filename, QVector<QVector3D> &vertices, QVector<QVector3D> &vertexNormals, QVector<GLushort> &elements)
{
    qreal x, y, z, w;
    GLushort vertexElement, quadElement;
    char cg;

    QVector<QVector3D> quadNormals;

    ifstream in(filename, ios::in);
    if (!in) {
        printf("Cannot open: %s\n", filename);
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.substr(0,2) == "v ") {
            // get vertex
            istringstream s(line.substr(2));
            s >> x >> y >> z; w = 1.0;
            //vertices.append(QVector4D(x, y, z, w));
            vertices.append(QVector3D(x, y, z));
            vertexNormals.append(QVector3D(0, 0, 0));
        } else if (line.substr(0,3) == "vn ") {
            // get quad normals
            istringstream s(line.substr(3));
            s >> x >> y >> z;
            quadNormals.append(QVector3D(x, y, z));
        }  else if (line.substr(0,2) == "f ") {
            // get face
            istringstream s(line.substr(2));

            // first id
            s >> vertexElement >> cg >> cg >> quadElement;
            vertexElement--; quadElement--;
            elements.append(vertexElement);
            vertexNormals[vertexElement] += quadNormals[quadElement];

            // second id
            s >> vertexElement >> cg >> cg >> quadElement;
            vertexElement--; quadElement--;
            elements.append(vertexElement);
            vertexNormals[vertexElement] += quadNormals[quadElement];

            // third id
            s >> vertexElement >> cg >> cg >> quadElement;
            vertexElement--; quadElement--;
            elements.append(vertexElement);
            vertexNormals[vertexElement] += quadNormals[quadElement];

        } else if (line[0] == '#') {
            // ignoring for now
        } else {
            // ignoring for now
        }
    }

    // normalize vertex normals
    for ( int vertex = 0; vertex < vertexNormals.count(); vertex++ )
        vertexNormals[vertex].normalize();

}

void GLWidget::createObj(const char *filename)
{
    loadObj(filename, vertices, vertexNormals, elements);
    printf("vertices count: %d\n", vertices.count());
    printf("vertex normals count: %d\n", vertexNormals.count());
    printf("elements count: %d\n", elements.count());
}

void GLWidget::createGeom()
{
    unsigned long int i;

    // cube vertices
    qreal cube_vertices[] = {
        // front
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // back
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
    };
    for ( i = 0; i < sizeof(cube_vertices)/sizeof(qreal); i += 3)
        vertices.append(QVector3D(cube_vertices[i+0], cube_vertices[i+1], cube_vertices[i+2]));

    // cube normals
    qreal cube_normals[] = {
        // front
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // back
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
    };
    for ( i = 0; i < sizeof(cube_normals)/sizeof(qreal); i += 3)
        vertexNormals.append(QVector3D(cube_normals[i+0], cube_normals[i+1], cube_normals[i+2]));

    // cube colors
    qreal cube_colors[] = {
        // front colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        // back colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
    };
    for ( i = 0; i < sizeof(cube_colors)/sizeof(qreal); i += 3)
        vertexColors.append(QVector3D(cube_colors[i+0], cube_colors[i+1], cube_colors[i+2]));

    // cube indices
    GLushort cube_elements[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3,
    };
    for ( i = 0; i < sizeof(cube_elements)/sizeof(GLushort); i ++)
        elements.append(cube_elements[i]);

    printf("vertices count: %d\n", vertices.count());
    printf("vertex normals count: %d\n", vertexNormals.count());
    printf("vertex colors count: %d\n", vertexColors.count());
    printf("elements count: %d\n", elements.count());
}

void GLWidget::initShadedTriangle()
{
    // Combined buffer for vertices and colors of triangle
    struct attributes triangle_attributes[] = {
        {{ 0.0,  0.8, 0.0}, {1.0, 1.0, 0.0}},
        {{-0.8, -0.8, 0.0}, {0.0, 0.0, 1.0}},
        {{ 0.8, -0.8, 0.0}, {1.0, 0.0, 0.0}},
    };
    glGenBuffers(1, &vbo_triangle_attributes);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_attributes);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // compile vertex and fragment shader
    GLuint vs, fs;
    if ((vs = createShader("/home/afrank/trees/users/afrank/tests/bin/qtopenglbox/triangle.v.glsl", GL_VERTEX_SHADER))   == 0) return;
    if ((fs = createShader("/home/afrank/trees/users/afrank/tests/bin/qtopenglbox/triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return;

    // create program
    GLint link_ok = GL_FALSE;
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        printf("glLinkProgram:");
        glLog(program);
    }

    // Find triangle vertices attribute location in the program
    attribute_coord3d = glGetAttribLocation(program, "coord3d");
    if (attribute_coord3d == -1)
        printf("Could not bind attribute coord3d\n");

    // Find triangle colors attribute location in the program
    attribute_v_color = glGetAttribLocation(program, "v_color");
    if (attribute_v_color == -1)
        printf("Could not bind attribute v_color\n");

    glEnable(GL_DEPTH_TEST);
}

void GLWidget::drawShadedTriangle() const
{
}

void GLWidget::initShadedBufferedBox()
{
    // buffer for cube vertices
    glGenBuffers(1, &vbo_cube_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glBufferData(GL_ARRAY_BUFFER, cube_vertices_count*sizeof(GLfloat), cube_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // buffer for cube colors
    glGenBuffers(1, &vbo_cube_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
    glBufferData(GL_ARRAY_BUFFER, cube_colors_count*sizeof(GLfloat), cube_colors, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // buffer for cube indices
    glGenBuffers(1, &ibo_cube_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_indices_count*sizeof(GLushort), cube_indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // compile vertex and fragment shader
    GLuint vs, fs;
    if ((vs = createShader("/home/afrank/trees/users/afrank/tests/bin/qtopenglbox/box.v.glsl", GL_VERTEX_SHADER))   == 0) return;
    if ((fs = createShader("/home/afrank/trees/users/afrank/tests/bin/qtopenglbox/box.f.glsl", GL_FRAGMENT_SHADER)) == 0) return;

    // create program
    GLint link_ok = GL_FALSE;
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        printf("glLinkProgram:");
        glLog(program);
    }

    // Set initial master matrix
    setMvp();

    // Find box vertices attribute location in the program
    attribute_coord3d = glGetAttribLocation(program, "coord3d");
    if (attribute_coord3d == -1)
        printf("Could not bind attribute coord3d\n");

    // Find box color attribute location in the program
    attribute_v_color = glGetAttribLocation(program, "v_color");
    if (attribute_v_color == -1)
        printf("Could not bind attribute v_color\n");

    // Find mvp uniform location in the program
    uniform_mvp = glGetUniformLocation(program, "mvp");
    if (uniform_mvp == -1)
        printf("Could not bind uniform mvp\n");

    glEnable(GL_DEPTH_TEST);
}

void GLWidget::drawShadedBufferedBox() const
{
    glUseProgram(program);

    // cube buffered vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glVertexAttribPointer(
        attribute_coord3d,  // attribute
        3,                  // number of elements per vertex, here (x,y,z)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // stride
        0                   // vertices data pointer
    );
    glEnableVertexAttribArray(attribute_coord3d);

    // cube buffered colors
/*
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
    glVertexAttribPointer(
        attribute_v_color,  // attribute
        3,                  // number of elements per vertex, here (r, g, b)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // stride
        0                   // vertices data pointer
    );
    glEnableVertexAttribArray(attribute_v_color);
*/
    // cube buffered indices and draw
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
    glDrawElements(
        GL_TRIANGLES,       // draw type
        cube_indices_count, // elements count
        GL_UNSIGNED_SHORT,  // type of elements
        0                   // elements pointer
    );

    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, mvp);

    glDisableVertexAttribArray(attribute_coord3d);
    //glDisableVertexAttribArray(attribute_v_color);
}

void GLWidget::animShadedBufferedBox()
{
    modelRot.setY(modelRot.y()+1.0);
    //modelRot.setX(modelRot.x()+0.5);
    //modelRot.setZ(modelRot.x()+0.25);
    setMvp();
}

void GLWidget::initShadedBox()
{
    // compile vertex and fragment shader
    GLuint vs, fs;
    if ((vs = createShader("/home/afrank/trees/users/afrank/tests/bin/qtopenglbox/box.v.glsl", GL_VERTEX_SHADER))   == 0) return;
    if ((fs = createShader("/home/afrank/trees/users/afrank/tests/bin/qtopenglbox/box.f.glsl", GL_FRAGMENT_SHADER)) == 0) return;

    // create program
    GLint link_ok = GL_FALSE;
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        printf("glLinkProgram:");
        glLog(program);
    }

    // Find box vertices attribute location in the program
    attribute_coord3d = glGetAttribLocation(program, "coord3d");
    if (attribute_coord3d == -1)
        printf("Could not bind attribute coord3d\n");

    // Find box color attribute location in the program
    attribute_v_color = glGetAttribLocation(program, "v_color");
    if (attribute_v_color == -1)
        printf("Could not bind attribute v_color\n");

    glEnable(GL_DEPTH_TEST);
}

void GLWidget::drawShadedBox() const
{
    glUseProgram(program);

    glVertexAttribPointer(
        attribute_coord3d,  // attribute
        3,                  // number of elements per vertex, here (x,y,z)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // stride
        cube_vertices       // vertices data pointer
    );
    glEnableVertexAttribArray(attribute_coord3d);

    glVertexAttribPointer(
        attribute_v_color,  // attribute
        3,                  // number of elements per vertex, here (r, g, b)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // stride
        cube_colors         // vertices data pointer
    );
    glEnableVertexAttribArray(attribute_v_color);

    glDrawElements(
        GL_TRIANGLES,       // draw type
        cube_indices_count, // elements count
        GL_UNSIGNED_SHORT,  // type of elements
        cube_indices        // elements pointer
    );

    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_v_color);
}

void GLWidget::initBufferedBox()
{
    // buffer for cube vertices
    glGenBuffers(1, &vbo_cube_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glBufferData(GL_ARRAY_BUFFER, cube_vertices_count*sizeof(GLfloat), cube_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // buffer for cube normals
    glGenBuffers(1, &vbo_cube_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);
    glBufferData(GL_ARRAY_BUFFER, cube_normals_count*sizeof(GLfloat), cube_normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // buffer for cube colors
    glGenBuffers(1, &vbo_cube_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
    glBufferData(GL_ARRAY_BUFFER, cube_colors_count*sizeof(GLfloat), cube_colors, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // buffer for cube indices
    glGenBuffers(1, &ibo_cube_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_indices_count*sizeof(GLushort), cube_indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.0, 0.0, 20, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLWidget::drawBufferedBox() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camAngle, camAspect, camNear, camFar);
    gluLookAt(camPos.x(), camPos.y(), camPos.z(), camLookAt.x(), camLookAt.y(), camLookAt.z(), camUp.x(), camUp.y(), camUp.z());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(modelPos.x(), modelPos.y(), modelPos.z());
    glRotatef(modelRot.x(), 1.0, 0.0, 0.0);
    glRotatef(modelRot.y(), 0.0, 1.0, 0.0);
    glRotatef(modelRot.z(), 0.0, 0.0, 1.0);

    // cube buffered vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glVertexPointer(
        3,              // number of elements per vertex, here (x,y,z)
        GL_FLOAT,       // the type of each element
        0,              // stride
        0               // vertices data pointer
    );
    glEnableClientState(GL_VERTEX_ARRAY);

    // cube buffered colors
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
    glColorPointer(
        3,              // color channels per vertex, here (r,g,b)
        GL_FLOAT,       // the type of each element
        0,              // stride
        0               // vertices data pointer
    );
    glEnableClientState(GL_COLOR_ARRAY);

    // cube buffered normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);
    glNormalPointer(
        GL_FLOAT,
        0,
        0
    );
    glEnableClientState(GL_NORMAL_ARRAY);

    // cube buffered indices and draw
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
    glDrawElements(
        GL_TRIANGLES,       // draw type
        cube_indices_count, // elements count
        GL_UNSIGNED_SHORT,  // type of elements
        0                   // elements pointer
    );

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void GLWidget::initUnbuffered()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 100.0, 100.0, 100.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLWidget::drawUnbuffered() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camAngle, camAspect, camNear, camFar);
    gluLookAt(camPos.x(), camPos.y(), camPos.z(), camLookAt.x(), camLookAt.y(), camLookAt.z(), camUp.x(), camUp.y(), camUp.z());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(modelPos.x(), modelPos.y(), modelPos.z());
    glRotatef(modelRot.x(), 1.0, 0.0, 0.0);
    glRotatef(modelRot.y(), 0.0, 1.0, 0.0);
    glRotatef(modelRot.z(), 0.0, 0.0, 1.0);

    glVertexPointer(
        3,              // number of elements per vertex, here (x,y,z)
        GL_FLOAT,       // the type of each element
        0,              // stride
        vertices.constData()   // vertices data pointer
    );
    //printf("%f %f %f\n", ((GLfloat*) vertices.constData())[3], ((GLfloat*) vertices.constData())[4], ((GLfloat*) vertices.constData())[5]);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(
        GL_FLOAT,
        0,
        vertexNormals.constData()
    );
    glEnableClientState(GL_NORMAL_ARRAY);

    glDrawElements(
        GL_TRIANGLES,       // draw type
        elements.count(), // elements count
        GL_UNSIGNED_SHORT,  // type of elements
        elements.constData()       // elements pointer
    );

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void GLWidget::initBox()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.0, 0.0, 20, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLWidget::drawBox() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camAngle, camAspect, camNear, camFar);
    gluLookAt(camPos.x(), camPos.y(), camPos.z(), camLookAt.x(), camLookAt.y(), camLookAt.z(), camUp.x(), camUp.y(), camUp.z());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(modelPos.x(), modelPos.y(), modelPos.z());
    glRotatef(modelRot.x(), 1.0, 0.0, 0.0);
    glRotatef(modelRot.y(), 0.0, 1.0, 0.0);
    glRotatef(modelRot.z(), 0.0, 0.0, 1.0);

    //glScalef(0.25, 0.25, 0.25);

    glVertexPointer(
        3,              // number of elements per vertex, here (x,y,z)
        GL_FLOAT,       // the type of each element
        0,              // stride
        cube_vertices   // vertices data pointer
    );
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(
        GL_FLOAT,
        0,
        cube_normals
    );
    glEnableClientState(GL_NORMAL_ARRAY);

    glDrawElements(
        GL_TRIANGLES,       // draw type
        cube_indices_count, // elements count
        GL_UNSIGNED_SHORT,  // type of elements
        cube_indices        // elements pointer
    );

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void GLWidget::initBoxy()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void GLWidget::drawBoxy() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camAngle, camAspect, camNear, camFar);
    gluLookAt(camPos.x(), camPos.y(), camPos.z(), camLookAt.x(), camLookAt.y(), camLookAt.z(), camUp.x(), camUp.y(), camUp.z());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(modelPos.x(), modelPos.y(), modelPos.z());
    glRotatef(modelRot.x(), 1.0, 0.0, 0.0);
    glRotatef(modelRot.y(), 0.0, 1.0, 0.0);
    glRotatef(modelRot.z(), 0.0, 0.0, 1.0);

    GLfloat cube_vertices[] = {
        -0.125000, -0.056568, 0.050000,
        0.125000, -0.056568, 0.050000,
        0.125000, 0.056568, 0.050000,
        -0.125000, -0.056568, 0.050000,
        0.125000, 0.056568, 0.050000,
        -0.125000, 0.056568, 0.050000,
        -0.125000, 0.056568, -0.050000,
        0.125000, 0.056568, -0.050000,
        0.125000, -0.056568, -0.050000,
        -0.125000, 0.056568, -0.050000,
        0.125000, -0.056568, -0.050000,
        -0.125000, -0.056568, -0.050000,
        -0.125000, -0.056568, -0.050000,
        0.125000, -0.056568, -0.050000,
        0.125000, -0.056568, 0.050000,
        -0.125000, -0.056568, -0.050000,
        0.125000, -0.056568, 0.050000,
        -0.125000, -0.056568, 0.050000,
        0.125000, -0.056568, -0.050000,
        0.125000, 0.056568, -0.050000,
        0.125000, 0.056568, 0.050000,
        0.125000, -0.056568, -0.050000,
        0.125000, 0.056568, 0.050000,
        0.125000, -0.056568, 0.050000,
        0.125000, 0.056568, -0.050000,
        -0.125000, 0.056568, -0.050000,
        -0.125000, 0.056568, 0.050000,
        0.125000, 0.056568, -0.050000,
        -0.125000, 0.056568, 0.050000,
        0.125000, 0.056568, 0.050000,
        -0.125000, 0.056568, -0.050000,
        -0.125000, -0.056568, -0.050000,
        -0.125000, -0.056568, 0.050000,
        -0.125000, 0.056568, -0.050000,
        -0.125000, -0.056568, 0.050000,
        -0.125000, 0.056568, 0.050000
    };
    glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
    GLfloat cube_normals[] = {
        -0.125000, -0.056568, 0.050000,
        0.125000, -0.056568, 0.050000,
        0.125000, 0.056568, 0.050000,
        -0.125000, -0.056568, 0.050000,
        0.125000, 0.056568, 0.050000,
        -0.125000, 0.056568, 0.050000,
        -0.125000, 0.056568, -0.050000,
        0.125000, 0.056568, -0.050000,
        0.125000, -0.056568, -0.050000,
        -0.125000, 0.056568, -0.050000,
        0.125000, -0.056568, -0.050000,
        -0.125000, -0.056568, -0.050000,
        -0.125000, -0.056568, -0.050000,
        0.125000, -0.056568, -0.050000,
        0.125000, -0.056568, 0.050000,
        -0.125000, -0.056568, -0.050000,
        0.125000, -0.056568, 0.050000,
        -0.125000, -0.056568, 0.050000,
        0.125000, -0.056568, -0.050000,
        0.125000, 0.056568, -0.050000,
        0.125000, 0.056568, 0.050000,
        0.125000, -0.056568, -0.050000,
        0.125000, 0.056568, 0.050000,
        0.125000, -0.056568, 0.050000,
        0.125000, 0.056568, -0.050000,
        -0.125000, 0.056568, -0.050000,
        -0.125000, 0.056568, 0.050000,
        0.125000, 0.056568, -0.050000,
        -0.125000, 0.056568, 0.050000,
        0.125000, 0.056568, 0.050000,
        -0.125000, 0.056568, -0.050000,
        -0.125000, -0.056568, -0.050000,
        -0.125000, -0.056568, 0.050000,
        -0.125000, 0.056568, -0.050000,
        -0.125000, -0.056568, 0.050000,
        -0.125000, 0.056568, 0.050000
    };
    glNormalPointer(GL_FLOAT, 0, cube_normals);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    GLdouble mat[] = {
        0.707107, 0.707107, 0.000000, 0.000000, 
        -0.707107, 0.707107, 0.000000, 0.000000, 
        0.000000, 0.000000, 1.000000, 0.000000, 
        0.000000, 0.000000, 0.000000, 1.000000
    };
    GLfloat faceColor[] = {
        1.0, 1.0, 0.0, 1.0
    };
    GLushort indices[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };

    glPushMatrix();
    glMultMatrixd(mat);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, faceColor);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, indices + 0);
    glPopMatrix();

    glPushMatrix();
    glMultMatrixd(mat);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, faceColor);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_SHORT, indices + 12);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    /*
    gluLookAt(camPos.x(), camPos.y(), camPos.z(), camLookAt.x(), camLookAt.y(), camLookAt.z(), camUp.x(), camUp.y(), camUp.z());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
    //glOrtho(-2.0, +2.0, -2.0, +2.0, 4.0, 15.0);
    gluPerspective(camAngle, camAspect, camNear, camFar);
#endif
    glMatrixMode(GL_MODELVIEW);
    */
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    stopAnim();
    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    startAnim();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if ( dx == 0 and dy == 0 )
        return;

    if (event->buttons() & Qt::LeftButton) {
        setLMouseRotation(dx, dy);
    } else if (event->buttons() & Qt::RightButton) {
        setRMouseRotation(dx, dy);
    }

    lastPos = event->pos();
}

void GLWidget::startAnim()
{
    animTimer->start(10);
}

void GLWidget::stopAnim()
{
    animTimer->stop();
}

void GLWidget::animUpdate()
{
    animTimer->stop();
    animCount += 1;

    animGLUpdate();

    updateGL();
    animTimer->start();
}

void GLWidget::setMvp()
{
    // model matrix
    QMatrix4x4 model;
    model.translate(modelPos);
    if ( modelRot.x() >= 360 )
        modelRot.setX(modelRot.x()-360.0);
    model.rotate(modelRot.x(), QVector3D(1.0, 0.0, 0.0));
    if ( modelRot.y() >= 360 )
        modelRot.setY(modelRot.y()-360.0);
    model.rotate(modelRot.y(), QVector3D(0.0, 1.0, 0.0));
    if ( modelRot.z() >= 360 )
        modelRot.setZ(modelRot.z()-360.0);
    model.rotate(modelRot.z(), QVector3D(0.0, 0.0, 1.0));

    // view matrix
    QMatrix4x4 view;
    view.lookAt(camPos, camLookAt, camUp);

    // projection matrix
    QMatrix4x4 projection; projection.perspective(camAngle, camAspect, camNear, camFar);

    // final mat and copy to mvp
    GLdouble *mat = (GLdouble*) (projection * view * model).constData();
    copy(mat, mat + 16, mvp);
}
