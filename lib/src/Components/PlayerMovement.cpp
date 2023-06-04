#define _USE_MATH_DEFINES

#include "PlayerMovement.h"
#include "Transform.h"
#include "TimeManager.h"
#include "DynamicColliderComponent.h"
#include "StaticColliderComponent.h"
#include "TileState.h"
#include "Entity.h"
#include "PickUp.h"
#include "Tool.h"
#include <cmath>
#include <math.h>
#include <iostream>

PlayerMovement::PlayerMovement(GLFWwindow* window, Entity* parent, Transform* transform, DynamicColliderComponent* collider, DynamicColliderComponent* frontCollider, float speed, EPlayerID ID, glm::vec3 forward)
    : Component(parent)
    , forward(forward)
    , speed(speed)
    , transform(transform)
    , collider(collider)
    , frontCollider(frontCollider)
    , ID(ID)
    , window(window)
    , tool(nullptr)
{
    timeManager = TimeManager::getInstance();
    timeManager->attach120FPS(this);

    setForward = forward;
    if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        //std::cout << "Joystick" << std::endl;
        const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
    }
    
    if (glfwJoystickPresent(GLFW_JOYSTICK_2)) {
        //std::cout << "Joystick" << std::endl;
        const float* axes2 = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &axisCount2);
    }
    
}

void PlayerMovement::move()
{
    std::vector<PickUp*> compTypePU;
    std::vector<Tool*> compTypeTool;
    if (ID == Player1) {
            for (auto comp : collider->getTouchingDynamicComponents()) {
                if (comp != nullptr) {
                    comp->getParent()->getComponentsByType(&compTypePU);
                    for (PickUp* pickUp : compTypePU) {
                        if(!pickUp->isUsed)
                            pickUp->use(parent);
                    }
                    comp->getParent()->getComponentsByType(&compTypeTool);
                    for (Tool* tool : compTypeTool) {
                        if (!tool->isPickedUp() && !tool->isRestarting()) {
                            if (this->tool == nullptr) {
                                this->tool = tool;
                                tool->PickedUp(Player1);
                            }
                            else {
                                this->tool->PickedUp(None, parent->transform);
                                this->tool = tool;
                                tool->PickedUp(Player1);

                            }
                        }
                    }
                }
            }


            previousForward = forward;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS
                || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                    //std::cout << "W" << std::endl;
                    forward.x = 1;
                    //transform->addToLocalPosition(forward * (speed*timeManager->getDeltaTime120FPS()));
                }
                else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                    //std::cout << "S" << std::endl;
                    forward.x = -1;
                    //transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                }
                else {
                    forward.x = 0;
                }
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                    //std::cout << "A" << std::endl;
                    forward.z = -1;
                    //transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                }
                else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                    //std::cout << "D" << std::endl;
                    forward.z = 1;
                    //transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                }
                else {
                    forward.z = 0;
                }
                
                forward = glm::vec3{ forward.x,0,forward.z };
                forward = glm::normalize(forward);
                transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                if (previousForward != forward) {
                    //std::cout << "inny" << std::endl;
                    float angle = atan2(setForward.x * forward.z - forward.x * setForward.z, forward.x * forward.z + setForward.x * setForward.z) * (180.0 / M_PI);
                    if (angle == 0 || angle == -0) { angle = 180; }
                    if (setForward == forward) { angle = 0; }
                    if (angle < 0 && angle > -180) { angle = -180 - angle; }
                    //std::cout << -angle << std::endl;
                    transform->setLocalRotation({ 0,-angle,0 });
                }

                //std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
            }

            if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
                //std::cout << "Joystick" << std::endl;
                axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
                //std::cout << "Left Stick X: " << axes[0] << " Y: " << axes[1] << std::endl;
                //std::cout << "Right Stick X: " << axes[2] << " Y: " << axes[3] << std::endl;

                if (abs(axes[0]) > 0 && abs(axes[0]) < 0.1 && abs(axes[1]) > 0 && abs(axes[1]) < 0.1) {
                    forward = glm::vec3{ 0,0,0 };
                }else {
                    forward.x = -axes[1];
                    forward.z = axes[0];

                    forward = glm::vec3{ forward.x,0,forward.z };
                    forward = glm::normalize(forward);
                    transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                    std::cout << forward.x << " " << forward.z << std::endl;
                    if (previousForward != forward) {
                        //w miare plynny obrot
                        float angle = atan2(setForward.x * forward.z - forward.x * setForward.z, forward.x * forward.z + setForward.x * setForward.z) * (180.0 / M_PI);
                        if (angle < 0 && angle > -180) { angle = -180 - angle; }
                        if (forward.x == 1 && (forward.z < 0.1 && forward.z > -0.1)) { angle = 0; }
                        if (forward.x == -1 && (forward.z < 0.1 && forward.z > -0.1)) { angle = 180; }
                        
                        transform->setLocalRotation({ 0,-angle,0 });
                    }

                }
                GLFWgamepadstate state;

                if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
                {
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
                    {
                        std::cout << "DOWN" << std::endl;
                    }
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_B])
                    {
                        std::cout << "RIGHT" << std::endl;
                    }
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_X])
                    {
                        std::cout << "LEFT" << std::endl;
                    }
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_Y])
                    {
                        std::cout << "UP" << std::endl;
                    }
                }

            }
    }
    if (ID == Player2) {
        for (auto comp : collider->getTouchingDynamicComponents()) {
            if (comp != nullptr) {
                comp->getParent()->getComponentsByType(&compTypePU);
                for (PickUp* pickUp : compTypePU) {
                    if (!pickUp->isUsed)
                        pickUp->use(parent);
                }
                comp->getParent()->getComponentsByType(&compTypeTool);
                for (Tool* tool : compTypeTool) {
                    if (!tool->isPickedUp() && !tool->isRestarting()) {
                        if (this->tool == nullptr) {
                            this->tool = tool;
                            tool->PickedUp(Player2);
                        }
                        else {
                            this->tool->PickedUp(None, parent->transform);
                            this->tool = tool;
                            tool->PickedUp(Player2);

                        }
                    }
                }
            }
        }

        previousForward = forward;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
            || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                forward.x = 1;
            }
            else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                forward.x = -1;
            }
            else {
                forward.x = 0;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                forward.z = -1;
            }
            else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                forward.z = 1;
            }
            else {
                forward.z = 0;
            }

            forward = glm::vec3{ forward.x,0,forward.z };
            forward = glm::normalize(forward);
            transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
            if (previousForward != forward) {
                float angle = atan2(setForward.x * forward.z - forward.x * setForward.z, forward.x * forward.z + setForward.x * setForward.z) * (180.0 / M_PI);
                if (angle == 0 || angle == -0) { angle = 180; }
                if (setForward == forward) { angle = 0; }
                if (angle < 0 && angle > -180) { angle = -180 - angle; }
                transform->setLocalRotation({ 0,-angle,0 });
            }

        }
        
        if (glfwJoystickPresent(GLFW_JOYSTICK_2)) {
            //std::cout << "Joystick" << std::endl;
            axes2 = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &axisCount2);
            //std::cout << "Left Stick X: " << axes2[0] << " Y: " << axes2[1] << std::endl;
            //std::cout << "Right Stick X: " << axes2[2] << " Y: " << axes2[3] << std::endl;

            if (abs(axes2[0]) > 0 && abs(axes2[0]) < 0.1 && abs(axes2[1]) > 0 && abs(axes2[1]) < 0.1) {
                forward = glm::vec3{ 0,0,0 };
                //std::cout << forward.x << " " << forward.z << std::endl;
            }
            else {
                forward.x = -axes2[1];
                forward.z = axes2[0];

                forward = glm::vec3{ forward.x,0,forward.z };
                forward = glm::normalize(forward);
                transform->addToLocalPosition(forward * (speed * timeManager->getDeltaTime120FPS()));
                //std::cout << forward.x << " " << forward.z << std::endl;
                if (previousForward != forward) {
                    float angle = atan2(setForward.x * forward.z - forward.x * setForward.z, forward.x * forward.z + setForward.x * setForward.z) * (180.0 / M_PI);
                    if (angle < 0 && angle > -180) { angle = -180 - angle; }
                    if (forward.x == 1 && (forward.z < 0.1 && forward.z > -0.1)) { angle = 0; }
                    if (forward.x == -1 && (forward.z < 0.1 && forward.z > -0.1)) { angle = 180; }

                    transform->setLocalRotation({ 0,-angle,0 });
                }

            }
            GLFWgamepadstate state;

            if (glfwGetGamepadState(GLFW_JOYSTICK_2, &state))
            {
                if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
                {
                    std::cout << "DOWN" << std::endl;
                }
                if (state.buttons[GLFW_GAMEPAD_BUTTON_B])
                {
                    std::cout << "RIGHT" << std::endl;
                }
                if (state.buttons[GLFW_GAMEPAD_BUTTON_X])
                {
                    std::cout << "LEFT" << std::endl;
                }
                if (state.buttons[GLFW_GAMEPAD_BUTTON_Y])
                {
                    std::cout << "UP" << std::endl;
                }
            }

        }
        
    }
}

void PlayerMovement::update() {
    move();
    checkInput();
    handleSeenTile();
}

void PlayerMovement::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

void PlayerMovement::checkInput(){
    switch(ID){
        case Player1:
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
                frontCollider->getTileColliderIAmOn()->getTileState()->changeTileState(ID);
            }
            if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
                GLFWgamepadstate state;
                if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
                {
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
                    {
                        frontCollider->getTileColliderIAmOn()->getTileState()->changeTileState(ID);
                    }
                }
            }

            break;
        case Player2:
            if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
                frontCollider->getTileColliderIAmOn()->getTileState()->changeTileState(ID);
            }
            if (glfwJoystickPresent(GLFW_JOYSTICK_2)) {
                GLFWgamepadstate state;
                if (glfwGetGamepadState(GLFW_JOYSTICK_2, &state))
                {
                    if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
                    {
                        frontCollider->getTileColliderIAmOn()->getTileState()->changeTileState(ID);
                    }
                }
            }
            break;
        default:
            break;
    }
}

void PlayerMovement::handleSeenTile(){
    if(lastSeenTile == nullptr){
        lastSeenTile = frontCollider->getTileColliderIAmOn();
        if(lastSeenTile == nullptr)
            return;

        lastSeenTile->getParent()->switchShader();
    }
    else if(lastSeenTile != frontCollider->getTileColliderIAmOn()) {
        lastSeenTile->getParent()->switchShader();
        lastSeenTile = frontCollider->getTileColliderIAmOn();
        lastSeenTile->getParent()->switchShader();
    }
}
