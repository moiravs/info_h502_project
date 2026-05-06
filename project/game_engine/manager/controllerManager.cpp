#include "controllerManager.h"

#include "../../utils/utils.h"
#include "mainCamera.h"

#include "../entity/player.h"

std::shared_ptr<ControllerManager> ControllerManager::_instance = nullptr;

std::shared_ptr<ControllerManager> ControllerManager::get()
{
    if (!_instance)
    {
        _instance = std::make_shared<ControllerManager>(ControllerManager());
    }

    return _instance;
}

void ControllerManager::setPlayer(const std::shared_ptr<Player>& player)
{
    this->_player = player;
}

std::shared_ptr<Controllable> ControllerManager::getMainControllable() const
{
    if (this->_isPlayerControlled)
    {
        if (!this->_player)
        {
            ERROR("No player given to the Controller Manager.");
        }
        return std::dynamic_pointer_cast<Controllable>(this->_player);
    }
    return MainCamera::get();
}

void ControllerManager::toggleIsPlayerControlled()
{
    this->setIsPlayerControlled(!this->_isPlayerControlled);
}

void ControllerManager::setIsPlayerControlled(const bool value)
{
    this->_isPlayerControlled = value;

    if (!this->_player)
    {
        ERROR("No player given to the Controller Manager.");
    }

    if (this->_isPlayerControlled)
    {
        this->_player->attachCamera();
        MainCamera::get()->forceLookAt(this->_player);
    } else
    {
        this->_player->removeAttachment(MainCamera::get());
        MainCamera::get()->forceLookAt(nullptr);
    }
}
