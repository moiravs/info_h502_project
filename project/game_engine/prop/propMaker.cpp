
#include "propMaker.h"

#include "../../utils/constants.h"
#include "../entity/instancedObject.h"
#include "../entity/particleGenerator.h"
#include "../entity/player.h"
#include "../entity/renderableEntityMaker.h"
#include "../entity/spinner.h"
#include "../renderer/instancedRenderer.h"
#include "../renderer/objectRenderer.h"

#include "../mesh/heightMap.h"
#include <random>

std::shared_ptr<Prop> PropMaker::makeLamp(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &color,
                                          const glm::vec4 &lightProperties, const glm::vec3 &lightAttenuation)
{
    const auto sphere = Object::make(
        std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/sphere_smooth.obj"),
        "solid");
    const auto light = Light::make();

    sphere->attach(light);
    sphere->setColor(color);
    light->setColor(color);

    light->setAttenuation(lightAttenuation.x, lightAttenuation.y, lightAttenuation.z);
    light->setProperties(lightProperties.x, lightProperties.y, lightProperties.z, lightProperties.w);
    sphere->setPosition(position);
    sphere->setScale(scale);

    auto prop = std::make_shared<Prop>();

    prop->addRenderable(sphere);
    prop->setMainObject(sphere);

    return prop;
}

std::shared_ptr<Prop> PropMaker::makePlane(const std::shared_ptr<HeightMap> &heightMap)
{
    const auto plane = RenderableEntityMaker::makeRenderable<Player, ObjectRenderer>("object", std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/plane/restoftheplane.obj"), heightMap);
    const auto spinnyThing = RenderableEntityMaker::makeRenderable<Spinner, ObjectRenderer>("object", std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/plane/helice.obj"), 0, 0, PLANE_SPINNER_RAD_PER_SEC);
    plane->setPosition(glm::vec3(1, 30, -5));
    auto prop = std::make_shared<Prop>();

    const auto lamp = PropMaker::makeLamp(
        glm::vec3(1.0, 15.0, 1.5), glm::vec3(0.1), glm::vec3(1, 0, 0),
        glm::vec4(0.1, 0.9, 1, 32), glm::vec3(5, 0.2, 0));

    plane->attach(lamp->getMainObject(), {0, -0.43, -6.1}, true, true);
    plane->attach(spinnyThing, {0, 0, 0}, true, true, false);

    prop->addRenderable(plane);
    prop->addRenderable(spinnyThing);
    prop->setMainObject(plane);
    prop->addRenderable(lamp);
    return prop;
}

std::shared_ptr<Prop> PropMaker::makeFirecamp(const float x, const float z, const std::shared_ptr<HeightMap> &heightMap)
{
    const auto firecamp = Object::make(
        std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/Campfire/Campfire OBJ.obj"),
        "object");

    firecamp->setPosition(glm::vec3(x, heightMap->getHeight(x, z), z));
    firecamp->setScale(glm::vec3(0.2, 0.2, 0.2));

    auto prop = std::make_shared<Prop>();
    auto light = PropMaker::makeLamp(
        glm::vec3(1.0, 15.0, 1.5), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0),
        glm::vec4(0, 0.3, 0, 1), glm::vec3(0.5, 0.1, 0));

    auto pg = ParticleGenerator::make(ParticleParams{
        .spread = 0.2,
        .range = 0.5,
        .initialSize = 0.1,
        .maxLife = 2,
        .color1 = glm::vec3(1.0f, 1.0f, 0.8f),
        .color2 = glm::vec3(1.0f, 0.5f, 0.0f),
        .color3 = glm::vec3(0.5f, 0.0f, 0.0f)});

    constexpr auto offset = glm::vec3(2.6, 0, 0);

    firecamp->attach(light->getMainObject(), offset, false);
    firecamp->attach(pg, offset);
    prop->addRenderable(firecamp);
    prop->addRenderable(pg);
    prop->setMainObject(firecamp);
    return prop;
}

std::shared_ptr<Prop> PropMaker::makeRings(const std::shared_ptr<HeightMap> &heightMap)
{
    const auto ring = Object::make(
        std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/ring/ring.obj"),
        "object");

    ring->setPosition(glm::vec3(15, 50, 15));
    ring->setScale(glm::vec3(15, 15, 15));
    auto prop = std::make_shared<Prop>();

    prop->addRenderable(ring);
    prop->setMainObject(ring);
    return prop;
}

std::shared_ptr<Prop> PropMaker::makeFlowers(const std::shared_ptr<HeightMap> &heightMap, const int nbTrees, const int chunkSize)
{
    constexpr int maxRandom = PLAN_SIZE_X / 2;
    constexpr int minRandom = -PLAN_SIZE_X / 2;

    const int chunksPerAxis = (PLAN_SIZE_X + chunkSize - 1) / chunkSize;

    const int minChunk = static_cast<int>(std::floor(minRandom / static_cast<float>(chunkSize)));

    std::vector<std::vector<glm::mat4>> treeMatrices(chunksPerAxis * chunksPerAxis);

    for (int i = 0; i < nbTrees; i++)
    {
        glm::mat4 model(1.0f);

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
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));

        model = glm::scale(model, glm::vec3(0.15));
        const int chunkX = static_cast<int>(std::floor(x / static_cast<float>(chunkSize)));
        const int chunkY = static_cast<int>(std::floor(z / static_cast<float>(chunkSize)));

        const int ix = chunkX - minChunk;
        const int iy = chunkY - minChunk;

        const int index = iy * chunksPerAxis + ix;

        treeMatrices[index].push_back(model);
    }

    // const auto crocus = std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/crocus/12974_crocus_flower_v1_l3.obj");
    const auto daffodil = std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/daffodil/12977_Daffodil_flower_v1_l2.obj");
    const auto maki = std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/maki/maki.obj");
    const auto snowdrop = std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/snowdrop/12970_snowdrop_flower_v1_l2.obj");
    auto prop = std::make_shared<Prop>();

    const std::vector<std::shared_ptr<Mesh>> meshes = {snowdrop, maki, daffodil};
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, meshes.size() - 1);

    for (const auto &v : treeMatrices)
    {
        if (v.empty())
            continue;

        auto tree = InstancedObject::make(meshes[distr(eng)], "instancedObject", v);

        prop->addRenderable(tree);
    }
    return prop;
}

std::shared_ptr<Prop> PropMaker::makeTrees(const std::shared_ptr<HeightMap> &heightMap, const int nbTrees, const int chunkSize)
{
    constexpr int maxRandom = PLAN_SIZE_X / 2;
    constexpr int minRandom = -PLAN_SIZE_X / 2;

    const int chunksPerAxis = (PLAN_SIZE_X + chunkSize - 1) / chunkSize;

    const int minChunk = static_cast<int>(std::floor(minRandom / static_cast<float>(chunkSize)));

    std::vector<std::vector<glm::mat4>> treeMatrices(chunksPerAxis * chunksPerAxis);

    for (int i = 0; i < nbTrees; i++)
    {
        glm::mat4 model(1.0f);

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
        model = glm::scale(model, glm::vec3(1.5));

        const int chunkX = static_cast<int>(std::floor(x / static_cast<float>(chunkSize)));
        const int chunkY = static_cast<int>(std::floor(z / static_cast<float>(chunkSize)));

        const int ix = chunkX - minChunk;
        const int iy = chunkY - minChunk;

        const int index = iy * chunksPerAxis + ix;

        treeMatrices[index].push_back(model);
    }

    const auto mesh = std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/Tree_V10_OBJ/Tree.obj");
    auto prop = std::make_shared<Prop>();

    for (const auto &v : treeMatrices)
    {
        if (v.empty())
            continue;

        auto tree = InstancedObject::make(mesh, "instancedObject", v);

        prop->addRenderable(tree);
    }
    return prop;
}
