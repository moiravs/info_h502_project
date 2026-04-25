
#include "idmanager.h"

size_t IDManager::id = 0;

size_t IDManager::generateID()
{
    return id++;
}
