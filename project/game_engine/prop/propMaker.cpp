
#include "propMaker.h"

#include "../renderer/objectRenderer.h"
#include "../../utils/constants.h"
#include "../entity/instancedObject.h"
#include "../renderer/instancedRenderer.h"

std::shared_ptr<Prop> PropMaker::makeLamp(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &color,
                                          const glm::vec4 &lightProperties, const glm::vec3 &lightAttenuation)
{
    const auto sphere = Object::make(
        std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/sphere_smooth.obj"),
        "solid");
    const auto light = Light::make();
    sphere->setScale(scale);

    sphere->attach(light);
    sphere->setColor(color);
    light->setColor(color);

    light->setAttenuation(lightAttenuation.x, lightAttenuation.y, lightAttenuation.z);
    light->setProperties(lightProperties.x, lightProperties.y, lightProperties.z, lightProperties.w);
    sphere->setPosition(position);
    sphere->setScale(glm::vec3(4, 4, 4));

    auto prop = std::make_shared<Prop>();

    prop->addRenderable(sphere);
    prop->addEntity(sphere);
    prop->addEntity(light);
    prop->setMainObject(sphere);

    return prop;
}

std::shared_ptr<Prop> PropMaker::makePlane(const std::shared_ptr<TerrainMesh> &heightMap)
{
    const auto plane = Object::make(
        std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/AirPlane/11803_Airplane_v1_l1.obj"),
        "firecamp");

    plane->setPosition(glm::vec3(1, 30, -5));
    plane->setScale(glm::vec3(0.01, 0.01, 0.01));
    plane->rotate(0.5, 0.1);
    auto prop = std::make_shared<Prop>();

    prop->addRenderable(plane);
    return prop;
}

std::shared_ptr<Prop> PropMaker::makeFirecamp(const std::shared_ptr<TerrainMesh> &heightMap)
{

    const auto firecamp = Object::make(
        std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/Campfire/Campfire OBJ.obj"),
        "firecamp");

    firecamp->setPosition(glm::vec3(1, heightMap->getHeight(1, -5.0), -5));
    firecamp->setScale(glm::vec3(0.2, 0.2, 0.2));

    auto prop = std::make_shared<Prop>();
    auto whiteLight = PropMaker::makeLamp(
        glm::vec3(1.0, 15.0, 1.5), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0),
        glm::vec4(0.1, 0.9, 1, 32), glm::vec3(0.5, 0.1, 0));

    firecamp->attach(whiteLight->getMainObject(), glm::vec3(0, 0, 0));
    prop->addRenderable(firecamp);
    return prop;
}

std::shared_ptr<Prop> PropMaker::makeTrees(const std::shared_ptr<TerrainMesh> &heightMap)
{
    std::vector<glm::mat4> treeMatrices;

    constexpr int maxRandom = PLAN_SIZE_X / 2;
    constexpr int minRandom = -PLAN_SIZE_X / 2;

    for (int i = 0; i < 200; i++)
    {
        auto model = glm::mat4(1.0f);
        const float x = minRandom + static_cast<float>(rand()) / RAND_MAX * (maxRandom - minRandom);
        const float z = minRandom + static_cast<float>(rand()) / RAND_MAX * (maxRandom - minRandom);
        const float y = heightMap->getHeight(x, z);

        if (y <= WATER_HEIGHT)
        {
            i--;
            continue;
        }
        model = glm::translate(model, glm::vec3(x, y, z));
        model = glm::rotate(model, static_cast<float>(rand() % 360), glm::vec3(0, 1, 0));

        treeMatrices.push_back(model);
    }

    const auto tree =
        InstancedObject::make(
            std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/Tree_V10_OBJ/Tree.obj"),
            "tree", treeMatrices);

    auto prop = std::make_shared<Prop>();

    prop->addEntity(tree);
    prop->addRenderable(tree);
    return prop;
}
