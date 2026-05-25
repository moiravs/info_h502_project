
#include "propMaker.h"

#include "../../utils/constants.h"
#include "../entity/instancedObject.h"
#include "../entity/light/directionalLight.h"
#include "../entity/light/pointLight.h"
#include "../entity/particleGenerator.h"
#include "../entity/player.h"
#include "../entity/renderableEntityMaker.h"
#include "../renderer/instancedRenderer.h"
#include "../renderer/objectRenderer.h"
#include "../entity/spinner.h"

#include "../mesh/heightMap.h"
#include <random>
#include <stb_image.h>

#include "../entity/sun.h"

std::pair<std::shared_ptr<HeightMap>, std::shared_ptr<Prop>> PropMaker::terrainFromTexture(const std::string &texturePath,
                                                                                           const float width, const float depth)
{
    int imageWidth;
    int imageHeight;
    stbi_set_flip_vertically_on_load(true);
    int nrChannels;
    unsigned char *data = stbi_load(texturePath.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    std::vector<float> heights(imageWidth * imageHeight);

    constexpr float yScale = 64.0f / 256.0f;

    auto idx = [&](const int x, const int z)
    {
        return z * imageWidth + x;
    };

    for (int z = 0; z < imageHeight; z++)
    {
        for (int x = 0; x < imageWidth; x++)
        {
            constexpr float yShift = 16.0f;
            const unsigned char *pixel = data + (x + imageWidth * z) * nrChannels;
            const float h = pixel[0] * yScale - yShift;

            heights[idx(x, z)] = h;
        }
    }

    std::vector<glm::vec3> normals(imageWidth * imageHeight);

    for (int z = 0; z < imageHeight; z++)
    {
        for (int x = 0; x < imageWidth; x++)
        {
            normals[idx(x, z)] = calculateNormal(x, z,
                                                 imageWidth,
                                                 imageHeight,
                                                 heights);
        }
    }

    auto heightMap = std::make_shared<HeightMap>(width, depth, imageWidth, imageHeight, heights);
    auto prop = std::make_shared<Prop>();

    for (int cz = 0; cz < imageHeight - 1; cz += CHUNK_SIZE)
    {
        for (int cx = 0; cx < imageWidth - 1; cx += CHUNK_SIZE)
        {
            auto mesh = std::make_shared<Mesh>();

            std::vector<Vertex> vertices;
            vertices.reserve((CHUNK_SIZE + 1) * (CHUNK_SIZE + 1));
            std::vector<unsigned> indices;
            indices.reserve(CHUNK_SIZE * CHUNK_SIZE * 6);

            int maxX = std::min(cx + CHUNK_SIZE, imageWidth - 1);
            int maxZ = std::min(cz + CHUNK_SIZE, imageHeight - 1);

            for (int z = cz; z <= maxZ; z++)
            {
                for (int x = cx; x <= maxX; x++)
                {
                    float xPercent = static_cast<float>(x) / (imageWidth - 1);
                    float zPercent = static_cast<float>(z) / (imageHeight - 1);

                    float vx = (xPercent - 0.5f) * width;
                    float vz = (zPercent - 0.5f) * depth;
                    float vy = heights[idx(x, z)];

                    glm::vec3 pos(vx, vy, vz);

                    float u = xPercent;
                    float v = zPercent;

                    glm::vec2 uv(u, v);

                    glm::vec3 n = normals[idx(x, z)];

                    vertices.push_back(Vertex{pos, uv, n});
                }
            }
            int chunkWidth = maxX - cx + 1;

            for (int z = 0; z < (maxZ - cz); z++)
            {
                for (int x = 0; x < (maxX - cx); x++)
                {
                    int i0 = z * chunkWidth + x;
                    int i1 = i0 + 1;
                    int i2 = i0 + chunkWidth;
                    int i3 = i2 + 1;

                    indices.push_back(i0);
                    indices.push_back(i2);
                    indices.push_back(i1);

                    indices.push_back(i1);
                    indices.push_back(i2);
                    indices.push_back(i3);
                }
            }
            MeshEntry entry{};
            entry.init(vertices, indices);
            entry.materialIndex = 0;
            mesh->addEntry(entry);
            std::shared_ptr<Object> object = Object::make(mesh, "cpu_height");
            object->setMaterial(0.5, 1);

            prop->addRenderable(object);
        }
    }
    stbi_image_free(data);
    return {heightMap, prop};
}

glm::vec3 PropMaker::calculateNormal(const int x, const int z, const int imageWidth, const int imageHeight,
                                     const std::vector<float> &heights)
{
    auto idx = [&](const int _x, const int _z)
    {
        return _z * imageWidth + _x;
    };
    // Get heights of neighbors (with bounds checking)
    const float hL = (x > 0) ? heights[idx((x - 1), z)] : heights[idx(x, z)];
    const float hR = (x < imageWidth - 1) ? heights[idx(x + 1, z)] : heights[idx(x, z)];
    const float hD = (z > 0) ? heights[idx(x, z - 1)] : heights[idx(x, z)];
    const float hU = (z < imageHeight - 1) ? heights[idx(x, z + 1)] : heights[idx(x, z)];

    // Deduce normal from height difference
    // The constant (2.0) should match your grid spacing for perfect accuracy
    const glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));
    return normal;
}

std::shared_ptr<Prop> PropMaker::makeLamp(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &color,
                                          const glm::vec3 &lightProperties, const glm::vec3 &lightAttenuation)
{
    const auto sphere = Object::make(
        std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/sphere_smooth.obj"),
        "solid");
    const auto light = PointLight::make();

    sphere->attach(light);
    sphere->setColor(color);
    light->setColor(color);

    light->setAttenuation(lightAttenuation.x, lightAttenuation.y, lightAttenuation.z);
    light->setProperties(lightProperties.x, lightProperties.y, lightProperties.z);
    sphere->setPosition(position);
    sphere->setScale(scale);

    auto prop = std::make_shared<Prop>();

    prop->addRenderable(sphere);
    prop->setMainObject(sphere);

    return prop;
}

std::pair<std::shared_ptr<Prop>, std::shared_ptr<DirectionalLight>> PropMaker::makeSun(const glm::vec3 &position,
                                                                                       const glm::vec3 &scale, const glm::vec3 &color)
{
    const auto directionalLight = DirectionalLight::make();

    const auto sun = RenderableEntityMaker::makeRenderable<Sun, ObjectRenderer>("solid", std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/sphere_smooth.obj"), SUN_SPEED, SUN_PATH_RADIUS);
    const auto light = PointLight::make();

    light->setAttenuation(1, 0, 0);
    light->setProperties(0.5, 0.9, 1);
    light->setColor(color);
    sun->setColor(color);
    sun->setScale(scale);
    sun->attach(light);

    directionalLight->setTarget({0, 0, 0});
    sun->attach(directionalLight);
    directionalLight->setColor(color);

    sun->setPosition(position);

    auto prop = std::make_shared<Prop>();
    prop->addRenderable(sun);
    prop->setMainObject(sun);

    return {prop, directionalLight};
}

std::pair<std::shared_ptr<Prop>, std::shared_ptr<Prop>> PropMaker::makePlane(const std::shared_ptr<HeightMap> &heightMap)
{
    const auto plane = RenderableEntityMaker::makeRenderable<Player, ObjectRenderer>("object", std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/plane/restoftheplane.obj"), heightMap);
    const auto spinnyThing = RenderableEntityMaker::makeRenderable<Spinner, ObjectRenderer>("object", std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/plane/helice.obj"), 0, 0, PLANE_SPINNER_RAD_PER_SEC);
    plane->setPosition(glm::vec3(1, 30, -5));
    auto prop = std::make_shared<Prop>();

    const auto lamp = PropMaker::makeLamp(
        glm::vec3(1.0, 15.0, 1.5), glm::vec3(0.1), glm::vec3(1, 1, 1),
        glm::vec3(0.1, 3, 1), glm::vec3(1, 1, 0));

    const auto lamp2 = PropMaker::makeLamp(
        glm::vec3(1.0, 15.0, 1.5), glm::vec3(0.1), glm::vec3(1, 0, 0),
        glm::vec3(0.1, 3, 1), glm::vec3(1, 1, 0));

    const auto lamp3 = PropMaker::makeLamp(
        glm::vec3(1.0, 15.0, 1.5), glm::vec3(0.1), glm::vec3(0, 1, 0),
        glm::vec3(0.1, 3, 1), glm::vec3(1, 1, 0));

    plane->attach(lamp->getMainObject(), {0, -0.26, -6.3}, false, true);
    plane->attach(lamp2->getMainObject(), {-3.43, -0.2, 0.5}, true, true);
    plane->attach(lamp3->getMainObject(), {3.43, -0.2, 0.5}, true, true);
    plane->attach(spinnyThing, {0, 0, 0}, true, true, false);

    plane->setMaterial(1, 256);

    prop->addRenderable(plane);
    prop->addRenderable(spinnyThing);
    prop->setMainObject(plane);
    prop->addRenderable(lamp);
    return {prop, lamp};
}

std::pair<std::shared_ptr<Prop>, std::shared_ptr<ParticleGenerator>> PropMaker::makeFirecamp(const float x, const float z, const std::shared_ptr<HeightMap> &heightMap)
{
    const auto firecamp = Object::make(
        std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/Campfire/Campfire OBJ.obj"),
        "object");

    firecamp->setPosition(glm::vec3(x, heightMap->getHeight(x, z), z));
    firecamp->setScale(glm::vec3(0.2, 0.2, 0.2));

    firecamp->setMaterial(0.5, 1);

    auto prop = std::make_shared<Prop>();
    const auto light = PropMaker::makeLamp(
        glm::vec3(1.0, 15.0, 1.5), glm::vec3(1, 1, 1), glm::vec3(1.0f, 0.5f, 0.0f),
        glm::vec3(0.5, 2, 0.1), glm::vec3(1, 0.1, 0));

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
    firecamp->attach(pg, offset, false);
    prop->addRenderable(firecamp);
    prop->addRenderable(pg);
    prop->setMainObject(firecamp);
    return {prop, pg};
}

std::shared_ptr<Prop> PropMaker::makeRings(const std::shared_ptr<HeightMap> &heightMap)
{
    auto ringMesh = std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/ring/torus1.obj");

    // Manually add a texture to the mesh's texture vector
    // Assuming materialIndex 0 is what the torus uses
    if (ringMesh->m_Textures.size() < 1)
        ringMesh->m_Textures.resize(1);
    ringMesh->m_Textures[0] = new Texture(PATH_TO_SRC "/../assets/textures/gold_diffuse.jpg");

    // 2. FORCE every sub-mesh entry to point to material 0
    for (auto &entry : ringMesh->getEntries())
    {
        const_cast<MeshEntry &>(entry).materialIndex = 0;
    }
    auto prop = std::make_shared<Prop>();

    constexpr int maxRandom = PLAN_SIZE_X / 2;
    constexpr int minRandom = -PLAN_SIZE_X / 2;

    constexpr int maxHeightRandom = 10;
    constexpr int minHeightRandom = 50;

    for (int i = 0; i < 10; i++)
    {
        const auto ring = Object::make(ringMesh, "object");
        ring->setScale(glm::vec3(4, 4, 4));
        const float x = minRandom + static_cast<float>(rand()) / RAND_MAX * (maxRandom - minRandom);
        const float z = minRandom + static_cast<float>(rand()) / RAND_MAX * (maxRandom - minRandom);
        const float y = glm::max(minHeightRandom + static_cast<float>(rand()) / RAND_MAX * (maxHeightRandom - minHeightRandom), heightMap->getHeight(x, z) + 10);

        auto l = PointLight::make();
        l->setColor(glm::vec3(1, 1, 0));
        l->setProperties(0.5, 2, 0.1);
        l->setAttenuation(1, 0.1, 0);
        ring->attach(l, glm::vec3(2, 2, 0));
        ring->setPosition(glm::vec3(x, y, z));
        ring->rotate(0, glm::radians(90.0f), 0);
        ring->setMaterial(1, 256);
        prop->addRenderable(ring);
    }
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

        model = glm::scale(model, glm::vec3(0.8));
        const int chunkX = static_cast<int>(std::floor(x / static_cast<float>(chunkSize)));
        const int chunkY = static_cast<int>(std::floor(z / static_cast<float>(chunkSize)));

        const int ix = chunkX - minChunk;
        const int iy = chunkY - minChunk;

        const int index = iy * chunksPerAxis + ix;

        treeMatrices[index].push_back(model);
    }

    const auto sunflower = std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/sunflower/Sunflowers set.obj");
    auto prop = std::make_shared<Prop>();

    const std::vector<std::shared_ptr<Mesh>> meshes = {sunflower};
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
        model = glm::scale(model, glm::vec3(1));

        const int chunkX = static_cast<int>(std::floor(x / static_cast<float>(chunkSize)));
        const int chunkY = static_cast<int>(std::floor(z / static_cast<float>(chunkSize)));

        const int ix = chunkX - minChunk;
        const int iy = chunkY - minChunk;

        const int index = iy * chunksPerAxis + ix;

        treeMatrices[index].push_back(model);
    }

    const auto mesh = std::make_shared<Mesh>(PATH_TO_SRC "/../assets/models/Tree_V10_OBJ/RedDeliciousApple.obj");
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
