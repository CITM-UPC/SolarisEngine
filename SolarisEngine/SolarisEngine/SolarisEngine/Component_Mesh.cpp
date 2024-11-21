#include "Component_Mesh.h"
#include "App.h"
#include <iostream>
#include "Component_Transform.h"
#include <SDL2/SDL_stdinc.h>
#include "Debug.h"


Component_Mesh::Component_Mesh(GameObject* containerGO)  // Cambiado a puntero crudo
	: Component(containerGO, ComponentType::Mesh), vao(0), material(nullptr) {
	// Intenta obtener el componente de material del GameObject
	material = containerGO->GetComponent<Component_Material>();
}

Component_Mesh::~Component_Mesh() {}

void Component_Mesh::Enable() {
	enabled = true;
}

void Component_Mesh::Disable() {
	enabled = false;
}

void Component_Mesh::Update(double dt) {
	// L�gica de actualizaci�n si es necesario
}

void Component_Mesh::DrawComponent() {
	if (!enabled) return;


	if (containerGO->GetComponent<Component_Material>()) {
		material = containerGO->GetComponent<Component_Material>(); 
		material->DrawTexture();

	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor3f(1.0f, 0.0f, 1.0f); // Color rosa si no hay material
	}



	// Obtener el GameObject contenedor
	auto transform = containerGO->GetComponent<Component_Transform>();
	if (!transform) {
		std::cerr << "Error: No se pudo obtener el componente de transformaci�n." << std::endl;
		return;
	}
	glm::mat4 modelMatrix = transform->GetModelMatrix();

	// Aplicar la matriz de transformaci�n en el modelo
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(modelMatrix));


	// Dibujo de la malla usando OpenGL
	glEnableClientState(GL_VERTEX_ARRAY);

	for (const auto& mesh : meshes) {
		glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());

		if (!mesh.texCoords.empty() && material && material->GetTextureID() != 0) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, mesh.texCoords.data());
		}

		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());

		if (!mesh.texCoords.empty() && material && material->GetTextureID() != 0) {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		// Mostrar normales de v�rtices
		if (showVertexNormals) {
			glColor3f(0.0f, 1.0f, 0.0f); // Verde para las normales
			glBegin(GL_LINES);
			for (size_t i = 0; i < mesh.vertices.size(); i += 3) {
				glm::vec3 v(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);
				glm::vec3 n;
				if (mesh.normals.size() > 0) {
					n = glm::vec3(mesh.normals[i], mesh.normals[i + 1], mesh.normals[i + 2]);
				}
				else {
					n = glm::vec3(mesh.faceNormals[i], mesh.faceNormals[i + 1], mesh.faceNormals[i + 2]);
				}


				//glm::vec3 n(mesh.normals[i], mesh.normals[i + 1], mesh.normals[i + 2]);
				glm::vec3 end = v + n * 0.1f; // Escala de la l�nea
				glVertex3fv(glm::value_ptr(v));
				glVertex3fv(glm::value_ptr(end));
			}
			glEnd();
			if (material) {
				glColor3f(material->GetDiffuseColor().r, material->GetDiffuseColor().g, material->GetDiffuseColor().b);
			}
		}

		// Mostrar normales de caras
		if (showFaceNormals) {
			glColor3f(1.0f, 0.0f, 0.0f); // Rojo para las normales de caras
			glBegin(GL_LINES);
			for (size_t i = 0; i < mesh.indices.size(); i += 3) {
				glm::vec3 v0(mesh.vertices[mesh.indices[i] * 3], mesh.vertices[mesh.indices[i] * 3 + 1], mesh.vertices[mesh.indices[i] * 3 + 2]);
				glm::vec3 v1(mesh.vertices[mesh.indices[i + 1] * 3], mesh.vertices[mesh.indices[i + 1] * 3 + 1], mesh.vertices[mesh.indices[i + 1] * 3 + 2]);
				glm::vec3 v2(mesh.vertices[mesh.indices[i + 2] * 3], mesh.vertices[mesh.indices[i + 2] * 3 + 1], mesh.vertices[mesh.indices[i + 2] * 3 + 2]);
				glm::vec3 center = (v0 + v1 + v2) / 3.0f;
				glm::vec3 n(mesh.faceNormals[i], mesh.faceNormals[i + 1], mesh.faceNormals[i + 2]);
				glm::vec3 end = center + n * 0.1f;
				glVertex3fv(glm::value_ptr(center));
				glVertex3fv(glm::value_ptr(end));
			}
			glEnd();
			if (material) {
				glColor3f(material->GetDiffuseColor().r, material->GetDiffuseColor().g, material->GetDiffuseColor().b);
			}
			
		}

		// Mostrar la Bounding Box si est� activado
		if (showBoundingBox) {
			glm::vec3 min, max;
			for (size_t i = 0; i < mesh.vertices.size(); i += 3) {
				glm::vec3 v(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);
				if (i == 0) {
					min = max = v;
				}
				else {
					min = glm::min(min, v);
					max = glm::max(max, v);
				}
			}

			// Dibujar la caja delimitadora
			glColor3f(1.0f, 0.0f, 0.0f); // Rojo para la Bounding Box
			glBegin(GL_LINES);
			// Dibujar las aristas de la caja delimitadora
			glVertex3f(min.x, min.y, min.z);
			glVertex3f(max.x, min.y, min.z);

			glVertex3f(min.x, min.y, min.z);
			glVertex3f(min.x, max.y, min.z);

			glVertex3f(min.x, min.y, min.z);
			glVertex3f(min.x, min.y, max.z);

			glVertex3f(max.x, max.y, max.z);
			glVertex3f(min.x, max.y, max.z);

			glVertex3f(max.x, max.y, max.z);
			glVertex3f(max.x, min.y, max.z);

			glVertex3f(max.x, max.y, max.z);
			glVertex3f(max.x, max.y, min.z);

			glVertex3f(min.x, max.y, min.z);
			glVertex3f(max.x, max.y, min.z);

			glVertex3f(min.x, min.y, max.z);
			glVertex3f(max.x, min.y, max.z);

			glVertex3f(min.x, max.y, max.z);
			glVertex3f(min.x, max.y, min.z);

			glVertex3f(max.x, min.y, max.z);
			glVertex3f(max.x, min.y, min.z);

			glEnd();
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix(); // Restablecer la matriz despu�s de dibujar
}

void Component_Mesh::DrawInspectorComponent()
{
	if (ImGui::CollapsingHeader(u8"\ue025 Mesh")) {
		//ImGui::Text("Mesh Component");

		ImGui::Text("Total Vertex: %d", totalVertex);



		ImGui::Checkbox("Show Vertex Normals", &showVertexNormals); // Activar o desactivar normales
		ImGui::Checkbox("Show Face Normals", &showFaceNormals); // Activar o desactivar normales
		ImGui::Checkbox("Show Bounding Box", &showBoundingBox); // Activar o desactivar la visualizaci�n de la bounding box



		// Crear una ventana scrollable para la lista de meshes
		if (ImGui::TreeNode("Meshes")) {
			// Configurar la lista como scrollable
			ImGui::BeginChild("MeshesList", ImVec2(0, 200), true, ImGuiWindowFlags_HorizontalScrollbar);

			for (size_t i = 0; i < meshes.size(); ++i) {
				// Identificar cada mesh
				std::string meshLabel = "Mesh " + std::to_string(i);

				if (ImGui::TreeNode(meshLabel.c_str())) {
					// Mostrar información específica del mesh
					ImGui::Text("Vertices: %zu", meshes[i].vertices.size() / 3);
					ImGui::Text("Indices: %zu", meshes[i].indices.size());
					ImGui::Text("Normals: %zu", meshes[i].normals.size() / 3);
					ImGui::Text("TexCoords: %zu", meshes[i].texCoords.size() / 2);

					ImGui::TreePop();
				}
			}

			ImGui::EndChild(); // Finalizar el contenedor de scroll
			ImGui::TreePop();  // Finalizar el árbol de "Meshes"




		}
	}




}

void Component_Mesh::LoadMesh(aiMesh* ai_mesh) {
	/* glGenVertexArrays(1, &vao);
	 glBindVertexArray(vao);

	 GLuint vbo;
	 glGenBuffers(1, &vbo);
	 glBindBuffer(GL_ARRAY_BUFFER, vbo);
	 glBufferData(GL_ARRAY_BUFFER, ai_mesh->mNumVertices * sizeof(aiVector3D), ai_mesh->mVertices, GL_STATIC_DRAW);

	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	 glEnableVertexAttribArray(0);

	 this->vao = vao;*/
}

void Component_Mesh::LoadMesh(const aiScene* ai_scene) {
	for (unsigned int i = 0; i < ai_scene->mNumMeshes; ++i) {
		Mesh mesh;
		const aiMesh* aiMesh = ai_scene->mMeshes[i];

		for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j) {
			mesh.vertices.push_back(aiMesh->mVertices[j].x);
			mesh.vertices.push_back(aiMesh->mVertices[j].y);
			mesh.vertices.push_back(aiMesh->mVertices[j].z);


			if (aiMesh->HasNormals()) { // Copia las normales si existen
				mesh.normals.push_back(aiMesh->mNormals[j].x);
				mesh.normals.push_back(aiMesh->mNormals[j].y);
				mesh.normals.push_back(aiMesh->mNormals[j].z);
			}
			else {
				// Calcula las normales si no est�n disponibles en el modelo
				// C�digo para calcular las normales por tri�ngulo
			}


			if (aiMesh->mTextureCoords[0]) {
				mesh.texCoords.push_back(aiMesh->mTextureCoords[0][j].x);
				mesh.texCoords.push_back(aiMesh->mTextureCoords[0][j].y);
			}
			else {
				mesh.texCoords.push_back(0.0f);
				mesh.texCoords.push_back(0.0f);
			}
		}

		for (unsigned int j = 0; j < aiMesh->mNumFaces; ++j) {
			const aiFace& face = aiMesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; ++k) {
				mesh.indices.push_back(face.mIndices[k]);
			}
		}

		CalculateFaceNormals(mesh);

		Debug::Log("	Mesh ", i, "cargado con : ", mesh.indices.size(), " indices, ", mesh.vertices.size(), " vertices.");
		totalVertex += mesh.vertices.size();

		meshes.push_back(mesh);
	}

	Debug::Log("	Meshes del objecto cargado. Total de meshes: ", meshes.size());

}

void Component_Mesh::CalculateFaceNormals(Mesh& mesh) {
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

void Component_Mesh::GenerateCubeMesh() {
	Mesh cubeMesh;
	cubeMesh.vertices = {
		//  (Frente)
		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,

		//  (Atr��s)
		-0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,

		//  (Izquierda)
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,

		//  (Derecha)
		 0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,

		 //  (Arriba)
		 -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,

		 //  (Abajo)
		 -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
	};

	cubeMesh.indices = {
		//  (Frente)
		0, 1, 2, 2, 3, 0,

		//  (Atr��s)
		4, 5, 6, 6, 7, 4,

		//  (Izquierda)
		8, 9, 10, 10, 11, 8,

		//  (Derecha)
		12, 13, 14, 14, 15, 12,

		//  (Arriba)
		16, 17, 18, 18, 19, 16,

		//  (Abajo)
		20, 21, 22, 22, 23, 20,
	};

	cubeMesh.texCoords = {
		// Frente
		0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
		// Atrás
		0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
		// Izquierda
		0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
		// Derecha
		0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
		// Arriba
		0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
		// Abajo
		0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
	};

	CalculateFaceNormals(cubeMesh);



	meshes.push_back(cubeMesh);
}


void Component_Mesh::GenerateSphereMesh() {
	Mesh sphereMesh;
	const int sectorCount = 36;
	const int stackCount = 18;
	const float radius = 0.5f;
	for (int i = 0; i <= stackCount; ++i) {
		float stackAngle = M_PI / 2 - i * M_PI / stackCount;
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);
		for (int j = 0; j <= sectorCount; ++j) {
			float sectorAngle = j * 2 * M_PI / sectorCount;
			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);
			sphereMesh.vertices.push_back(x);
			sphereMesh.vertices.push_back(y);
			sphereMesh.vertices.push_back(z);
		}
	}
	for (int i = 0; i < stackCount; ++i) {
		for (int j = 0; j < sectorCount; ++j) {
			int first = (i * (sectorCount + 1)) + j;
			int second = first + sectorCount + 1;
			sphereMesh.indices.push_back(first);
			sphereMesh.indices.push_back(second);
			sphereMesh.indices.push_back(first + 1);
			sphereMesh.indices.push_back(second);
			sphereMesh.indices.push_back(second + 1);
			sphereMesh.indices.push_back(first + 1);
		}
	}


	for (int i = 0; i <= stackCount; ++i) {
		for (int j = 0; j <= sectorCount; ++j) {
			float u = (float)j / sectorCount; // Coordenada horizontal (longitud)
			float v = (float)i / stackCount; // Coordenada vertical (latitud)
			sphereMesh.texCoords.push_back(u);
			sphereMesh.texCoords.push_back(v);
		}
	}

	CalculateFaceNormals(sphereMesh);

	meshes.push_back(sphereMesh);
}

void Component_Mesh::GeneratePlaneMesh() {
	Mesh planeMesh;
	planeMesh.vertices = { -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f };
	planeMesh.indices = { 0, 1, 2, 2, 3, 0 };

	planeMesh.texCoords = {
	0.0f, 0.0f,  // Vértice 1
	1.0f, 0.0f,  // Vértice 2
	1.0f, 1.0f,  // Vértice 3
	0.0f, 1.0f   // Vértice 4
	};

	CalculateFaceNormals(planeMesh);


	meshes.push_back(planeMesh);
}

void Component_Mesh::GenerateTriangleMesh() {
	Mesh triangleMesh;

	// ����
	triangleMesh.vertices = {
		0.0f, 0.5f, 0.0f,  // A
	   -0.5f, -0.5f, 0.5f, // B
		0.5f, -0.5f, 0.5f, // C
	   -0.5f, -0.5f, -0.5f,// D
		0.5f, -0.5f, -0.5f // E
	};

	// ����
	triangleMesh.indices = {
		0, 1, 2,  // ABC
		0, 3, 1,  // ADE
		0, 4, 3,  // ABE
		0, 2, 4,  // ACE
		1, 3, 4,  // BDE
		1, 4, 2   // BCE
	};


	// Coordenadas UV
	triangleMesh.texCoords = {
		// Coordenadas para el vértice superior (compartido)
		0.5f, 1.0f,  // A (vértice superior)

		// Coordenadas para la base cuadrada (comparten vértices)
		0.0f, 0.0f,  // B (inferior izquierda)
		1.0f, 0.0f,  // C (inferior derecha)
		1.0f, 1.0f,  // D (superior derecha)
		0.0f, 1.0f   // E (superior izquierda)
	};

	CalculateFaceNormals(triangleMesh);


	meshes.push_back(triangleMesh);
}


void Component_Mesh::GenerateCapsuleMesh() {
	Mesh capsuleMesh;
	const int slices = 36;
	const int stacks = 18;
	const float radius = 0.5f;
	const float height = 1.0f;
	const float halfHeight = height / 2.0f;

	// Generate vertices
	for (int i = 0; i <= stacks; ++i) {
		float phi = i * M_PI / stacks - M_PI_2; // From -90 to 90 degrees
		for (int j = 0; j <= slices; ++j) {
			float theta = j * 2.0f * M_PI / slices; // From 0 to 360 degrees

			float x = radius * cos(phi) * cos(theta);
			float y = radius * sin(phi);
			float z = radius * cos(phi) * sin(theta);

			if (phi >= 0) {
				y += halfHeight; // Top hemisphere
			}
			else {
				y -= halfHeight; // Bottom hemisphere
			}

			capsuleMesh.vertices.push_back(x);
			capsuleMesh.vertices.push_back(y);
			capsuleMesh.vertices.push_back(z);
		}
	}

	// Generate indices
	for (int i = 0; i < stacks; ++i) {
		for (int j = 0; j < slices; ++j) {
			int first = (i * (slices + 1)) + j;
			int second = first + slices + 1;

			capsuleMesh.indices.push_back(first);
			capsuleMesh.indices.push_back(second);
			capsuleMesh.indices.push_back(first + 1);

			capsuleMesh.indices.push_back(second);
			capsuleMesh.indices.push_back(second + 1);
			capsuleMesh.indices.push_back(first + 1);
		}
	}


	for (int i = 0; i <= stacks; ++i) {
		for (int j = 0; j <= slices; ++j) {
			float u = (float)j / slices;
			float v = (float)i / stacks;
			capsuleMesh.texCoords.push_back(u);
			capsuleMesh.texCoords.push_back(v);
		}
	}

	CalculateFaceNormals(capsuleMesh);

	meshes.push_back(capsuleMesh);
}




void Component_Mesh::GenerateCylinderMesh() {
	Mesh cylinderMesh;
	const int slices = 36;
	const float radius = 0.5f;
	const float height = 1.0f;

	// Generar vértices y coordenadas UV para el cuerpo del cilindro
	for (int i = 0; i <= slices; ++i) {
		float angle = i * 2.0f * M_PI / slices;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		float u = (float)i / slices;

		// Vértices del cuerpo
		cylinderMesh.vertices.push_back(x);
		cylinderMesh.vertices.push_back(-0.5f * height);
		cylinderMesh.vertices.push_back(z);
		cylinderMesh.texCoords.push_back(u); // Coordenada U
		cylinderMesh.texCoords.push_back(0.0f); // Coordenada V

		cylinderMesh.vertices.push_back(x);
		cylinderMesh.vertices.push_back(0.5f * height);
		cylinderMesh.vertices.push_back(z);
		cylinderMesh.texCoords.push_back(u); // Coordenada U
		cylinderMesh.texCoords.push_back(1.0f); // Coordenada V
	}

	// Centro de las tapas (vértices adicionales)
	cylinderMesh.vertices.push_back(0.0f);
	cylinderMesh.vertices.push_back(-0.5f * height);
	cylinderMesh.vertices.push_back(0.0f);
	cylinderMesh.texCoords.push_back(0.5f); // Coordenada U
	cylinderMesh.texCoords.push_back(0.5f); // Coordenada V

	cylinderMesh.vertices.push_back(0.0f);
	cylinderMesh.vertices.push_back(0.5f * height);
	cylinderMesh.vertices.push_back(0.0f);
	cylinderMesh.texCoords.push_back(0.5f); // Coordenada U
	cylinderMesh.texCoords.push_back(0.5f); // Coordenada V

	// Generar índices para el cuerpo del cilindro
	for (int i = 0; i < slices; ++i) {
		int bottom1 = 2 * i;
		int bottom2 = 2 * ((i + 1) % slices);
		int top1 = bottom1 + 1;
		int top2 = bottom2 + 1;

		// Lados del cilindro
		cylinderMesh.indices.push_back(bottom1);
		cylinderMesh.indices.push_back(bottom2);
		cylinderMesh.indices.push_back(top1);

		cylinderMesh.indices.push_back(top1);
		cylinderMesh.indices.push_back(bottom2);
		cylinderMesh.indices.push_back(top2);

		// Tapa inferior
		cylinderMesh.indices.push_back(bottom1);
		cylinderMesh.indices.push_back(bottom2);
		cylinderMesh.indices.push_back(2 * slices);

		// Tapa superior
		cylinderMesh.indices.push_back(top1);
		cylinderMesh.indices.push_back(2 * slices + 1);
		cylinderMesh.indices.push_back(top2);
	}

	// Coordenadas UV para las tapas
	for (int i = 0; i < slices; ++i) {
		float angle = i * 2.0f * M_PI / slices;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		float u = (x / radius + 1.0f) * 0.5f;
		float v = (z / radius + 1.0f) * 0.5f;

		// UV para la tapa inferior
		cylinderMesh.texCoords.push_back(u);
		cylinderMesh.texCoords.push_back(v);

		// UV para la tapa superior
		cylinderMesh.texCoords.push_back(u);
		cylinderMesh.texCoords.push_back(v);
	}

	CalculateFaceNormals(cylinderMesh);

	// Guardar el cilindro
	meshes.push_back(cylinderMesh);
}


glm::vec3 Component_Mesh::CalculateMeshSize() {
	if (meshes.empty()) {
		return glm::vec3(0.0f); // Retornar un tama�o cero si no hay mallas
	}

	glm::vec3 minBounds(FLT_MAX, FLT_MAX, FLT_MAX); // Inicializar con valores m�ximos
	glm::vec3 maxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX); // Inicializar con valores m�nimos

	for (const auto& mesh : meshes) {
		// Asumiendo que 'mesh.vertices' es un vector plano que contiene las coordenadas de los v�rtices
		for (size_t i = 0; i < mesh.vertices.size(); i += 3) {
			glm::vec3 vertex(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);

			// Encontrar los l�mites m�nimos y m�ximos en cada eje
			minBounds = glm::min(minBounds, vertex);
			maxBounds = glm::max(maxBounds, vertex);
		}
	}

	// Calcular el tama�o de la malla en cada dimensi�n
	glm::vec3 meshSize = maxBounds - minBounds;

	
	return meshSize; 
}

Component* Component_Mesh::Clone() const {
	return new Component_Mesh(*this); // Crea una copia usando el constructor de copia
}

std::pair<glm::vec3, glm::vec3> Component_Mesh::GetBoundingBoxInWorldSpace() const {
	// Verificar si hay mallas; si no, retornar una caja vacía
	if (meshes.empty()) {
		return { glm::vec3(0.0f), glm::vec3(0.0f) };
	}

	// Inicializar los límites locales con valores extremos
	glm::vec3 minLocal(FLT_MAX, FLT_MAX, FLT_MAX);
	glm::vec3 maxLocal(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// Iterar a través de todas las mallas para encontrar los límites mínimos y máximos
	for (const auto& mesh : meshes) {
		for (size_t i = 0; i < mesh.vertices.size(); i += 3) {
			glm::vec3 vertex(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);
			minLocal = glm::min(minLocal, vertex);
			maxLocal = glm::max(maxLocal, vertex);
		}
	}

	// Transformar los límites locales a espacio mundial
	glm::mat4 modelMatrix = containerGO->GetComponent<Component_Transform>()->GetModelMatrix();
	glm::vec3 minWorld = glm::vec3(modelMatrix * glm::vec4(minLocal, 1.0f));
	glm::vec3 maxWorld = glm::vec3(modelMatrix * glm::vec4(maxLocal, 1.0f));

	return { minWorld, maxWorld };
}