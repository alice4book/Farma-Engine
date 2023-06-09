#include "TimeManager.h"
#include "Component.h"
#include "GLFW/glfw3.h"

#define DELTA_TIME_120FPS (7.5f/900.f)
#define DELTA_TIME_60FPS (15.f/900.f)
#define DELTA_TIME_2FPS 0.5f
#define DELTA_TIME_1FPS 1.f

// Define the static Singleton pointer
TimeManager* TimeManager::inst_ = nullptr;

TimeManager* TimeManager::getInstance() {
    if (inst_ == nullptr) {
        inst_ = new TimeManager();
    }
    return(inst_);
}

void TimeManager::updateTime() {
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // skip first frame because of lag on load
    if (deltaTime >= 0.1f) {
        deltaTime = 0.f;
        return;
    }

    notify(listObserverUnlimited);

    if (!sharedPointers.empty()) {
        for (auto component : sharedPointers) {
            component->update();
        }
    }

    deltaTime120FPS += deltaTime;
    deltaTime60FPS += deltaTime;
    deltaTime2FPS += deltaTime;
    deltaTime1FPS += deltaTime;
    time120FPS += deltaTime;
    time60FPS += deltaTime;
    time2FPS += deltaTime;
    time1FPS += deltaTime;

    if (time120FPS >= DELTA_TIME_120FPS) {
        notify(listObserver120FPS);
        deltaTime120FPS = 0.f;
        time120FPS = -DELTA_TIME_120FPS;
    }

    if (time60FPS >= DELTA_TIME_60FPS) {
        notify(listObserver60FPS);
        deltaTime60FPS = 0.f;
        time60FPS -= DELTA_TIME_60FPS;
    }

    if (time2FPS >= DELTA_TIME_2FPS) {
        notify(listObserver2FPS);;
        deltaTime2FPS = 0.f;
        time2FPS -= DELTA_TIME_2FPS;
    }

    if (time1FPS >= DELTA_TIME_1FPS) {
        notify(listObserver1FPS);;
        deltaTime1FPS = 0.f;
        time1FPS -= DELTA_TIME_1FPS;
    }
}

void TimeManager::notify(std::vector<Component*> listObserver) {
    if (listObserver.empty() || dynamic_cast<Component*>(listObserver.front()) == nullptr)
        return;

    for (auto component : listObserver) {
        component->update();
    }
}

void TimeManager::attachUnlimitedFPS(Component* observer) {
    listObserverUnlimited.push_back(observer);
}

void TimeManager::attachUnlimitedFPS(std::shared_ptr<Component> observer) {
    sharedPointers.push_back(observer);
}

void TimeManager::attach120FPS(Component* observer) {
    listObserver120FPS.push_back(observer);
}

void TimeManager::attach60FPS(Component* observer) {
    listObserver60FPS.push_back(observer);
}

void TimeManager::attach2FPS(Component* observer) {
    listObserver2FPS.push_back(observer);
}

void TimeManager::attach1FPS(Component* observer) {
    listObserver1FPS.push_back(observer);
}

void TimeManager::detach(Component* observer) {
    listObserverUnlimited.erase(
        std::remove_if(listObserverUnlimited.begin(), listObserverUnlimited.end(),
            [&observer](const Component* o) { return o == observer; }),
        listObserverUnlimited.end());
    listObserver120FPS.erase(
        std::remove_if(listObserver120FPS.begin(), listObserver120FPS.end(),
            [&observer](const Component* o) { return o == observer; }),
        listObserver120FPS.end());
    listObserver60FPS.erase(
        std::remove_if(listObserver60FPS.begin(), listObserver60FPS.end(),
            [&observer](const Component* o) { return o == observer; }),
        listObserver60FPS.end());
    listObserver2FPS.erase(
        std::remove_if(listObserver2FPS.begin(), listObserver2FPS.end(),
            [&observer](const Component* o) { return o == observer; }),
        listObserver2FPS.end());
    listObserver1FPS.erase(
        std::remove_if(listObserver1FPS.begin(), listObserver1FPS.end(),
            [&observer](const Component* o) { return o == observer; }),
        listObserver1FPS.end());
}

float TimeManager::getDeltaTimeUnlimitedFPS() const {
    return deltaTime;
}

float TimeManager::getDeltaTime120FPS() const {
    return deltaTime120FPS;
}

float TimeManager::getDeltaTime60FPS() const {
    return deltaTime60FPS;
}

float TimeManager::getDeltaTime2FPS() const {
    return deltaTime2FPS;
}

float TimeManager::getDeltaTime1FPS() const {
    return deltaTime1FPS;
}
