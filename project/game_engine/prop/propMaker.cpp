
#include "propMaker.h"

#include "../renderer/objectRenderer.h"
#include "../entity/particleGenerator.h"
#include "../entity/player.h"
#include "../../utils/constants.h"
#include "../entity/instancedObject.h"
#include "../renderer/instancedRenderer.h"
#include "../entity/renderableEntityMaker.h"

#include "../mesh/heightMap.h"

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
    prop->addEntity(sphere);
    prop->addEntity(light);
    prop->setMainObject(sphere);

    return prop;
}

std::shared_ptr<Prop> PropMaker::makePlane()
{
    const auto plane = RenderableEntityMaker::makeRenderable<Player, ObjectRenderer>("firecamp", std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/plane/uploads_files_6592991_Model.obj"));

    plane->setPosition(glm::vec3(1, 30, -5));
    auto prop = std::make_shared<Prop>();

    prop->addRenderable(plane);
    prop->setMainObject(plane);
    return prop;
}

std::shared_ptr<Prop> PropMaker::makeFirecamp(const float x, const float z, const std::shared_ptr<HeightMap> &heightMap)
{
    const auto firecamp = Object::make(
        std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/Campfire/Campfire OBJ.obj"),
        "firecamp");

    firecamp->setPosition(glm::vec3(x, heightMap->getHeight(x, z), z));
    firecamp->setScale(glm::vec3(0.2, 0.2, 0.2));

    auto prop = std::make_shared<Prop>();
    auto light = PropMaker::makeLamp(
        glm::vec3(1.0, 15.0, 1.5), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0),
        glm::vec4(0.1, 0.9, 1, 32), glm::vec3(0.5, 0.1, 0));

    auto pg = ParticleGenerator::make(ParticleParams{
        .spread = 0.2,
        .range = 0.5,
        .initialSize = 0.1,
        .maxLife = 2,
        .color1 = glm::vec3(1.0f, 1.0f, 0.8f),
        .color2 = glm::vec3(1.0f, 0.5f, 0.0f),
        .color3 = glm::vec3(0.5f, 0.0f, 0.0f)});

    constexpr auto offset = glm::vec3(0, 0, 2.6);

    firecamp->attach(light->getMainObject(), offset);
    firecamp->attach(pg, offset);
    prop->addRenderable(firecamp);
    prop->addRenderable(pg);
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

        const int chunkX = static_cast<int>(std::floor(x / static_cast<float>(chunkSize)));
        const int chunkY = static_cast<int>(std::floor(z / static_cast<float>(chunkSize)));

        const int ix = chunkX - minChunk;
        const int iy = chunkY - minChunk;

        const int index = iy * chunksPerAxis + ix;

        treeMatrices[index].push_back(model);
    }

    const auto mesh = std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/Tree_V10_OBJ/Tree.obj");
    auto prop = std::make_shared<Prop>();

    for (const auto& v : treeMatrices)
    {
        if (v.empty()) continue;

        auto tree = InstancedObject::make(mesh, "tree", v);

        prop->addEntity(tree);
        prop->addRenderable(tree);

        std::cout << "size " << v.size() << std::endl;
        auto bounds = tree->getBounds();
        for (auto& b: bounds)
            printVec3(b);
    }

    return prop;
}
