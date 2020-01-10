#if 0
static guint model_view_create_shader(int shader_type, const char *source)
{
    int   status = 0;
    guint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        char *buffer = NULL;
        int  log_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
        buffer = g_malloc(log_len + 1);
        glGetShaderInfoLog(shader, log_len, NULL, buffer);
        logBoxTrace(LOGBOX_ERROR, "MODELVIEW load shader ERROR: [%d] %s\n", status, buffer);
        g_free(buffer);
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

static void model_view_init_shaders(void)
{
    GBytes     *source = NULL;
    int         status = 0;
    guint vertexShader = 0;
    guint   fragShader = 0;
    source = g_resources_lookup_data("/wcd/launchpad/def-shader-vertex", 0, NULL);
    vertexShader = model_view_create_shader(GL_VERTEX_SHADER, g_bytes_get_data(source, NULL));
    g_bytes_unref(source);
    source = g_resources_lookup_data("/wcd/launchpad/def-shader-fragment", 0, NULL);
    fragShader = model_view_create_shader(GL_FRAGMENT_SHADER, g_bytes_get_data(source, NULL));
    g_bytes_unref(source);
    if (vertexShader || fragShader) {
        defGLProgram = glCreateProgram();
        glAttachShader(defGLProgram, vertexShader);
        glAttachShader(defGLProgram, fragShader);
        glLinkProgram(defGLProgram);
        glGetProgramiv(defGLProgram, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            char *buffer = NULL;
            int  log_len = 0;
            glGetProgramiv(defGLProgram, GL_INFO_LOG_LENGTH, &log_len);
            buffer = g_malloc(log_len + 1);
            glGetProgramInfoLog(defGLProgram, log_len, NULL, buffer);
            logBoxTrace(LOGBOX_ERROR, "MODELVIEW link GL program ERROR: [%d] %s\n", status, buffer);
            g_free (buffer);
            glDeleteProgram (defGLProgram);
            defGLProgram = 0;
        }
        else {
            defMvpLocation = glGetUniformLocation(defGLProgram, "mvp");
            defPositionLocation = glGetAttribLocation (defGLProgram, "position");
            defColorLocation = glGetAttribLocation (defGLProgram, "color");
            glDetachShader(defGLProgram, vertexShader);
            glDetachShader(defGLProgram, fragShader);
        }
    }
    if (vertexShader) {
        glDeleteShader(vertexShader);
    }
    if (fragShader) {
        glDeleteShader(fragShader);
    }
}
#endif
