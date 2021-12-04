#include <SFML/Graphics.hpp>
#include <iostream>

#include <world.hpp>
#include <render.hpp>

int WINDOW_WIDTH = 1000;
int WINDOW_HEIGHT = 1000;

void UpdateWindowSize(sf::RenderWindow& p_win, sf::View& p_view, int p_w, int p_h){
    WINDOW_WIDTH = p_w;
    WINDOW_HEIGHT = p_h;
    p_view.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    p_view.setCenter(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
    p_win.setView(p_view);
}

int main(){
    int id = 0;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Alien Maintenance", sf::Style::Default);
    sf::View view;
    UpdateWindowSize(window, view, WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::Clock clock;

    World w("save2");

    Renderer r;
    r.setWindow(&window);

    sf::Vector2u size = w.GetSize();

    while (window.isOpen())
    {

        double lastframetime = clock.getElapsedTime().asSeconds();
        clock.restart();
        window.setTitle("Alien Maintenance " + std::to_string(1.0 / lastframetime));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            }
            if (event.type == sf::Event::Resized) {
                UpdateWindowSize(window, view, event.size.width, event.size.height);
            }
            if (event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Equal){
                    id++;
                }
                if(event.key.code == sf::Keyboard::Dash){
                    id--;
                }
            }
        }
        window.clear();
        r.SimpleRender(size.x, size.y, w);
        window.display();
    }
}