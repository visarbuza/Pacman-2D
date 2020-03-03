#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <stb_image.h>

// Instantiate static variables
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const std::string &file, GLboolean alpha, std::string name, bool sprite)
{
    if (!sprite)
    {
        Textures[name] = loadTextureFromFile(file, alpha);
        return Textures[name];
    }

    std::vector sprites = loadTexturesFromSprite(file, alpha);
    int i;
    for (i = 0; i < sprites.size(); i++)
    {
        Textures[name+std::to_string(i)] = sprites[i];
    }

    return Textures[name+std::to_string(i-1)];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

std::vector<int> ResourceManager::ReadHighscore(const std::string& file)
{
    std::vector<int> highscores{};
    std::fstream highscoreFile(file);
    return highscores;
}

void ResourceManager::WriteHighscore(int score)
{
    
}

void ResourceManager::Clear()
{
    // (Properly) delete all shaders
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (Properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    // 2. Now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const std::string &file, GLboolean alpha)
{
    // Create Texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // Load image
    int width, height, channels;
    unsigned char *image = stbi_load(file.c_str(), &width, &height, &channels, texture.Image_Format == GL_RGBA ? STBI_rgb_alpha : STBI_rgb);
    // Now generate texture
    texture.Generate(width, height, image);
    // And finally free image data
    stbi_image_free(image);

    return texture;
}

std::vector<Texture2D> ResourceManager::loadTexturesFromSprite(const std::string &file, GLboolean alpha)
{
    std::vector<Texture2D> sprites{};
    // Load image
    int width, height, channels;
    unsigned char *image = stbi_load(file.c_str(), &width, &height, &channels, alpha ? STBI_rgb_alpha : STBI_rgb);
    for (int k = 0; k < 4; k++)
    {
        Texture2D texture;
        if (alpha)
        {
            texture.Internal_Format = GL_RGBA;
            texture.Image_Format = GL_RGBA;
        }

        int i, j = 0;
        const int size = 72;
        unsigned char newImage[size * size * 4];

        for (i = k * size * 4 ; i < width * height * 4; i++)
        {
            if (j >= size * size * 4)
            {
                break;
            }
            newImage[j] = image[i];
            j++;

            if (j % (size * 4) == 0)
            {
                i += (width - size) * 4;
            }
        }

        texture.Generate(size, size, newImage);

        sprites.push_back(texture);
    }
    stbi_image_free(image);

    return sprites;
}
