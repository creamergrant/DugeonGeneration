#pragma once

#include "../Math/Vector2.h"
#include <unordered_map>
#include <vector>
#include <string>


namespace GameDev2D
{
    //Forward declarations
    class Texture;

    enum class GraphicType
    {
        Byte = 0,
        UnsignedByte,
        Short,
        UnsignedShort,
        Int,
        UnsignedInt,
        Float,
        Double
    };

    enum class RenderMode
    {
        Points = 0,
        Lines,
        LineLoop,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan,
        Quads,
        LinesAdjacent,
        LineStripAdjacent
    };

    enum class BlendingFactor
    {
        Zero = 0,
        One,
        SourceColor,
        OneMinusSourceColor,
        SourceAlpha,
        OneMinusSourceAlpha,
        DestinationAlpha,
        OneMinusDestinationAlpha,
        DestinationColor,
        OneMinusDestinationColor,
        SourceAlphaSaturate
    };

    struct BlendingMode
    {
        BlendingMode() :
            source(BlendingFactor::SourceAlpha),
            destination(BlendingFactor::OneMinusSourceAlpha)
        {}

        BlendingMode(BlendingFactor source, BlendingFactor destination) :
            source(source), 
            destination(destination)
        {}

        //Member variables
        BlendingFactor source;
        BlendingFactor destination;
    };

    enum class TextureMinFilter
    {
        Nearest = 0,
        Linear
    };

    enum class TextureMagFilter
    {
        Nearest = 0,
        Linear
    };

    enum class PixelLayout
    {
        RGB = 0,
        RGBA
    };

    enum class PixelSize
    {
        UnsignedByte = 0,
    };

    struct PixelFormat
    {
        PixelFormat(PixelLayout layout, PixelSize size) :
            layout(layout),
            size(size)
        {

        }

        PixelFormat() :
            layout(PixelLayout::RGBA),
            size(PixelSize::UnsignedByte)
        {

        }

        PixelLayout layout;
        PixelSize size;
    };
    
    enum class WrapMode
    {
        ClampToEdge = 0,
        ClampToBorder,
        Repeat,
        MirroredRepeat,
        WRAP_MAX_ENUM
    };

    struct Wrap
    {
        Wrap(WrapMode s, WrapMode t) :
            s(s),
            t(t)
        {}

        Wrap() :
            s(WrapMode::ClampToEdge),
            t(WrapMode::ClampToEdge)
        {}

        WrapMode s;
        WrapMode t;
    };

    struct ImageData
    {
        ImageData(PixelFormat format, unsigned int width, unsigned int height) :
            format(format),
            width(width),
            height(height)
        {}

        ImageData() :
            format(),
            width(0),
            height(0)
        {}

        //Member variables
        PixelFormat format;
        unsigned int width;
        unsigned int height;
        std::vector<unsigned char> data;
    };

    struct Rect
    {
        Rect(Vector2 origin, Vector2 size) :
            origin(origin),
            size(size)
        {}

        Rect() :
            origin(0.0f, 0.0f),
            size(0.0f, 0.0f)
        {}

        Rect(const Rect& rect) :
            origin(rect.origin),
            size(rect.size)
        {}

        bool operator==(const Rect& rect) const
        {
            return origin == rect.origin && size == rect.size;
        }

        bool operator!=(const Rect& rect) const
        {
            return origin != rect.origin || size != rect.size;

        }

        Vector2 origin;
        Vector2 size;
    };

    struct Viewport
    {
        Viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) :
            x(x),
            y(y),
            width(width),
            height(height)
        {

        }

        Viewport(unsigned int width, unsigned int height) :
            x(0),
            y(0),
            width(width),
            height(height)
        {

        }

        unsigned int x;
        unsigned int y;
        unsigned int width;
        unsigned int height;
    };

    enum class BufferUsage
    {
        StreamDraw = 0,
        StreamRead,
        StreamCopy,
        StaticDraw,
        StaticRead,
        StaticCopy,
        DynamicDraw,
        DynamicRead,
        DynamicCopy
    };

    enum class BufferType
    {
        Vertex = 0,
        Index
    };

    struct VertexBufferDescriptor
    {
        VertexBufferDescriptor() :
            size(0),
            capacity(0),
            usage(BufferUsage::DynamicDraw)
        {
        }

        std::vector<int> attributeIndex;
        std::vector<unsigned int> attributeSize;
        unsigned int size;
        unsigned int capacity;
        BufferUsage usage;
    };

    struct IndexBufferDescriptor
    {
        IndexBufferDescriptor() :
        size(0),
        capacity(0),
        usage(BufferUsage::StaticDraw)
        {
        }

        unsigned int size;
        unsigned int capacity;
        BufferUsage usage;
    };

    enum class ShaderType
    {
        Unknown = -1,
        Vertex = 0,
        Fragment,
        Geometry
    };

    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static unsigned int ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:    return 4;
        case ShaderDataType::Float2:   return 4 * 2;
        case ShaderDataType::Float3:   return 4 * 3;
        case ShaderDataType::Float4:   return 4 * 4;
        case ShaderDataType::Mat3:     return 4 * 3 * 3;
        case ShaderDataType::Mat4:     return 4 * 4 * 4;
        case ShaderDataType::Int:      return 4;
        case ShaderDataType::Int2:     return 4 * 2;
        case ShaderDataType::Int3:     return 4 * 3;
        case ShaderDataType::Int4:     return 4 * 4;
        case ShaderDataType::Bool:     return 1;
        }

        return 0;
    }

    
}
