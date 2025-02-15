#include "Node.h"

GLuint Node::shaderProgram = 0;
GLuint Node::nodeVAO = 0;
GLuint Node::nodeVBO = 0;
std::vector<float> Node::nodeData;
bool Node::initialized = false;
int Node::vertCount = 0;
vec3* Node::negCol = nullptr;
vec3* Node::posCol = nullptr;
float Node::PointSize = 3;

Node::Node(vec3 position, float isInputLayerNode) : pos(position), isInputLayerNode(isInputLayerNode){
    Node::AddNode(*this);
}

void Node::InitializeBuffers() {
    if (initialized) return;

    glGenVertexArrays(1, &nodeVAO);
    glGenBuffers(1, &nodeVBO);

    glBindVertexArray(nodeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, nodeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // activation
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float))); // isInputLayer
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Node::shaderProgram = ShaderPrograms::getInstance().getNodeShaderProgramId();

    initialized = true;
}

void Node::AddNode(const Node& node) {
    nodeData.insert(nodeData.end(), {
            node.pos.x, node.pos.y, node.pos.z, 0, node.isInputLayerNode
        });
    vertCount++;
}

void Node::UpdateValues(vector<float> values) {
    for (int i = 0; i < vertCount; i++) {
        nodeData[i * 5 + 3] = values[i];
    }
}

void Node::ClearNodes() {
    nodeData.clear();
    vertCount = 0;
}

void Node::UploadData() {
    if (!initialized) return;
    glBindBuffer(GL_ARRAY_BUFFER, nodeVBO);
    glBufferData(GL_ARRAY_BUFFER, nodeData.size() * sizeof(float), nodeData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Node::RenderNodes(Camera* cam) {
    if (!initialized) return;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, value_ptr(cam->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, value_ptr(cam->GetProjMatrix()));
    glUniform1f(glGetUniformLocation(shaderProgram, "pointSize"), 100.0f*PointSize);

    glUniform3f(glGetUniformLocation(shaderProgram, "negCol"), negCol->x, negCol->y, negCol->z);
    glUniform3f(glGetUniformLocation(shaderProgram, "posCol"), posCol->x, posCol->y, posCol->z);

    glBindVertexArray(nodeVAO);
    glDrawArrays(GL_POINTS, 0, vertCount);
    glBindVertexArray(0);

    glDisable(GL_PROGRAM_POINT_SIZE);
    glDisable(GL_POINT_SPRITE);
}