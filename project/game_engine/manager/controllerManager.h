
#ifndef INFOH502_CPP_CONTROLLERMANAGER_H
#define INFOH502_CPP_CONTROLLERMANAGER_H
#include "../entity/camera.h"

class Player;

class ControllerManager
{
    ControllerManager()=default;
    std::shared_ptr<Player> _player = nullptr;
    bool _isPlayerControlled = DEFAULT_CAMERA_LOCKED_ON_PLAYER;

    static std::shared_ptr<ControllerManager> _instance;
public:

    static std::shared_ptr<ControllerManager> get();
    void setPlayer(const std::shared_ptr<Player> &player);
    [[nodiscard]] std::shared_ptr<Controllable> getMainControllable() const;
    void toggleIsPlayerControlled();
    void setIsPlayerControlled(bool value);
};

#endif //INFOH502_CPP_CONTROLLERMANAGER_H
