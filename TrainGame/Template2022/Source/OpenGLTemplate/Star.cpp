#include "Star.h"

CStar::CStar()
{}

CStar::~CStar()
{
	Release();
}

void CStar::Create(string filename)
{
	m_tTexture.Load(filename);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_tTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_uiVAO);
	glBindVertexArray(m_uiVAO);
	m_VBO.Create();
	m_VBO.Bind();

	// Write the code to add interleaved point, texture coord, and normal of the diamond
	glm::vec3 v0 = glm::vec3(0, 10, 0);
	glm::vec3 v1 = glm::vec3(2.5, 2.5, 0);
	glm::vec3 v2 = glm::vec3(10, 0, 0);
	glm::vec3 v3 = glm::vec3(2.5, -2.5, 0);
	glm::vec3 v4 = glm::vec3(0, -10, 0);
	glm::vec3 v5 = glm::vec3(-2.5, -2.5, 0);
	glm::vec3 v6 = glm::vec3(-10, 0, 0);
	glm::vec3 v7 = glm::vec3(-2.5, 2.5, 0);
	glm::vec3 v8 = glm::vec3(0, 0, 2.5);
	glm::vec3 v9 = glm::vec3(0, 0, -2.5);
	glm::vec3 n0 = glm::normalize(cross(normalize(v8 - v0), normalize(v7 - v0)));
	glm::vec3 n1 = glm::normalize(cross(normalize(v1 - v0), normalize(v8 - v0)));
	glm::vec3 n2 = glm::normalize(cross(normalize(v8 - v2), normalize(v2 - v1)));
	glm::vec3 n3 = glm::normalize(cross(normalize(v3 - v2), normalize(v8 - v2)));
	glm::vec3 n4 = glm::normalize(cross(normalize(v8 - v4), normalize(v4 - v3)));
	glm::vec3 n5 = glm::normalize(cross(normalize(v5 - v4), normalize(v8 - v4)));
	glm::vec3 n6 = glm::normalize(cross(normalize(v8 - v6), normalize(v6 - v5)));
	glm::vec3 n7 = glm::normalize(cross(normalize(v7 - v6), normalize(v8 - v6)));
	glm::vec3 n8 = glm::normalize(cross(normalize(v7 - v0), normalize(v9 - v0)));
	glm::vec3 n9 = glm::normalize(cross(normalize(v9 - v0), normalize(v1 - v0)));
	glm::vec3 n10 = glm::normalize(cross(normalize(v2 - v1), normalize(v9 - v2)));
	glm::vec3 n11 = glm::normalize(cross(normalize(v9 - v2), normalize(v3 - v2)));
	glm::vec3 n12 = glm::normalize(cross(normalize(v4 - v3), normalize(v9 - v4)));
	glm::vec3 n13 = glm::normalize(cross(normalize(v9 - v4), normalize(v5 - v4)));
	glm::vec3 n14 = glm::normalize(cross(normalize(v6 - v5), normalize(v9 - v6)));
	glm::vec3 n15 = glm::normalize(cross(normalize(v9 - v6), normalize(v7 - v6)));
	glm::vec2 t0 = glm::vec2(0, 0);
	glm::vec2 t1 = glm::vec2(1, 0);
	glm::vec2 t2 = glm::vec2(0.5, 1);

	//Front of the star
	//Left Triangle
	m_VBO.AddData(&v0, sizeof(glm::vec3));//
	m_VBO.AddData(&t0, sizeof(glm::vec2));//
	m_VBO.AddData(&n0, sizeof(glm::vec3));//
	m_VBO.AddData(&v7, sizeof(glm::vec3));//
	m_VBO.AddData(&t1, sizeof(glm::vec2));//
	m_VBO.AddData(&n0, sizeof(glm::vec3));//
	m_VBO.AddData(&v8, sizeof(glm::vec3));//
	m_VBO.AddData(&t2, sizeof(glm::vec2));//
	m_VBO.AddData(&n0, sizeof(glm::vec3));//

	//Right Triangle
	m_VBO.AddData(&v0, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n1, sizeof(glm::vec3));
	m_VBO.AddData(&v8, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n1, sizeof(glm::vec3));
	m_VBO.AddData(&v1, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n1, sizeof(glm::vec3));

	//Right side of the star
	//Left Triangle
	m_VBO.AddData(&v2, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n2, sizeof(glm::vec3));
	m_VBO.AddData(&v1, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n2, sizeof(glm::vec3));
	m_VBO.AddData(&v8, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n2, sizeof(glm::vec3));

	//Right Triangle
	m_VBO.AddData(&v2, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n3, sizeof(glm::vec3));
	m_VBO.AddData(&v8, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n3, sizeof(glm::vec3));
	m_VBO.AddData(&v3, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n3, sizeof(glm::vec3));

	//Bottom side of the star
	//Left Triangle
	m_VBO.AddData(&v4, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n4, sizeof(glm::vec3));
	m_VBO.AddData(&v3, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n4, sizeof(glm::vec3));
	m_VBO.AddData(&v8, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n4, sizeof(glm::vec3));

	//Right Triangle
	m_VBO.AddData(&v4, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n5, sizeof(glm::vec3));
	m_VBO.AddData(&v8, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n5, sizeof(glm::vec3));
	m_VBO.AddData(&v5, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n5, sizeof(glm::vec3));

	//Left Side of the star
	//Left Triangle
	m_VBO.AddData(&v6, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n6, sizeof(glm::vec3));
	m_VBO.AddData(&v5, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n6, sizeof(glm::vec3));
	m_VBO.AddData(&v8, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n6, sizeof(glm::vec3));
	//Right Triangle
	m_VBO.AddData(&v6, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n7, sizeof(glm::vec3));
	m_VBO.AddData(&v8, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n7, sizeof(glm::vec3));
	m_VBO.AddData(&v7, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n7, sizeof(glm::vec3));

	//Back of the star
	//Top of the star
	//Left
	m_VBO.AddData(&v0, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n8, sizeof(glm::vec3));
	m_VBO.AddData(&v9, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n8, sizeof(glm::vec3));
	m_VBO.AddData(&v7, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n8, sizeof(glm::vec3));

	//Right
	m_VBO.AddData(&v0, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n9, sizeof(glm::vec3));
	m_VBO.AddData(&v1, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n9, sizeof(glm::vec3));
	m_VBO.AddData(&v9, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n9, sizeof(glm::vec3));

	//Right side of the star
	//Left
	m_VBO.AddData(&v2, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n10, sizeof(glm::vec3));
	m_VBO.AddData(&v9, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n10, sizeof(glm::vec3));
	m_VBO.AddData(&v1, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n10, sizeof(glm::vec3));

	//Right
	m_VBO.AddData(&v2, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n11, sizeof(glm::vec3));
	m_VBO.AddData(&v3, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n11, sizeof(glm::vec3));
	m_VBO.AddData(&v9, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n11, sizeof(glm::vec3));

	//Bottom side of the star
	//Left
	m_VBO.AddData(&v4, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n12, sizeof(glm::vec3));
	m_VBO.AddData(&v9, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n12, sizeof(glm::vec3));
	m_VBO.AddData(&v3, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n12, sizeof(glm::vec3));

	//Right
	m_VBO.AddData(&v4, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n13, sizeof(glm::vec3));
	m_VBO.AddData(&v5, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n13, sizeof(glm::vec3));
	m_VBO.AddData(&v9, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n13, sizeof(glm::vec3));

	//Left side of the star
	//Left
	m_VBO.AddData(&v6, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n14, sizeof(glm::vec3));
	m_VBO.AddData(&v9, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n14, sizeof(glm::vec3));
	m_VBO.AddData(&v5, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n14, sizeof(glm::vec3));

	//Right
	m_VBO.AddData(&v6, sizeof(glm::vec3));
	m_VBO.AddData(&t0, sizeof(glm::vec2));
	m_VBO.AddData(&n15, sizeof(glm::vec3));
	m_VBO.AddData(&v7, sizeof(glm::vec3));
	m_VBO.AddData(&t1, sizeof(glm::vec2));
	m_VBO.AddData(&n15, sizeof(glm::vec3));
	m_VBO.AddData(&v9, sizeof(glm::vec3));
	m_VBO.AddData(&t2, sizeof(glm::vec2));
	m_VBO.AddData(&n15, sizeof(glm::vec3));

	// Upload data to GPU
	m_VBO.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}

void CStar::Render()
{
	glBindVertexArray(m_uiVAO);
	m_tTexture.Bind();

	// Call glDrawArrays to render the side
	glDrawArrays(GL_TRIANGLES, 0, 16 * 3);
}

void CStar::Release()
{
	m_tTexture.Release();
	glDeleteVertexArrays(1, &m_uiVAO);
	m_VBO.Release();
}