#include "TextureManager.hpp"

std::shared_ptr<TextureManager> TextureManager::manager_ = nullptr;

std::shared_ptr<TextureManager> TextureManager::getManager()
{
    if (manager_ == nullptr) 
        manager_ = std::shared_ptr<TextureManager>(new TextureManager());
        
    return manager_;
}

const sf::Texture* TextureManager::getTexture(const std::string& name) const
{
    auto it = textures_.find(INSTALL_DIR + prefix_ + name);
    if (it != textures_.end()) return &(it->second);
    else return nullptr;
}

sf::Texture* TextureManager::loadTextureFromFile(const std::string& filename)
{
    sf::Texture texture;
    std::string file = INSTALL_DIR + prefix_ + filename;

    if (texture.loadFromFile(file))
    {
        textures_[INSTALL_DIR + file] = texture;
        return &textures_[INSTALL_DIR + file];
    }
    else return nullptr;
}

TextureManager::TextureManager()
    : prefix_("app_data/")
{
    loadAllTexturesFromDirectory(std::string(INSTALL_DIR) + prefix_ + "textures/interface/buttons/");
    loadAllTexturesFromDirectory(std::string(INSTALL_DIR) + prefix_ + "textures/interface/windows/");
    loadAllTexturesFromDirectory(std::string(INSTALL_DIR) + prefix_ + "textures/marks/");
    loadAllTexturesFromDirectory(std::string(INSTALL_DIR) + prefix_ + "textures/ships/");
}

void TextureManager::loadAllTexturesFromDirectory(const std::string& dir)
{
    for(const auto& file : fs::directory_iterator(dir))
    {
        if (fs::is_regular_file(file))
        {
            std::string filename = file.path().filename();
            std::string texture_name = file.path().stem();
            
            textures_[dir + texture_name].loadFromFile(dir + filename);
        }
    }
}
