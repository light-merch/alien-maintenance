#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace sf;
using namespace std;

int HEIGHT = 600, WIDTH = 800; // Default size of the window
float TILES = 10; // Number of tiles (sprites) in a column
int TILE_SIZE = 32; // Tile size in viewport points

int MAP[50][50][4]; // Map

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
            mapfile.open(argument, ios::out | ios::binary );
            if (!mapfile.fail()) {
                mapfile.read((char*)&MAP, sizeof(MAP));
                mapfile.close();
                mapReceived = true;
                continue;
            }
            else{
                cout << "error loading texture" << '\n';
            }
        }
        else {
            cout << "unknown command" << '\n';
        }
    }

    bool mouseClicked;
    bool shiftPressed;

    View mainGrid(Vector2f(0, 0), Vector2f(WIDTH, HEIGHT));
    View toolsGrid(Vector2f(0, 0), Vector2f(WIDTH, HEIGHT));

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Alien Maintenance");


    for (const auto & entry : filesystem::directory_iterator("../sprites")) {
        if (entry.path() != "../sprites/empty.png") {
            textures.insert(textures.begin(), Texture());
            textures[0].loadFromFile(entry.path());
        }
    }

    textures.insert(textures.begin(), Texture());
    textures[0].loadFromFile("../sprites/empty.png");

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
//                    MAP[(int) worldPos.x / TILE_SIZE][(int) worldPos.y / TILE_SIZE][0] = textureIndex;
                    cout << (event.key.code == Keyboard::LShift);
                    if (shiftPressed && textureIndex != 0) {
                        for (int i = 0; i < 4; i++) {
                            if (MAP[blockX][blockY][i] == 0) {
                                MAP[blockX][blockY][i] = textureIndex;
                            }
                        }
                    } else {
                        for (int i = 0; i < 4; i++) {
                            MAP[blockX][blockY][i] = 0;
                        }
                        MAP[blockX][blockY][0] = textureIndex;
                    }
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
                    mapfile.open(argument, ios::out | ios::binary );
                    mapfile.write((char*)&MAP, sizeof(MAP));
                    mapfile.close();
                }
                if (event.key.code == Keyboard::Escape) {
                    window.close();
                    return 0;
                }
                if (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift) {
                    shiftPressed = true;
                }
            }
            if (event.type == Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift) {
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

        for(int x = 0; x < 50; x++) {
            for(int y = 0; y < 40; y++){
                for(int t = 0; t < 4; t++){
                    Sprite sprite;
                    sprite.setTexture(textures[MAP[x][y][t]]);
                    sprite.setPosition( Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                    window.draw(sprite);
                }
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
