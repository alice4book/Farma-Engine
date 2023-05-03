#include "RobotMovement.h"

#include "imgui_internal.h"

#include "DynamicColliderComponent.h"
#include "StaticColliderComponent.h"
#include "Transform.h"
#include "TimeManager.h"

RobotMovement::RobotMovement(Entity* parent, Transform* transform,
	DynamicColliderComponent* colliderBody, DynamicColliderComponent* colliderFront,
	float speed, RobotMovementType type, glm::vec3 forward, float offset)
	: Component(parent)
	, forward(forward)
	, speed(speed)
	, transform(transform)
	, colliderBody(colliderBody)
	, colliderFront(colliderFront)
	, offset(offset)
{
    timeManager = TimeManager::getInstance();
    timeManager->attach120FPS(this);

	if(forward.x == 1.0f && forward.z == 0.0f){
		side = 0.0;
	}else
	if(forward.x == -1.0f && forward.z == 0.0f){
		side = 180.0;
	}else
	if(forward.x == 0.0f && forward.z == 1.0f){
		side = 90.0;
	}else
	if(forward.x == 0.0f && forward.z == -1.0f){
		side = 270.0;
	}
	else {
		side = 0.0;
	}

	colliderFront->setCenterOffset(glm::vec2(forward.x * offset, forward.z * offset));

	switch (type) {
	case eLeft:
		moveRob = &RobotMovement::turnLeft;
		break;
	case eRight:
		moveRob = &RobotMovement::turnRight;
		break;
	case eStop:
		moveRob = &RobotMovement::noMove;
		break;
	default:
		moveRob = &RobotMovement::noMove;
	}
}

void RobotMovement::update() {
	transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));

    if (!colliderFront->getTouchingDynamicComponents().empty()) {
        (this->*moveRob)(timeManager->getDeltaTime120FPS());
        return;
    }

    if (!colliderFront->getTouchingStaticComponents().empty())
    for (StaticColliderComponent* staticComp : colliderFront->getTouchingStaticComponents()) {
        if (!staticComp->getIsPassable()) {
            (this->*moveRob)(timeManager->getDeltaTime120FPS());
            break;
        }
    }
}

// turns robot right (only with right angle)
void RobotMovement::turnRight(float dTime) {
	side += 90.f;
	if(side > 270.f)
		side = 0.0f;
	//transform->addToLocalPosition(forward * -(speed * dTime));
	transform->rotateLocal(glm::vec3(0.0f, -90.0f, 0.0f));
	glm::vec3 front;
	front.x = cos(glm::radians(side));
	front.y = 0.f;
	front.z = sin(glm::radians(side));
	forward = glm::normalize(front);
	colliderFront->setCenterOffset(glm::vec2(forward.x * offset, forward.z * offset));
}

// turns robot left (only with right angle)
void RobotMovement::turnLeft(float dTime) {
	side -= 90.f;
	if (side < 0.f)
		side = 270.0f;
	//transform->addToLocalPosition(forward * -(speed * dTime));
	transform->rotateLocal(glm::vec3(0.0f, 90.0f, 0.0f));
	glm::vec3 front;
	front.x = cos(glm::radians(side));
	front.y = 0.f;
	front.z = sin(glm::radians(side));
	forward = glm::normalize(front);
	colliderFront->setCenterOffset(glm::vec2(forward.x * offset, forward.z * offset));
}

void RobotMovement::noMove(float dTime) {
	transform->addToLocalPosition(forward * -(speed * dTime));
}
