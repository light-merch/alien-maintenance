#include <SFML/Graphics.hpp>
using namespace sf;

int HEIGHT = 600, WIDTH = 800; // Default size of the window
float TILES = 10; // Number of tiles (sprites) in a column

int main() {
    View view(Vector2f(0, 0), Vector2f(WIDTH, HEIGHT));
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Alien Maintenance");
    window.setView(view);

    Texture floor1;
    floor1.loadFromFile("../sprites/the_alien.gif");

    Sprite floor(floor1);

    while (window.isOpen()) {
        // Event handling
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return 0;
            }

            if (event.type == Event::Resized) {
                view.setSize(event.size.width, event.size.height);

                float zoom = 32 / (event.size.height / TILES);
                view.setCenter(event.size.width / 2 * zoom, event.size.height / 2 * zoom);
                view.zoom(zoom); // 32 is the pixel size of sprites
                window.setView(view);
            }
        }

        window.clear(Color::White);

        window.draw(floor);

        window.display();
    }
}
