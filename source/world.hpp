#pragma once

// C Headers
#include <cstdint>

// C++ Headers
#include <fstream>

// SFML Headers
#include <SFML/System/Vector2.hpp>

struct Block {
    uint32_t m_id;
};

class Entity {
public:
    Entity(uint32_t p_id, sf::Vector2<float> p_pos) {
        m_id = p_id;
        m_position = p_pos;
    }

    uint32_t GetId() {
        return m_id;
    }

private:
    uint32_t m_id;
    sf::Vector2<float> m_position;
};

class World {
public:
    World() {

    }

    World(uint32_t p_size){
        World(p_size, p_size);
    }

    World(uint32_t p_sizeW, uint32_t p_sizeH){
        m_sizeWidth = p_sizeW;
        m_sizeHeight = p_sizeH;
        m_blocks = new Block[m_sizeWidth * m_sizeHeight];
        m_ready = true;
    }

    World(const std::string p_Path) {
        std::cout << "loading map\n";
        std::fstream mapfile;
        mapfile.open(p_Path);
        mapfile >> m_sizeWidth >> m_sizeHeight;
        m_blocks = new Block[m_sizeWidth * m_sizeHeight];
        m_ready = true;
        for(int i = 0; i < m_sizeWidth * m_sizeHeight; i++){
            uint32_t l_id;
            mapfile >> l_id;
            m_blocks[i] = Block{l_id};
        }
        m_generated = true;
        std::cout << "loaded map\n";
    }

    // FIXME: create normal generator
    bool Generate(){
        for(int i = 0; i < m_sizeWidth * m_sizeHeight; i++){
            m_blocks[i] = Block{(uint32_t)i % 32};
        }
        m_generated = true;
        return true;
    }

    sf::Vector2u GetSize(){
        std::cout << m_sizeWidth << ' ' << m_sizeHeight << '\n';
        return {m_sizeWidth, m_sizeHeight};
    }

    bool GetBlock(uint32_t t_x, uint32_t t_y, Block*&t_b){
        if (t_x < 0 || t_x > m_sizeWidth || t_y < 0 || t_y > m_sizeHeight){
            return false;
        }
        t_b = &m_blocks[t_x + t_y * m_sizeWidth];
        return true;
    }

    bool SetBlock(uint32_t t_x, uint32_t t_y, Block p_block){
        if (t_x < 0 || t_x > m_sizeWidth || t_y < 0 || t_y > m_sizeHeight){
            return false;
        }
        m_blocks[t_x + t_y * m_sizeWidth] = p_block;
        return true;
    }

private:
    uint32_t m_sizeWidth = 0;
    uint32_t m_sizeHeight = 0;
    bool m_ready = false;
    bool m_generated = false;
    Block* m_blocks = nullptr;
};