#include "Animator.h"
#include "TimeManager.h"

void Animator::UpdateAnimation(float dt)
{
    m_DeltaTime = dt;
    if (m_CurrentAnimation)
    {
        m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
        CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
}

void Animator::PlayAnimation(std::shared_ptr<Animation> pAnimation)
{
    m_CurrentAnimation = pAnimation;
    m_CurrentTime = 0.0f;
}

void Animator::PlayAnimation(PlayerAnimType animType)
{
    if (m_CurrentAnimation != animations[animType])
    {
        m_CurrentAnimation = animations[animType];
        m_CurrentTime = 0.0f;
    }
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

    if (Bone)
    {
        Bone->Update(m_CurrentTime);
        nodeTransform = Bone->GetLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        m_FinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);
}

std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
{
    return m_FinalBoneMatrices;
}

void Animator::update()
{
    float dt = TimeManager::getInstance()->getDeltaTimeUnlimitedFPS();
    UpdateAnimation(dt);
}

Animator::Animator(Model* playerModel)
    :Component(parent)
{
    std::shared_ptr <Animation> animation = std::make_shared<Animation>("res/anim/red/stand.fbx", playerModel);
    std::shared_ptr <Animation> animation2 = std::make_shared<Animation>("res/anim/red/go.fbx", playerModel);
    std::shared_ptr <Animation> animation3 = std::make_shared<Animation>("res/anim/red/action.fbx", playerModel);
    std::shared_ptr <Animation> animation4 = std::make_shared<Animation>("res/anim/red/hit.fbx", playerModel);
    std::shared_ptr <Animation> animation5 = std::make_shared<Animation>("res/anim/red/ishit.fbx", playerModel);
    animations.push_back(animation);
    animations.push_back(animation2);
    animations.push_back(animation3);
    animations.push_back(animation4);
    animations.push_back(animation5);

    m_CurrentTime = 0.0;
    m_CurrentAnimation = animations[0];

    m_FinalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++)
        m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}
