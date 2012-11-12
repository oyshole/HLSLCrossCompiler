#include "Shader.h"
#include <GL/glew.h>
#include "debug.h"

ShaderEffect::ShaderEffect() : mCompileFlags(0), mLang(LANG_DEFAULT)
{
}

void ShaderEffect::Create()
{
    mProgram = glCreateProgram();
}

void ShaderEffect::FromByteFile(std::string& path)
{
    GLSLShader result;
    TranslateHLSLFromFile(path.c_str(), mCompileFlags, mLang, &result);

    switch(result.shaderType)
    {
        case GL_VERTEX_SHADER:
        {
            mVertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(mVertex, 1, (const char **)&result.sourceCode, 0);
            glCompileShader(mVertex);
            glAttachShader(mProgram, mVertex);
            break;
        }
        case GL_FRAGMENT_SHADER:
        {
            mPixel = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(mPixel, 1, (const char **)&result.sourceCode, 0);
            glCompileShader(mPixel);
            glAttachShader(mProgram, mPixel);
            break;
        }
        default:
        {
            break;
        }
    }
}

void ShaderEffect::FromVertexByteFile(std::string& path)
{
    GLSLShader result;
    TranslateHLSLFromFile(path.c_str(), mCompileFlags, mLang, &result);

    ASSERT(result.shaderType == GL_VERTEX_SHADER);

    mVertex = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(mVertex, 1, (const char **)&result.sourceCode, 0);
    glCompileShader(mVertex);

#ifdef _DEBUG
    GLint compiled = GL_FALSE;
    glGetShaderiv(mVertex, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        ASSERT(0);
    }
#endif

    glAttachShader(mProgram, mVertex);
}

void ShaderEffect::FromPixelByteFile(std::string& path)
{
    GLSLShader result;
    TranslateHLSLFromFile(path.c_str(), mCompileFlags, mLang, &result);

    ASSERT(result.shaderType == GL_FRAGMENT_SHADER);

    mPixel = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(mPixel, 1, (const char **)&result.sourceCode, 0);
    glCompileShader(mPixel);

#ifdef _DEBUG
    GLint compiled = GL_FALSE;
    glGetShaderiv(mPixel, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        ASSERT(0);
    }
#endif

    glAttachShader(mProgram, mPixel);
}

void ShaderEffect::FromGeometryByteFile(std::string& path)
{
    GLSLShader result;
    TranslateHLSLFromFile(path.c_str(), mCompileFlags, mLang, &result);

    ASSERT(result.shaderType == GL_GEOMETRY_SHADER);

    mGeometry = glCreateShader(GL_GEOMETRY_SHADER);

    glShaderSource(mGeometry, 1, (const char **)&result.sourceCode, 0);
    glCompileShader(mGeometry);

#ifdef _DEBUG
    GLint compiled = GL_FALSE;
    glGetShaderiv(mGeometry, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        ASSERT(0);
    }
#endif

    glAttachShader(mProgram, mGeometry);
}

void ShaderEffect::Enable()
{
    glBindAttribLocation(mProgram, 0, "Input0");
    glBindAttribLocation(mProgram, 1, "Input1");
    glBindFragDataLocation(mProgram, 0, "PixOutput0");
    glLinkProgram(mProgram);

#ifdef _DEBUG
    GLint linked = GL_FALSE;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        char* log;
        GLint length = 0;
        glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &length);
        log = new char[length];
        glGetProgramInfoLog(mProgram, length, NULL, log);
        ASSERT(0);
        delete [] log;
    }
#endif

    glUseProgram(mProgram);
}

void ShaderEffect::SetVec4(std::string& name, int count, float* v) {
    int loc = glGetUniformLocation(mProgram, (name + std::string("VS")).c_str());
    glUniform4fv(loc, count, v);

    loc = glGetUniformLocation(mProgram, (name + std::string("PS")).c_str());
    glUniform4fv(loc, count, v);

    loc = glGetUniformLocation(mProgram, (name + std::string("GS")).c_str());
    glUniform4fv(loc, count, v);
}

void ShaderEffect::CreateUniformBlock(std::string& name, uint_t& ubo)
{
	GLuint bufferID;

    glGenBuffers(1, &bufferID);

    uint_t uniformBlockIndex = glGetUniformBlockIndex(mProgram, name.c_str());
        
	GLint uniformBlockSize;
    //We need to get the uniform block's size in order to back it with the
    //appropriate buffer
    glGetActiveUniformBlockiv(mProgram, uniformBlockIndex,
                                    GL_UNIFORM_BLOCK_DATA_SIZE,
                                    &uniformBlockSize);

    //Create UBO.
    glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
    glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize,
                    NULL, GL_DYNAMIC_DRAW);

	ubo = bufferID;
}

void ShaderEffect::SetUniformBlock(std::string& name, uint_t bufIndex)
{
    int uniformIndex = glGetUniformBlockIndex(mProgram, (name + std::string("VS")).c_str());
    glUniformBlockBinding(mProgram, uniformIndex, bufIndex);

    uniformIndex = glGetUniformBlockIndex(mProgram, (name + std::string("GS")).c_str());
    glUniformBlockBinding(mProgram, uniformIndex, bufIndex);

    uniformIndex = glGetUniformBlockIndex(mProgram, (name + std::string("PS")).c_str());
    glUniformBlockBinding(mProgram, uniformIndex, bufIndex);
}

void ShaderEffect::SetUniformBlock(std::string& name, uint_t bufIndex, uint_t ubo)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, bufIndex, ubo);
	SetUniformBlock(name, bufIndex);
}
