#include "Edge.h"

GLuint Edge::shaderProgram = 0;
GLuint Edge::edgeVAO = 0;
GLuint Edge::edgeVBO = 0;
std::vector<float> Edge::edgeData;
bool Edge::initialized = false;
int Edge::vertCount = 0;

Edge::Edge(vec3 posA, vec3 posB, float w) : posA(posA), posB(posB), weight(w) {
    Edge::InitializeBuffers();
    Edge::AddEdge(*this);

    vec3 p = mix(posA, posB, 0.25);
    Label(p, weight);
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

    Label::InitializeBuffers();

    initialized = true;
}


void Edge::AddEdge(const Edge& edge) {
    edgeData.push_back(edge.posA.x);
    edgeData.push_back(edge.posA.y);
    edgeData.push_back(edge.posA.z);
    edgeData.push_back(edge.weight);
    edgeData.push_back(0.0);

    edgeData.push_back(edge.posB.x);
    edgeData.push_back(edge.posB.y);
    edgeData.push_back(edge.posB.z);
    edgeData.push_back(edge.weight);
    edgeData.push_back(1.0);

    vertCount += 2;
}

void Edge::UploadData() {
    glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);
    glBufferData(GL_ARRAY_BUFFER, edgeData.size() * sizeof(float), edgeData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    Label::UploadData();
}

void Edge::ClearEdges() {
    edgeData.clear();
    vertCount = 0;
    Label::ClearLabels();
}

void Edge::RenderEdges(Camera* cam) {
    if (!initialized) return;

    auto now = chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    float t = (chrono::duration_cast<chrono::milliseconds>(duration).count() % 1000) / 1000.0f;

    //cout << "time: " << t << endl;
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(cam->GetProjMatrix()));
    glUniform1f(glGetUniformLocation(shaderProgram, "t"), t);


    glBindVertexArray(edgeVAO);
    glDrawArrays(GL_LINES, 0, vertCount);
    glBindVertexArray(0);


    glDisable(GL_BLEND);
}