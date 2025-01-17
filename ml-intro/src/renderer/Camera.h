#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <iostream>

# define PI           3.14159265358979323846
//https://github.com/uysalaltas/Pixel-Engine/blob/main/Pixel/Camera.h

class Camera
{
public:

    Camera(glm::vec3 eye, glm::vec3 lookat, float* width, float* height)
        : m_eye(std::move(eye))
        , m_lookAt(std::move(lookat))
        , m_width(width)
        , m_height(height)
    {
        m_upVector = glm::vec3(.0f, 1.f, .0f);
        m_eyeInitial = m_eye;
        UpdateViewMatrix();
    }

    glm::mat4x4 GetViewMatrix() const { return m_viewMatrix; }
    glm::mat4x4 GetProjMatrix() const { return m_projMatrix; }
    glm::vec3 GetEye() const { return m_eye; }
    glm::vec3 GetUpVector() const { return m_upVector; }
    glm::vec3 GetLookAt() const { return m_lookAt; }
    glm::vec3 GetViewDir() const { return -glm::transpose(m_viewMatrix)[2]; }
    glm::vec3 GetRightVector() const { return glm::transpose(m_viewMatrix)[0]; }
    float GetFOV() const { return m_fov; }

    void SetFOV(float fov)
    {
        m_fov = fov;
    }

    void UpdateViewMatrix()
    {
        m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
    }

    void UpdateProjMatrix()
    {
        m_projMatrix = glm::perspective(glm::radians(m_fov), *m_width / *m_height, 0.1f, 200.0f * 20);
    }

    void SetCameraView(glm::vec3 eye, glm::vec3 lookat)
    {
        m_eye = std::move(eye);
        m_lookAt = std::move(lookat);
        UpdateViewMatrix();
    }

    void MoveCamera(glm::vec2 delta) {
        glm::vec3 eye = m_eye - GetRightVector() * delta.x;
        eye += GetUpVector() * delta.y;
        
        eye = glm::normalize(eye) * distToTarget;

        SetCameraView(eye, glm::vec3(0));
    }

    void ProcessMouseScroll(float y) {
        distToTarget -= y * 0.1;
        distToTarget = glm::clamp(distToTarget, 1.0f, 25.0f);

        SetCameraView(glm::normalize(m_eye) * distToTarget, glm::vec3(0));
    }

private:
    glm::mat4x4 m_viewMatrix;
    glm::mat4x4 m_projMatrix;
    glm::vec3 m_eye;
    glm::vec3 m_eyeInitial;
    glm::vec3 m_lookAt;
    glm::vec3 m_upVector;
    float* m_width;
    float* m_height;
    float m_fov = 45;
    float pan_speed = .5f;
    float distToTarget = 5;
};
