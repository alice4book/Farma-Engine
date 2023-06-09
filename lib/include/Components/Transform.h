#include "Component.h"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_float4x4.hpp"

class Transform : public Component
{
protected:
    //Local space information
    glm::vec3 m_pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_eulerRot = { 0.0f, 0.0f, 0.0f }; //In degrees
    glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

    //Global space information concatenate in matrix
    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

    //Dirty flag
    bool m_isDirty = true;

    glm::mat4 getLocalModelMatrix();

public:
    explicit Transform(Entity *parent);

    void computeModelMatrix();

    void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix);

    void setLocalPosition(const glm::vec3& newPosition);

    void addToLocalPosition(const glm::vec3& addPosition);

    glm::vec3 getLocalRotation();

    void setLocalRotation(const glm::vec3& newEulerRot);

    void rotateLocal(const glm::vec3& newEulerRot);

    void addToLocalRotation(const glm::vec3& addEulerRot);

    void scaleEntity(const glm::vec3& newScale);

    void addToScaleEntity(const glm::vec3& addScale);

    const glm::vec3& getLocalPosition();

    const glm::vec3& getGlobalPosition();

    const glm::mat4& getModelMatrix();

    bool isDirty();
};