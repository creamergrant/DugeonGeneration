#include "Graphics.h"
#include "../Application/Application.h"
#include <glad/glad.h>


namespace GameDev2D
{
    Graphics::Graphics() :
        m_MainCamera(Viewport(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight())),
        m_ClearColor(ColorList::Black)
    {
        EnableBlending(BlendingMode(BlendingFactor::SourceAlpha, BlendingFactor::OneMinusSourceAlpha));
    }

    unsigned int Graphics::GetApiVersion()
    {
        float version;
        sscanf((char *)glGetString(GL_VERSION), "%f", &version);
        return (unsigned int)(100 * version);
    }

    unsigned int Graphics::GetShadingLanguageVersion()
    {
        float version;
        sscanf((char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "%f", &version);
        return (unsigned int)(100 * version);
    }
    
    void Graphics::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Graphics::SetClearColor(Color aClearColor)
    {
        m_ClearColor = aClearColor;
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
    }

    Color Graphics::GetClearColor()
    {
        return m_ClearColor;
    }

    Camera* Graphics::GetMainCamera()
    {
        return &m_MainCamera;
    }

    Camera* Graphics::GetActiveCamera()
    {
        if (m_CameraStack.size() > 0)
        {
            return &m_CameraStack.back();
        }
        return GetMainCamera();
    }

	void Graphics::PushCamera(const Camera& aCamera)
	{
		m_CameraStack.push_back(Camera(aCamera));
		SetViewport(GetActiveCamera()->GetViewport());
	}

	void Graphics::PopCamera()
	{
		m_CameraStack.pop_back();
		SetViewport(GetActiveCamera()->GetViewport());
	}
    
    void Graphics::EnableBlending(BlendingMode aBlendingMode)
    {
        GLenum source = GetOpenGLBlendingFactor(aBlendingMode.source);
        GLenum destination = GetOpenGLBlendingFactor(aBlendingMode.destination);
        glBlendFunc(source, destination);
        glEnable(GL_BLEND);
    }

    void Graphics::DisableBlending()
    {
        glDisable(GL_BLEND);
    }

    void Graphics::SetViewport(const Viewport& viewport)
    {
        glViewport((int)viewport.x, (int)viewport.y, (int)viewport.width, (int)viewport.height);
    }

    void Graphics::PushScissorClip(float aX, float aY, float aWidth, float aHeight)
    {
        if (m_ScissorStack.size() == 0)
        {
            glEnable(GL_SCISSOR_TEST);
        }

        m_ScissorStack.push_back(Rect(Vector2(aX, aY), Vector2(aWidth, aHeight)));
        ResetScissorClip();
    }

    void Graphics::PopScissorClip()
    {
        m_ScissorStack.pop_back();
        ResetScissorClip();
    }

    void Graphics::ResetScissorClip()
    {
        //Is there still clipping rects on the scissor stack?
        if (m_ScissorStack.size() > 0)
        {
            Vector2 position = m_ScissorStack.back().origin;
            Vector2 size = m_ScissorStack.back().size;
            glScissor((int)position.x, (int)position.y, (int)size.x, (int)size.y);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }
    }

    void Graphics::DrawElements(RenderMode renderMode, const std::shared_ptr<VertexArray>& vertexArray, unsigned int indexCount)
    {
        unsigned int rm = GetOpenGLRenderMode(renderMode);
        unsigned int count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(rm, count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void Graphics::CreateTexture(unsigned int* id, int count)
    {
        glCreateTextures(GL_TEXTURE_2D, count, id);
    }

    void Graphics::DeleteTexture(unsigned int* id, int count)
    {
        glDeleteTextures(count, id);
    }

    void Graphics::BindTextureUnit(unsigned int id, unsigned int slot)
    {
        glBindTextureUnit(slot, id);
    }

    void Graphics::SetTextureStorage(unsigned int id, unsigned int width, unsigned int height, PixelFormat internalFormat, unsigned int levels)
    {
        glTextureStorage2D(id, levels, GetOpenGLInternalPixelFormat(internalFormat), width, height);
    }

    void Graphics::SetTextureFilter(unsigned int id, TextureMinFilter minFilter, TextureMagFilter magFilter)
    {
        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GetOpenGLTextureMinFilter(minFilter));
        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GetOpenGLTextureMagFilter(magFilter));
    }

    void Graphics::SetTextureData(unsigned int id, const ImageData& imageData)
    {
        assert(imageData.width > 0 && imageData.height > 0);
        unsigned int pixelLayout = GetOpenGLPixelLayout(imageData.format.layout);
        unsigned int pixelSize = GetOpenGLPixelSize(imageData.format.size);
        glTextureSubImage2D(id, 0, 0, 0, imageData.width, imageData.height, pixelLayout, pixelSize, &imageData.data[0]);
    }

    void Graphics::SetTextureWrap(unsigned int id, Wrap wrap)
    {
        int wrapModeS = GetOpenGLWrapMode(wrap.s);
        int wrapModeT = GetOpenGLWrapMode(wrap.t);
        glTextureParameteri(id, GL_TEXTURE_WRAP_S, wrapModeS);
        glTextureParameteri(id, GL_TEXTURE_WRAP_T, wrapModeT);
    }

    void Graphics::BindFrameBuffer(unsigned int frameBuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    }

    void Graphics::GenerateFrameBuffer(unsigned int* frameBuffer)
    {
        glGenFramebuffers(1, frameBuffer);
    }

    void Graphics::DeleteFrameBuffer(unsigned int* frameBuffer)
    {
        glDeleteFramebuffers(1, frameBuffer);
    }

    void Graphics::SetFrameBufferStorage(unsigned int frameBuffer, unsigned int textureId)
    {
        //TODO:
    }

    void Graphics::CopyFrameBufferContents(unsigned int aFrameBuffer, PixelFormat aFormat, unsigned int aWidth, unsigned int aHeight, unsigned char* aBuffer)
    {
        //TODO:
    }
    
    void Graphics::CreateBuffer(unsigned int* id, int count)
    {
        glCreateBuffers(count, id);
    }

    void Graphics::DeleteBuffer(unsigned int* id, int count)
    {
        glDeleteBuffers(count, id);
    }

    void Graphics::BindBuffer(BufferType bufferType, unsigned int id)
    {
        glBindBuffer(GetOpenGLBufferType(bufferType), id);
    }

    void Graphics::InitBuffer(BufferType bufferType, unsigned int size, const void* data, BufferUsage usage)
    {
        glBufferData(GetOpenGLBufferType(bufferType), (GLsizeiptr)size, data, GetOpenGLBufferUsage(usage));
    }

    void Graphics::UpdateBuffer(BufferType bufferType, unsigned int offset, unsigned int size, const void* data)
    {
        glBufferSubData(GetOpenGLBufferType(bufferType), (GLintptr)offset, (GLsizeiptr)size, data);
    }

    void Graphics::CreateVertexArray(unsigned int* id, int count)
    {
        glCreateVertexArrays(count, id);
    }

    void Graphics::DeleteVertexArray(unsigned int* id, int count)
    {
        glDeleteVertexArrays(count, id);
    }

    void Graphics::BindVertexArray(unsigned int id)
    {
        glBindVertexArray(id);
    }

    void Graphics::EnableVertexAttributeArray(unsigned int vertexBuffer)
    {
        glEnableVertexAttribArray(vertexBuffer);
    }

    void Graphics::SetVertexAttributePointer(unsigned int vertexBuffer, const BufferElement& element, int stride, const void* offset)
    {
        GLenum type = GetShaderDataType(element.type);
        GLboolean normalized = element.normalized ? GL_TRUE : GL_FALSE;
        glVertexAttribPointer(vertexBuffer, element.GetComponentCount(), type, normalized, stride, offset);
    }

    void Graphics::SetVertexAttributeDivisor(unsigned int vertexBuffer, unsigned int divisor)
    {
        glVertexAttribDivisor(vertexBuffer, divisor);
    }

    unsigned int Graphics::CreateShaderProgram()
    {
        return glCreateProgram();
    }

    unsigned int Graphics::CreateShader(ShaderType shaderType)
    {
        return glCreateShader(GetOpenGLShaderType(shaderType));
    }

    void Graphics::SetShaderSource(unsigned int shader, const std::string& source)
    {
        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);
    }

    void Graphics::CompileShader(unsigned int shader)
    {
        glCompileShader(shader);
    }

    void Graphics::DeleteShader(unsigned int shader)
    {
        glDeleteShader(shader);
    }

    bool Graphics::IsShaderCompiled(unsigned int shader)
    {
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        return isCompiled == GL_TRUE;
    }

    std::string Graphics::GetShaderInfoLog(unsigned int shader)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

        return std::string(infoLog.begin(), infoLog.end());
    }

    void Graphics::AttachShader(unsigned int program, unsigned int shader)
    {
        glAttachShader(program, shader);
    }

    void Graphics::DetachShader(unsigned int program, unsigned int shader)
    {
        glDetachShader(program, shader);
    }

    void Graphics::LinkShaderProgram(unsigned int program)
    {
        glLinkProgram(program);
    }

    bool Graphics::IsShaderProgramLinked(unsigned int program)
    {
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        return isLinked == GL_TRUE;
    }

    void Graphics::DeleteShaderProgram(unsigned int program)
    {
        glDeleteProgram(program);
    }

    void Graphics::UseShaderProgram(unsigned int program)
    {
        glUseProgram(program);
    }

    std::string Graphics::GetShaderProgramInfoLog(unsigned int program)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        return std::string(infoLog.begin(), infoLog.end());
    }

    int Graphics::GetUniformLocation(unsigned int program, const std::string& uniform)
    {
        return glGetUniformLocation(program, uniform.c_str());
    }

    int Graphics::GetAttributeLocation(unsigned int program, const std::string& attribute)
    {
        return glGetAttribLocation(program, attribute.c_str());
    }

    void Graphics::SetUniformInt(int location, int value)
    {
        glUniform1i(location, value);
    }

    void Graphics::SetUniformIntArray(int location, int* values, unsigned int count)
    {
        glUniform1iv(location, count, values);
    }

    void Graphics::SetUniformFloat(int location, float value)
    {
        glUniform1f(location, value);
    }

    void Graphics::SetUniformFloat2(int location, float value1, float value2)
    {
        glUniform2f(location, value1, value2);
    }

    void Graphics::SetUniformFloat3(int location, float value1, float value2, float value3)
    {
        glUniform3f(location, value1, value2, value3);
    }

    void Graphics::SetUniformFloat4(int location, float value1, float value2, float value3, float value4)
    {
        glUniform4f(location, value1, value2, value3, value4);
    }

    void Graphics::SetUniformMatrix(int location, const Matrix& value)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value.m[0][0]);
    }

    unsigned int Graphics::GetOpenGLGraphicType(GraphicType aType)
    {
        unsigned int types[] = { GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE };
        int index = static_cast<int>(aType);
        return types[index];
    }

    unsigned int Graphics::GetOpenGLTextureMinFilter(TextureMinFilter minFilter)
    {
        unsigned int types[] = { GL_NEAREST, GL_LINEAR };
        int index = static_cast<int>(minFilter);
        return types[index];
    }

    unsigned int Graphics::GetOpenGLTextureMagFilter(TextureMagFilter magFilter)
    {
        unsigned int types[] = { GL_NEAREST, GL_LINEAR };
        int index = static_cast<int>(magFilter);
        return types[index];
    }

    unsigned int Graphics::GetOpenGLPixelLayout(PixelLayout aLayout)
    {
        unsigned int layouts[] = { GL_RGB, GL_RGBA };
        int index = static_cast<int>(aLayout);
        return layouts[index];
    }

    unsigned int Graphics::GetOpenGLPixelSize(PixelSize aSize)
    {
        unsigned int sizes[] = { GL_UNSIGNED_BYTE };
        int index = static_cast<int>(aSize);
        return sizes[index];
    }

    unsigned int Graphics::GetOpenGLInternalPixelFormat(PixelFormat pixeFormat)
    {
        unsigned int layouts[] = { GL_RGB8, GL_RGBA8 };
        int index = static_cast<int>(pixeFormat.layout);
        return layouts[index];
    }

    unsigned int Graphics::GetOpenGLBlendingFactor(BlendingFactor aBlendingFactor)
    {
        GLenum blendingFactors[] = { GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA_SATURATE };
        int index = static_cast<int>(aBlendingFactor);
        return blendingFactors[index];
    }

    int Graphics::GetOpenGLWrapMode(WrapMode aWrapMode)
    {
        GLint wrapModes[] = { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_MIRRORED_REPEAT };
        int index = static_cast<int>(aWrapMode);
        return wrapModes[index];
    }

    unsigned int Graphics::GetOpenGLRenderMode(RenderMode aRenderMode)
    {
        unsigned int renderModes[] = { GL_POINTS, GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_LINES_ADJACENCY, GL_LINE_STRIP_ADJACENCY };
        int index = static_cast<int>(aRenderMode);
        return renderModes[index];
    }

    unsigned int Graphics::GetOpenGLBufferUsage(BufferUsage aBufferUsage)
    {
        unsigned int bufferUsages[] = { GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY };
        int index = static_cast<int>(aBufferUsage);
        return bufferUsages[index];
    }

    unsigned int Graphics::GetOpenGLBufferType(BufferType aBufferType)
    {
        unsigned int bufferTypes[] = { GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER };
        int index = static_cast<int>(aBufferType);
        return bufferTypes[index];
    }

    unsigned int Graphics::GetOpenGLShaderType(ShaderType aShaderType)
    {
        GLenum shaderTypes[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER };
        int index = static_cast<int>(aShaderType);
        return shaderTypes[index];
    }

    unsigned int Graphics::GetShaderDataType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:    return GL_FLOAT;
        case ShaderDataType::Float2:   return GL_FLOAT;
        case ShaderDataType::Float3:   return GL_FLOAT;
        case ShaderDataType::Float4:   return GL_FLOAT;
        case ShaderDataType::Mat3:     return GL_FLOAT;
        case ShaderDataType::Mat4:     return GL_FLOAT;
        case ShaderDataType::Int:      return GL_INT;
        case ShaderDataType::Int2:     return GL_INT;
        case ShaderDataType::Int3:     return GL_INT;
        case ShaderDataType::Int4:     return GL_INT;
        case ShaderDataType::Bool:     return GL_BOOL;
        }

        return 0;
    }
}
