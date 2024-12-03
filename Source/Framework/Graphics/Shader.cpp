#include "Shader.h"
#include "Graphics.h"
#include "../Application/Application.h"


namespace GameDev2D
{
	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource)
	{
		Compile(vertexSource, fragmentSource, geometrySource);
	}

	Shader::~Shader()
	{
	}

	void Shader::Bind() const
	{
		Application::Get().GetGraphics().UseShaderProgram(m_ID);
	}

	void Shader::Unbind() const
	{
		Application::Get().GetGraphics().UseShaderProgram(0);
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		int location = Application::Get().GetGraphics().GetUniformLocation(m_ID, name);
		Application::Get().GetGraphics().SetUniformInt(location, value);
	}

	void Shader::SetIntArray(const std::string& name, int* values, unsigned int count)
	{
		int location = Application::Get().GetGraphics().GetUniformLocation(m_ID, name);
		Application::Get().GetGraphics().SetUniformIntArray(location, values, count);
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		int location = Application::Get().GetGraphics().GetUniformLocation(m_ID, name);
		Application::Get().GetGraphics().SetUniformFloat(location, value);
	}

	void Shader::SetFloat2(const std::string& name, float value1, float value2)
	{
		int location = Application::Get().GetGraphics().GetUniformLocation(m_ID, name);
		Application::Get().GetGraphics().SetUniformFloat2(location, value1, value2);
	}

	void Shader::SetFloat3(const std::string& name, float value1, float value2, float value3)
	{
		int location = Application::Get().GetGraphics().GetUniformLocation(m_ID, name);
		Application::Get().GetGraphics().SetUniformFloat3(location, value1, value2, value3);
	}

	void Shader::SetFloat4(const std::string& name, float value1, float value2, float value3, float value4)
	{
		int location = Application::Get().GetGraphics().GetUniformLocation(m_ID, name);
		Application::Get().GetGraphics().SetUniformFloat4(location, value1, value2, value3, value4);
	}

	void Shader::SetMat4(const std::string& name, const Matrix& value)
	{
		int location = Application::Get().GetGraphics().GetUniformLocation(m_ID, name);
		Application::Get().GetGraphics().SetUniformMatrix(location, value);
	}

	ShaderType Shader::GetShaderType(const std::string& type)
	{
		if (type == "vertex")
			return ShaderType::Vertex;
		if (type == "geometry")
			return ShaderType::Geometry;
		if (type == "fragment")
			return ShaderType::Fragment;

		return ShaderType::Unknown;
	}

	bool Shader::Compile(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource)
	{
		Graphics& graphics = Application::Get().GetGraphics();

		assert(vertexSource.length() > 0);
		assert(fragmentSource.length() > 0);

		m_ID = graphics.CreateShaderProgram();

		unsigned int vertexShader = graphics.CreateShader(ShaderType::Vertex);
		graphics.SetShaderSource(vertexShader, vertexSource);
		graphics.CompileShader(vertexShader);

		if (graphics.IsShaderCompiled(vertexShader))
		{
			graphics.AttachShader(m_ID, vertexShader);
		}
		else
		{
			Log::Error(true, LogVerbosity::Shaders, "Compiling vertex shader failed: %s", graphics.GetShaderInfoLog(vertexShader).c_str());
			graphics.DeleteShader(vertexShader);
		}

		unsigned int fragmentShader = graphics.CreateShader(ShaderType::Fragment);
		graphics.SetShaderSource(fragmentShader, fragmentSource);
		graphics.CompileShader(fragmentShader);

		if (graphics.IsShaderCompiled(fragmentShader))
		{
			graphics.AttachShader(m_ID, fragmentShader);
		}
		else
		{
			Log::Error(true, LogVerbosity::Shaders, "Compiling fragment shader failed: %s", graphics.GetShaderInfoLog(fragmentShader).c_str());
			graphics.DeleteShader(fragmentShader);
		}

		if (geometrySource != "")
		{
			unsigned int geometryShader = graphics.CreateShader(ShaderType::Geometry);
			graphics.SetShaderSource(geometryShader, geometrySource);
			graphics.CompileShader(geometryShader);

			if (graphics.IsShaderCompiled(geometryShader))
			{
				graphics.AttachShader(m_ID, geometryShader);
			}
			else
			{
				Log::Error(true, LogVerbosity::Shaders, "Compiling geometry shader failed: %s", graphics.GetShaderInfoLog(geometryShader).c_str());
				graphics.DeleteShader(geometryShader);
			}
		}

		graphics.LinkShaderProgram(m_ID);
		if (graphics.IsShaderProgramLinked(m_ID))
		{

			graphics.DetachShader(m_ID, vertexShader);
			graphics.DeleteShader(vertexShader);
			graphics.DetachShader(m_ID, fragmentShader);
			graphics.DeleteShader(fragmentShader);
		}
		else
		{
			Log::Error(true, LogVerbosity::Shaders, "Shader program failed to link: %s", graphics.GetShaderProgramInfoLog(m_ID).c_str());
			graphics.DeleteShaderProgram(m_ID);
			graphics.DeleteShader(vertexShader);
			graphics.DeleteShader(fragmentShader);

			return false;
		}

		return true;
	}
}