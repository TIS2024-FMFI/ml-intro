#include "Edge.h"

GLuint Edge::shaderProgram = 0;
GLuint Edge::edgeVAO = 0;
GLuint Edge::edgeVBO = 0;
std::vector<float> Edge::edgeData;
bool Edge::initialized = false;
int Edge::vertCount = 0;
vec3* Edge::negCol = nullptr;
vec3* Edge::posCol = nullptr;
float Edge::LineSize = 5;

Edge::Edge(vec3 posA, vec3 posB) : posA(posA), posB(posB) {
    Edge::AddEdge(*this);
}

void Edge::InitializeBuffers() {
    if (initialized) return;

    glGenVertexArrays(1, &edgeVAO);
    glGenBuffers(1, &edgeVBO);

    glBindVertexArray(edgeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // pos
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // weight
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float))); // UV
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Edge::shaderProgram = ShaderPrograms::getInstance().getEdgeShaderProgramId();

    initialized = true;
}


void Edge::AddEdge(const Edge& edge) {
    edgeData.insert(edgeData.end(), {
        edge.posA.x, edge.posA.y, edge.posA.z, 0, 0,
        edge.posB.x, edge.posB.y, edge.posB.z, 0, 1,
        });

    vertCount += 2;
}

void Edge::UpdateValues(vector<float> weights) {
    for (int i = 0; i < vertCount; i++) {
        edgeData[i * 5 + 3] = weights[i/2];
    }
}

void Edge::UploadData() {
    if (!initialized) return;
    glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);
    glBufferData(GL_ARRAY_BUFFER, edgeData.size() * sizeof(float), edgeData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Edge::ClearEdges() {
    edgeData.clear();
    vertCount = 0;
}

void Edge::RenderEdges(Camera* cam) {
    if (!initialized) return;
    glLineWidth(LineSize);

    auto now = chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    float t = (chrono::duration_cast<chrono::milliseconds>(duration).count() % 1000) / 1000.0f;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(cam->GetProjMatrix()));
    glUniform1f(glGetUniformLocation(shaderProgram, "t"), t);

    glUniform3f(glGetUniformLocation(shaderProgram, "negCol"), negCol->x, negCol->y, negCol->z);
    glUniform3f(glGetUniformLocation(shaderProgram, "posCol"), posCol->x, posCol->y, posCol->z);


    glBindVertexArray(edgeVAO);
    glDrawArrays(GL_LINES, 0, vertCount);
    glBindVertexArray(0);


    glDisable(GL_BLEND);
}