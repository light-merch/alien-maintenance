#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace sf;
using namespace std;

int HEIGHT = 600, WIDTH = 800; // Default size of the window
float TILES = 10; // Number of tiles (sprites) in a column
int TILE_SIZE = 32;

int MAP[50][50][1];

int main() {
    vector<Texture> textures;

    string path;

    bool mapReceived;

    bool mouseClicked;

    while (!mapReceived) {
        ifstream mapfile;
        cin >> path;
        if (path == "-1") {
            mapReceived = true;
            continue;
        }
        else {
            mapfile.open(path, std::ios::out | std::ios::binary );
            if (!mapfile.fail()) {
                mapfile.read((char*)&MAP, sizeof(MAP));
                mapfile.close();
                mapReceived = true;
                continue;
            }
        }
    }


    View mainGrid(Vector2f(0, 0), Vector2f(WIDTH, HEIGHT));
    View toolsGrid(Vector2f(0, 0), Vector2f(WIDTH, HEIGHT));

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Alien Maintenance");


    for (const auto & entry : filesystem::directory_iterator("../sprites")) {
        textures.insert(textures.begin(), sf::Texture());
        textures[0].loadFromFile(entry.path());
    }
    int texturesCount = textures.size();

    int textureIndex = 0;

    while (window.isOpen()) {
        // Event handling
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return 0;
            }

            if (event.type == Event::Resized) {
                HEIGHT = event.size.height;
                WIDTH = event.size.width;

                float zoom = TILE_SIZE / (HEIGHT / TILES);

                mainGrid.setSize(WIDTH * 0.75, HEIGHT);
                mainGrid.setViewport(sf::FloatRect(0,0,0.75,1));
                mainGrid.setCenter(WIDTH / 2 * zoom * 0.75 , HEIGHT / 2 * zoom);
                mainGrid.zoom(zoom);

                toolsGrid.setSize(WIDTH * 0.25, HEIGHT);
                toolsGrid.setViewport(sf::FloatRect(0.75,0,0.25,1));
                toolsGrid.setCenter((WIDTH / 2) * zoom * 0.25, HEIGHT / 2 * zoom);
                toolsGrid.zoom(zoom);
            }

            if (event.type == sf::Event::MouseButtonPressed || (event.type == sf::Event::MouseMoved && mouseClicked)) {
                mouseClicked = true;
                Vector2i pixelPos = Mouse::getPosition(window);
                if (pixelPos.x < WIDTH * 0.75) {
                    window.setView(mainGrid);
                    Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                    MAP[(int) worldPos.x / TILE_SIZE][(int) worldPos.y / TILE_SIZE][0] = textureIndex;
                } else {
                    window.setView(toolsGrid);
                    Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                    int textureNumber = (int) worldPos.x / TILE_SIZE +
                                        (int) worldPos.y / TILE_SIZE * (int) (WIDTH * 0.25 / (HEIGHT / TILES));
                    if (textureNumber < texturesCount) {
                        textureIndex = textureNumber;
                    }
                }
            }
            if (event.type == Event::MouseButtonReleased) {
                mouseClicked = false;
            }
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::S) {
                    ofstream mapfile;
                    cin >> path;
                    mapfile.open(path, std::ios::out | std::ios::binary );
                    mapfile.write((char*)&MAP, sizeof(MAP));
                    mapfile.close();
                }
                if (event.key.code == Keyboard::Escape) {
                    window.close();
                    return 0;
                }
            }
        }

        window.setView(mainGrid);

        for(int i = 0; i <= 100; i++) {
            RectangleShape line(Vector2f(1, window.getSize().y));
            line.setPosition(Vector2f(i * TILE_SIZE, 0));
            line.setFillColor(Color::Black);
            window.draw(line);
        }

        for(int i = 0; i <= 100; i++) {
            RectangleShape line(Vector2f(window.getSize().x, 1));
            line.setPosition(Vector2f(0, i * TILE_SIZE));
            line.setFillColor(Color::Black);
            window.draw(line);
        }

        for(int x = 0; x < 50; x++) {
            for(int y = 0; y < 40; y++){
                sf::Sprite sprite;
                sprite.setTexture(textures[MAP[x][y][0]]);
                sprite.setPosition( Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                window.draw(sprite);
            }
        }

        window.setView(toolsGrid);

        RectangleShape back(Vector2f(WIDTH * 0.25, HEIGHT));
        back.setFillColor(sf::Color(120,120,120));
        window.draw(back);

        for(int i = 0; i <= (int) WIDTH * 0.25 / TILE_SIZE; i++) {
            RectangleShape line(Vector2f(1, WIDTH));
            line.setPosition(Vector2f(i * TILE_SIZE, 0));
            line.setFillColor(Color::White);
            window.draw(line);
        }

        for(int i = 0; i <= 100; i++) {
            int lineWidth = (int) (WIDTH * 0.25 / (HEIGHT / TILES)) * TILE_SIZE;
            RectangleShape line(Vector2f(lineWidth, 1));
            line.setPosition(Vector2f(0, i * TILE_SIZE));
            line.setFillColor(Color::White);
            window.draw(line);
        }

        for(int i = 0; i < textures.size(); i++ ) {
            sf::Sprite sprite;
            sprite.setTexture(textures[i]);
            sprite.setPosition( Vector2f(i % (int) (WIDTH * 0.25 / (HEIGHT / TILES)) * TILE_SIZE,
                                         i / (int) (WIDTH * 0.25 / (HEIGHT / TILES)) * TILE_SIZE));
            window.draw(sprite);
        }

        window.display();
        window.clear(Color::White);
    }
}
