#pragma once
#include "Resources/Resource.h"
#include "VertexBuffer.h"
#include <vector>


struct aiNode;
struct aiMesh;
struct aiScene;

namespace neu {
	class Model : public Resource {
	public:
		// vertex attributes
		struct Vertex {
			glm::vec3 position;
			glm::vec2 texcoord;
			glm::vec3 normal;
			glm::vec3 tangent;
		};

	public:
		CLASS_PROTOTYPE(Model)

		bool Load(const std::string& filename);
		void Draw(GLenum primitiveType = GL_TRIANGLES);

		void UpdateGui() override;

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);

	private:
		std::vector<res_t<VertexBuffer>> m_vertexBuffers;
	};
}