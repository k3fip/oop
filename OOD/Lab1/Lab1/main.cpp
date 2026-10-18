#include <SFML/Graphics.hpp>
#include "FileProcessor.h"
#include "Config.h"
#include <iostream>

int main() {
    auto shapes = FileProcessor::readShapes(Config::INPUT_FILE);
    if (shapes.empty()) {
        std::cerr << "No shapes loaded or file not found." << std::endl;
        return 1;
    }

    FileProcessor::writeResults(Config::OUTPUT_FILE, shapes);
    std::cout << "Results saved to " << Config::OUTPUT_FILE << std::endl;

    sf::RenderWindow window(
        sf::VideoMode({ Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT }),
        Config::WINDOW_TITLE
    );

    window.setFramerateLimit(Config::FRAMERATE_LIMIT);

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(Config::BACKGROUND_COLOR);

        for (const auto& shape : shapes) {
            shape->draw(window);
        }

        window.display();
    }

    return 0;
}