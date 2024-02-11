#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include <unordered_map>
#include "absolute_path.h"
#include <SFML/Graphics.hpp>

namespace fs = std::filesystem;

class TextureManager
{
public:

    static std::shared_ptr<TextureManager> getManager();

    const sf::Texture* getTexture(const std::string&) const;

    sf::Texture* loadTextureFromFile(const std::string&);

private:

    TextureManager();

    TextureManager(const TextureManager&) = delete;

    TextureManager& operator=(const TextureManager&) = delete;

    void loadAllTexturesFromDirectory(const std::string&);

private:

    static std::shared_ptr<TextureManager> manager_;

    std::string prefix_;

    mutable std::unordered_map<std::string, sf::Texture> textures_;
};
