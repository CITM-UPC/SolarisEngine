#include "DataImporter.h"
#include <fstream> 
#include <iostream>
#include "App.h"
#include "Debug.h"



void DataImporter::TransformData(const aiMesh* ai_mesh, MeshData& meshData)
{

    // Recorremos los vértices y los agregamos a la estructura Mesh
    for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i) {
    	// Agregar las posiciones de los vértices
        meshData.vertices.push_back(ai_mesh->mVertices[i].x);
        meshData.vertices.push_back(ai_mesh->mVertices[i].y);
        meshData.vertices.push_back(ai_mesh->mVertices[i].z);

    	// Si el mesh tiene normales, agregar las normales
    	if (ai_mesh->HasNormals()) {
            meshData.normals.push_back(ai_mesh->mNormals[i].x);
    		meshData.normals.push_back(ai_mesh->mNormals[i].y);
    		meshData.normals.push_back(ai_mesh->mNormals[i].z);
    	}
    	else {
    		// Si no tiene normales, puedes calcularlas (esto es opcional si no tienes normales en el modelo)
    		// Aquí puedes agregar código para calcular las normales por triángulo.
    	}

    	// Si el mesh tiene coordenadas de textura, agregarlas
    	if (ai_mesh->mTextureCoords[0]) {
    		meshData.texCoords.push_back(ai_mesh->mTextureCoords[0][i].x);
    		meshData.texCoords.push_back(ai_mesh->mTextureCoords[0][i].y);
    	}
    	else {
    		// Si no tiene coordenadas de textura, agregar valores predeterminados
    		meshData.texCoords.push_back(0.0f);
    		meshData.texCoords.push_back(0.0f);
    	}
    }

    // Recorremos las caras (triángulos) y agregamos los índices
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
    	const aiFace& face = ai_mesh->mFaces[i];
    	for (unsigned int j = 0; j < face.mNumIndices; ++j) {
    		meshData.indices.push_back(face.mIndices[j]);
    	}
    }

    // Calcular las normales de las caras si es necesario
    CalculateFaceNormals(meshData);

    // Registro en el log
    Debug::Log("Mesh cargado con: ", meshData.indices.size(), " índices, ", meshData.vertices.size(), " vértices.");


}

void DataImporter::ExportData(const std::string& filePath, const MeshData& meshData)
{
    std::ofstream file(filePath, std::ios::binary);

    // Asegurarse de que el archivo se abrió correctamente
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo para escribir: " << filePath << std::endl;
        return;
    }

    // Escribir el número de vértices
    uint32_t numVertices = meshData.vertices.size() / 3;  // Cada vértice tiene 3 componentes (x, y, z)
    file.write(reinterpret_cast<const char*>(&numVertices), sizeof(numVertices));

    // Escribir los vértices
    file.write(reinterpret_cast<const char*>(meshData.vertices.data()), meshData.vertices.size() * sizeof(float));

    // Escribir las normales
    file.write(reinterpret_cast<const char*>(meshData.normals.data()), meshData.normals.size() * sizeof(float));

    // Escribir las coordenadas de textura
    file.write(reinterpret_cast<const char*>(meshData.texCoords.data()), meshData.texCoords.size() * sizeof(float));

    // Escribir los índices
    uint32_t numIndices = meshData.indices.size();
    file.write(reinterpret_cast<const char*>(&numIndices), sizeof(numIndices));
    file.write(reinterpret_cast<const char*>(meshData.indices.data()), meshData.indices.size() * sizeof(unsigned int));

    file.close();
}

void DataImporter::ImportarData(const std::string& filePath, MeshData& meshData)
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo para leer: " << filePath << std::endl;
        meshData = MeshData();
        return; // Devuelve una malla vacía si no se puede abrir el archivo
    }

    // Leer el número de vértices
    uint32_t numVertices;
    file.read(reinterpret_cast<char*>(&numVertices), sizeof(numVertices));

    // Leer los vértices
    meshData.vertices.resize(numVertices * 3); // 3 componentes por vértice (x, y, z)
    file.read(reinterpret_cast<char*>(meshData.vertices.data()), meshData.vertices.size() * sizeof(float));

    // Leer las normales
    meshData.normals.resize(numVertices * 3); // Asumimos que hay una normal por vértice
    file.read(reinterpret_cast<char*>(meshData.normals.data()), meshData.normals.size() * sizeof(float));

    // Leer las coordenadas de textura
    meshData.texCoords.resize(numVertices * 2); // Asumimos que hay una coordenada UV por vértice
    file.read(reinterpret_cast<char*>(meshData.texCoords.data()), meshData.texCoords.size() * sizeof(float));

    // Leer los índices
    uint32_t numIndices;
    file.read(reinterpret_cast<char*>(&numIndices), sizeof(numIndices));
    meshData.indices.resize(numIndices);
    file.read(reinterpret_cast<char*>(meshData.indices.data()), meshData.indices.size() * sizeof(unsigned int));

    file.close();
}








void DataImporter::CalculateFaceNormals(MeshData& mesh) {
    mesh.faceNormals.clear();
    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        // Obt�n los tres �ndices del tri�ngulo
        glm::vec3 v0(mesh.vertices[mesh.indices[i] * 3], mesh.vertices[mesh.indices[i] * 3 + 1], mesh.vertices[mesh.indices[i] * 3 + 2]);
        glm::vec3 v1(mesh.vertices[mesh.indices[i + 1] * 3], mesh.vertices[mesh.indices[i + 1] * 3 + 1], mesh.vertices[mesh.indices[i + 1] * 3 + 2]);
        glm::vec3 v2(mesh.vertices[mesh.indices[i + 2] * 3], mesh.vertices[mesh.indices[i + 2] * 3 + 1], mesh.vertices[mesh.indices[i + 2] * 3 + 2]);

        // Calcula el vector normal usando el producto cruzado
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        // Almacena la normal para cada tri�ngulo
        mesh.faceNormals.push_back(normal.x);
        mesh.faceNormals.push_back(normal.y);
        mesh.faceNormals.push_back(normal.z);
    }
}