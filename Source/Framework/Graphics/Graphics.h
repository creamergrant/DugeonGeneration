#pragma once

#include "GraphicTypes.h"
#include "Buffer.h"
#include "Color.h"
#include "Camera.h"
#include "VertexArray.h"


namespace GameDev2D
{
    class Graphics
    {
    public:
        Graphics();
        ~Graphics() = default;
        
        unsigned int GetApiVersion();
        unsigned int GetShadingLanguageVersion();

        void Clear();
        void SetClearColor(Color clearColor);
        Color GetClearColor();

        Camera* GetMainCamera();
        Camera* GetActiveCamera();
		void PushCamera(const Camera& camera);
		void PopCamera();

        void SetViewport(const Viewport& viewport);

        void EnableBlending(BlendingMode aBlendingMode);
        void DisableBlending();

        void PushScissorClip(float x, float y, float width, float height);
        void PopScissorClip();
        void ResetScissorClip();

        void DrawElements(RenderMode renderMode, const std::shared_ptr<VertexArray>& vertexArray, unsigned int indexCount);

        void CreateTexture(unsigned int* id, int count = 1);
        void DeleteTexture(unsigned int* id, int count = 1);
        void BindTextureUnit(unsigned int id, unsigned int slot);
        void SetTextureStorage(unsigned int id, unsigned int width, unsigned int height, PixelFormat internalFormat, unsigned int levels = 1);
        void SetTextureFilter(unsigned int id, TextureMinFilter minFilter, TextureMagFilter magFilter);
        void SetTextureData(unsigned int id, const ImageData& imageData);
        void SetTextureWrap(unsigned int id, Wrap wrap);

        void BindFrameBuffer(unsigned int frameBuffer);
        void GenerateFrameBuffer(unsigned int* frameBuffer);
        void DeleteFrameBuffer(unsigned int* frameBuffer);
        void SetFrameBufferStorage(unsigned int frameBuffer, unsigned int textureId);
        void CopyFrameBufferContents(unsigned int frameBuffer, PixelFormat format, unsigned int width, unsigned int height, unsigned char* buffer);

        void CreateBuffer(unsigned int* id, int count = 1);
        void DeleteBuffer(unsigned int* id, int count = 1);
        void BindBuffer(BufferType bufferType, unsigned int id);
        void InitBuffer(BufferType bufferType, unsigned int size, const void* aData, BufferUsage usage);
        void UpdateBuffer(BufferType bufferType, unsigned int offset, unsigned int size, const void* data);

        void CreateVertexArray(unsigned int* id, int count = 1);
        void DeleteVertexArray(unsigned int* id, int count = 1);
        void BindVertexArray(unsigned int id);

        void EnableVertexAttributeArray(unsigned int vertexBuffer);
        void SetVertexAttributePointer(unsigned int vertexBuffer, const BufferElement& element, int stride, const void* offset);
        void SetVertexAttributeDivisor(unsigned int vertexBuffer, unsigned int divisor);

        unsigned int CreateShaderProgram();
        unsigned int CreateShader(ShaderType shaderType);
        void SetShaderSource(unsigned int shader, const std::string& source);
        void CompileShader(unsigned int shader);
        void DeleteShader(unsigned int shader);
        bool IsShaderCompiled(unsigned int shader);
        std::string GetShaderInfoLog(unsigned int shader);

        void AttachShader(unsigned int program, unsigned int shader);
        void DetachShader(unsigned int program, unsigned int shader);

        void LinkShaderProgram(unsigned int program);
        bool IsShaderProgramLinked(unsigned int program);
        void DeleteShaderProgram(unsigned int program);
        void UseShaderProgram(unsigned int program);
        std::string GetShaderProgramInfoLog(unsigned int program);

        int GetUniformLocation(unsigned int program, const std::string& uniform);
        int GetAttributeLocation(unsigned int program, const std::string& attribute);

        void SetUniformInt(int location, int value);
        void SetUniformIntArray(int location, int* values, unsigned int count);
        void SetUniformFloat(int location, float value);
        void SetUniformFloat2(int location, float value1, float value2);
        void SetUniformFloat3(int location, float value1, float value2, float value3);
        void SetUniformFloat4(int location, float value1, float value2, float value3, float value4);
        void SetUniformMatrix(int location, const Matrix& value);

        //Converts the GraphicType data type into the OpenGL constant
        unsigned int GetOpenGLGraphicType(GraphicType type);
       
        unsigned int GetOpenGLTextureMinFilter(TextureMinFilter minFilter);
        unsigned int GetOpenGLTextureMagFilter(TextureMagFilter magFilter);

        //Converts the PixelFormat::PixelLayout data type into the OpenGL constant
        unsigned int GetOpenGLPixelLayout(PixelLayout layout);

        //Converts the PixelFormat::PixelSize data type into the OpenGL constant
        unsigned int GetOpenGLPixelSize(PixelSize size);

        unsigned int GetOpenGLInternalPixelFormat(PixelFormat pixeFormat);

        //Converts the BlendingFactor data type into the OpenGL constant
        unsigned int GetOpenGLBlendingFactor(BlendingFactor blendingFactor);

        //Converts the Wrap::WrapMode data type into the OpenGL constant
        int GetOpenGLWrapMode(WrapMode wrapMode);

        //Converts the RenderMode data type into the OpenGL constant
        unsigned int GetOpenGLRenderMode(RenderMode renderMode);

        //Converts the BufferUsage data type into the OpenGL constant
        unsigned int GetOpenGLBufferUsage(BufferUsage dataBufferUsage);

        //Converts the BufferType data type into the OpenGL constant
        unsigned int GetOpenGLBufferType(BufferType dataBufferType);

        //Converts the ShaderType data type into the OpenGL constant
        unsigned int GetOpenGLShaderType(ShaderType shaderType);
        
        //Converts the ShaderDataType data type into the OpenGL constant
        unsigned int GetShaderDataType(ShaderDataType type);

    private:
        //Member variables
		Camera m_MainCamera;
        Color m_ClearColor;
		std::vector<Camera> m_CameraStack;
        std::vector<Rect> m_ScissorStack; 
    };
}

