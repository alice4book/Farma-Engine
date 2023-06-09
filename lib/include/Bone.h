#pragma once
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <assimp/scene.h>
#include <vector>
#include <string>

struct BoneInfo
{
    //index in finalBoneMatrices
    int id;
    //offset matrix transforms vertex from model space to bone space
    glm::mat4 offset;
};

struct KeyPosition
{
    glm::vec3 position = glm::vec3(0);
    float timeStamp = 0;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp = 0;
};

struct KeyScale
{
    glm::vec3 scale = glm::vec3(0);
    float timeStamp = 0;
};

class Bone
{
private:
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;
    int m_NumPositions = 0;
    int m_NumRotations = 0;
    int m_NumScalings = 0;

    glm::mat4 m_LocalTransform = glm::mat4(0);
    std::string m_Name;
    int m_ID = 0;

    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
    glm::mat4 InterpolatePosition(float animationTime);
    glm::mat4 InterpolateRotation(float animationTime);
    glm::mat4 InterpolateScaling(float animationTime);

public:

    Bone(const std::string& name, int ID, const aiNodeAnim* channel);
    void Update(float animationTime);
    glm::mat4 GetLocalTransform();
    std::string GetBoneName() const;
    int GetBoneID();
    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    int GetScaleIndex(float animationTime);
};

