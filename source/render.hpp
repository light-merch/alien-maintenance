#pragma once

// C Headers

// C++ Headers
#include <vector>
#include <string>
#include <map>
#include <filesystem>
#include <iostream>

// SFML Headers
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>

// Own Headers
#include <world.hpp>
#include <textureorder.hpp>

int const ATLAS_SIZE = 1024;
int const TEXTURES_IN_ATLAS_ROW = 32;
int const TEXTURE_SIZE = 32;

class Renderer {
public:
    Renderer(){
        sf::Image TextureBase;
        TextureBase.create(ATLAS_SIZE, ATLAS_SIZE, sf::Color::Magenta);
        m_textureAtlas.create(ATLAS_SIZE, ATLAS_SIZE);
        m_textureAtlas.update(TextureBase);
        LoadTextures(m_textureAtlas, "./sprites/");
    }

    void LoadTextures(sf::Texture &p_atlas, std::string p_path) {
        std::cout << "Loading textures" << '\n';
        sf::Texture lLoadedTexture;

        for(int i = 0; i < texturecount; i++){
            std::cout << p_path << ' ' << order[i] << '\n';
            lLoadedTexture.loadFromFile(p_path + order[i]);
            int x = (i % TEXTURES_IN_ATLAS_ROW) * TEXTURE_SIZE;
            int y = (i / TEXTURES_IN_ATLAS_ROW) * TEXTURE_SIZE;
            std::cout << x << ' ' << y << '\n';
            p_atlas.update(lLoadedTexture, x, y);
        }

        std::cout << "Textures loaded" << '\n';
    }

    void setWindow(sf::RenderWindow* p_rWin){
        m_rWin = p_rWin;
    }

    // FIXME: create normal renderer
    void SimpleRender(int wd, int h, World& wr){
        for(int i = 0; i < wd; i++){
            for(int j = 0; j < h; j++){
                sf::Sprite cell;
                cell.setTexture(m_textureAtlas);
                Block* bp = nullptr;
                wr.GetBlock(i, j, bp);
                int id = bp->m_id;
                int x = id % TEXTURES_IN_ATLAS_ROW;
                int y = id / TEXTURES_IN_ATLAS_ROW;
                cell.setTextureRect(sf::IntRect(x * TEXTURE_SIZE, y * TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_SIZE));
                cell.setPosition(i * TEXTURE_SIZE, j * TEXTURE_SIZE);
                m_rWin->draw(cell);
            }
        }
    }

    void debugTexture(sf::RenderWindow& p_win, int textureid){
        sf::Sprite cell;
        cell.setTexture(m_textureAtlas);
        int x = textureid % TEXTURES_IN_ATLAS_ROW;
        int y = textureid / TEXTURES_IN_ATLAS_ROW;
        cell.setTextureRect(sf::IntRect(x * TEXTURE_SIZE, y * TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_SIZE));
        p_win.draw(cell);
    }

private:
    sf::RenderWindow* m_rWin;
    sf::Texture m_textureAtlas;
};
