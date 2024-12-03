#pragma once

#include "GraphicTypes.h"
#include "../Math/Vector2.h"
#include "../Math/Matrix.h"
#include <string>
#include <unordered_map>


namespace GameDev2D
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "");
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, unsigned int count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, float value1, float value2);
		void SetFloat3(const std::string& name, float value1, float value2, float value3);
		void SetFloat4(const std::string& name, float value1, float value2, float value3, float value4);
		void SetMat4(const std::string& name, const Matrix& value);

		static ShaderType GetShaderType(const std::string& type);

	private:
		bool Compile(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource);

		unsigned int m_ID;
	};
}