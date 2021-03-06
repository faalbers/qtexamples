#include "ShaderUtils.h"

char* fileRead(const char* filename)
{
    FILE* input = fopen(filename, "rb");
    if(input == NULL) return NULL;

    if(fseek(input, 0, SEEK_END) == -1) return NULL;
    long size = ftell(input);
    if(size == -1) return NULL;
    if(fseek(input, 0, SEEK_SET) == -1) return NULL;

    /*if using c-compiler: dont cast malloc's return value*/
    char *content = (char*) malloc( (size_t) size +1  ); 
    if(content == NULL) return NULL;

    fread(content, 1, (size_t)size, input);
    if(ferror(input)) {
        free(content);
        return NULL;
    }

    fclose(input);
    content[size] = '\0';
    return content;
}

void glLog(GLuint object)
{
    GLint log_length = 0;
    if (glIsShader(object)) {
        printf("\nShader log:\n");
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else if (glIsProgram(object)) {
        printf("\nProgram log:\n");
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else {
        printf("printlog: Not a shader or a program\n");
        return;
    }

    char* log = (char*)malloc(log_length);

    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);

    printf("%s", log);
    free(log);
}

GLuint createShader(const char* filename, GLenum type)
{
    const GLchar* source = fileRead(filename);
    if (source == NULL) {
        printf("Error opening %s: ", filename); perror("");
        return 0;
    }
    GLuint res = glCreateShader(type);
    const GLchar* sources[2] = {
#ifdef GL_ES_VERSION_2_0
        "#version 100\n"
        "#define GLES2\n",
#else
        "#version 120\n",
#endif
        source };
    glShaderSource(res, 2, sources, NULL);
    free((void*)source);

    glCompileShader(res);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE) {
        printf("%s:", filename);
        glLog(res);
        glDeleteShader(res);
        return NULL;
    }
    return res;
}
