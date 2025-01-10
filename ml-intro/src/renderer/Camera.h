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

    void ArcBallCamera(float deltaX, float deltaY)
    {
        glm::vec4 position(GetEye().x, GetEye().y, GetEye().z, 1);
        glm::vec4 pivot(GetLookAt().x, GetLookAt().y, GetLookAt().z, 1);
        float deltaAngleX = (2 * PI / *m_width);
        float deltaAngleY = (PI / *m_height);
        float xAngle = deltaX * deltaAngleX;
        float yAngle = deltaY * deltaAngleY;

        xAngleSub += xAngle;
        yAngleSub += yAngle;

        std::cout << xAngleSub * (180 / PI) << " " << yAngleSub * (180 / PI) << std::endl;

        float cosAngle = glm::dot(GetViewDir(), m_upVector);
        if (cosAngle * glm::sign(yAngle) > 0.99f)
            yAngle = 0;

        glm::mat4x4 rotationMatrixX(1.0f);
        rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, m_upVector);
        position = (rotationMatrixX * (position - pivot)) + pivot;

        glm::mat4x4 rotationMatrixY(1.0f);
        rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, GetRightVector());
        glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

        SetCameraView(finalPosition, GetLookAt());
    }

    void PanCamera(glm::vec2 deltaMouse)
    {
        float xSub = deltaMouse.x * glm::sin(yAngleSub);
        float ySub = deltaMouse.x;
        float zSub = deltaMouse.y * glm::cos(xAngleSub);

        m_eye = glm::vec3(m_eye.x + xSub, m_eye.y + ySub, m_eye.z + zSub);
        m_lookAt = glm::vec3(m_lookAt.x + xSub, m_lookAt.y + ySub, m_lookAt.z + zSub);

        UpdateViewMatrix();
    }

    void ProcessMouseScroll(float yoffset)
    {
        m_fov -= (float)yoffset;
        if (m_fov < 1.0f)
            m_fov = 1.0f;
        if (m_fov > 45.0f)
            m_fov = 45.0f;
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

    float xAngleSub = 0.0;
    float yAngleSub = 0.0;
};
