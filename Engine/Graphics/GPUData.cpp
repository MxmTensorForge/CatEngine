#include "GPUData.h"

#include <iostream>

GPUData::GPUData() {
	_vao = std::make_unique<VertexArray>();
	_vbo = std::make_unique<Buffer>(GL_ARRAY_BUFFER);
}
GPUData::~GPUData() {

}

void GPUData::loadData(const std::vector<Mxm::Vec3>& vertices, const std::vector<Mxm::Vec3>& normals, const std::vector<Mxm::Vec2>& texCoords) {
	_verticesCount = vertices.size();

	_vao->bind();

    std::vector<float> vertexData;
    vertexData.reserve(vertices.size() * 3);

    for (size_t i = 0; i < vertices.size(); i++) {
        vertexData.push_back(vertices[i].x);
        vertexData.push_back(vertices[i].y);
        vertexData.push_back(vertices[i].z);

		vertexData.push_back(normals[i / 3].x);
		vertexData.push_back(normals[i / 3].y);
		vertexData.push_back(normals[i / 3].z);

		vertexData.push_back(texCoords[i].x);
		vertexData.push_back(texCoords[i].y);
    }

	_vbo->bind();
	_vbo->bufferData(vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

	_vao->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	_vao->enableAttribute(0);
	_vao->setAttribute(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	_vao->enableAttribute(1);
	_vao->setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	_vao->enableAttribute(2);

	_vbo->unBind();
	_vao->unBind();
}
void GPUData::draw() const noexcept {
	_vao->bind();
	_vbo->bind();
	glDrawArrays(GL_TRIANGLES, 0, _verticesCount);
}