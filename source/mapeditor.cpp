#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <textureorder.hpp>

using namespace sf;
using namespace std;

int HEIGHT = 600, WIDTH = 800; // Default size of the window
float TILES = 32; // Number of tiles (sprites) in a column
int TILE_SIZE = 32; // Tile size in viewport points

const int mapsizex = 100, mapsizey = 100;

int MAP[mapsizex][mapsizey]; // Map

int menu{

};

int map{

};

int main() {
    vector<Texture> textures;

    string command, argument;
    bool mapReceived = false;
    while (!mapReceived) {
        ifstream mapfile;
        cout << R"(type "new" to create new empty map, and "load" to load existing map)" << "\n";
        cin >> command;
        if (command == "new") {
            mapReceived = true;
            continue;
        }
        else if (command == "load") {
            cout << "name of map" << '\n';
            cin >> argument;
            mapfile.open(argument, ios::out);
            int d = 0;
            mapfile >> d ;
            mapfile >> d ;
            for(int j = 0; j < mapsizey; j++) {
                for(int i = 0; i < mapsizex; i++) {
                    mapfile >> MAP[i][j];
                }
            }
            mapfile.close();
            mapReceived = true;
            continue;
        }
        else {
            cout << "unknown command" << '\n';
        }
    }

    bool mouseClicked = false;
    bool shiftPressed = false;

    View mainGrid(Vector2f(0, 0), Vector2f(WIDTH, HEIGHT));
    View toolsGrid(Vector2f(0, 0), Vector2f(WIDTH, HEIGHT));

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Map Editor Dead");

    std::string path = "./sprites/";
    for (int i = 0; i < texturecount; i++) {
        textures.insert(textures.end(), Texture());
        textures[i].loadFromFile(path + order[i]);
    }

    int texturesCount = textures.size();

    int textureIndex = 0;
    std::cout << "started main loop\n";
    int icjokdsadsada = 0;
    sf::Clock clockS;
    while (window.isOpen()) {
        // Event handling
        double lastframetime = clockS.getElapsedTime().asSeconds();
        clockS.restart();
        window.setTitle("FUCKS per second " + std::to_string(1.0 / lastframetime));
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
                mainGrid.setViewport(FloatRect(0,0,0.75,1));
                mainGrid.setCenter(WIDTH / 2 * zoom * 0.75 , HEIGHT / 2 * zoom);
                mainGrid.zoom(zoom);

                toolsGrid.setSize(WIDTH * 0.25, HEIGHT);
                toolsGrid.setViewport(FloatRect(0.75,0,0.25,1));
                toolsGrid.setCenter((WIDTH / 2) * zoom * 0.25, HEIGHT / 2 * zoom);
                toolsGrid.zoom(zoom);
            }

            if (event.type == Event::MouseButtonPressed || (event.type == Event::MouseMoved && mouseClicked)) {
                mouseClicked = true;
                Vector2i pixelPos = Mouse::getPosition(window);
                if (pixelPos.x < WIDTH * 0.75) {
                    window.setView(mainGrid);
                    Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                    int blockX = (int) worldPos.x / TILE_SIZE;
                    int blockY = (int) worldPos.y / TILE_SIZE;
                    MAP[blockX][blockY] = textureIndex;
                } else {
                    window.setView(toolsGrid);
                    Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                    int blockX = (int) worldPos.x / TILE_SIZE;
                    int blockY = (int) worldPos.y / TILE_SIZE;
                    int textureNumber = blockX + blockY * (int) (WIDTH * 0.25 / (HEIGHT / TILES));
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
                    cin >> argument;
                    mapfile.open(argument, ios::out);
                    mapfile << mapsizex << '\n';
                    mapfile << mapsizey << '\n';
                    for(int j = 0; j < mapsizey; j++) {
                        for(int i = 0; i < mapsizex; i++) {
                            mapfile << MAP[i][j] << ' ';
                        }
                        mapfile << '\n';
                    }
                    mapfile.close();
                }
                if (event.key.code == Keyboard::Escape) {
                    window.close();
                    return 0;
                }
                if (event.key.code ==  Keyboard::LShift || event.key.code ==  Keyboard::RShift) {
                    shiftPressed = true;
                }
            }
            if (event.type == Event::KeyReleased) {
                if (event.key.code ==  Keyboard::LShift || event.key.code ==  Keyboard::RShift) {
                    shiftPressed = false;
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

        for(int x = 0; x < mapsizex; x++) {
            for(int y = 0; y < mapsizey; y++) {
                Sprite sprite;
                sprite.setTexture(textures[MAP[x][y]]);
                sprite.setPosition( Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                window.draw(sprite);
            }
        }

        window.setView(toolsGrid);

        RectangleShape back(Vector2f(WIDTH * 0.25, HEIGHT));
        back.setFillColor(Color(127,127,127));
        window.draw(back);

        for(int i = 0; i < textures.size(); i++ ) {
            Sprite sprite;
            sprite.setTexture(textures[i]);
            sprite.setPosition( Vector2f(i % (int) (WIDTH * 0.25 / (HEIGHT / TILES)) * TILE_SIZE,
                                         i / (int) (WIDTH * 0.25 / (HEIGHT / TILES)) * TILE_SIZE));
            window.draw(sprite);
        }

        window.display();
        window.clear(Color::White);
    }
}
