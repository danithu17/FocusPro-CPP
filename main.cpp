#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

// Apple-style Glass Card (Dashboard)
class AppleDashboard {
public:
    sf::RectangleShape card;
    sf::Text title;
    sf::Text stats;
    std::vector<sf::RectangleShape> graphBars;

    AppleDashboard(sf::Font& font) {
        // Main Card
        card.setSize({440.f, 180.f});
        card.setFillColor(sf::Color(255, 255, 255, 30));
        card.setOutlineThickness(1.5f);
        card.setOutlineColor(sf::Color(255, 255, 255, 60));
        card.setPosition(30, 430);

        title.setFont(font);
        title.setString("Focus Dashboard");
        title.setCharacterSize(22);
        title.setFillColor(sf::Color::White);
        title.setPosition(50, 445);

        stats.setFont(font);
        stats.setString("Total: 12.5 hrs  |  Streak: 5 Days");
        stats.setCharacterSize(16);
        stats.setFillColor(sf::Color(200, 200, 200));
        stats.setPosition(50, 480);

        // Simple Graph Simulation
        for (int i = 0; i < 7; i++) {
            sf::RectangleShape bar({30.f, -(float)(rand() % 60 + 20)});
            bar.setPosition(70.f + (i * 50), 580.f);
            bar.setFillColor(sf::Color(0, 122, 255, 180)); // Apple Blue
            graphBars.push_back(bar);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(card);
        window.draw(title);
        window.draw(stats);
        for (auto& bar : graphBars) window.draw(bar);
    }
};

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode({500, 650}), "FocusPro v5 - Apple Experience", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/segoeui.ttf")) return -1;

    AppleDashboard dashboard(font);

    int timeLeft = 25 * 60;
    bool isRunning = false;
    sf::Clock clock;

    // UI - Main Timer Ring
    sf::CircleShape ring(130.f);
    ring.setOutlineThickness(8.f);
    ring.setOutlineColor(sf::Color(60, 60, 60));
    ring.setFillColor(sf::Color::Transparent);
    ring.setOrigin({130.f, 130.f});
    ring.setPosition({250.f, 210.f});

    sf::Text timerText(font, "25:00", 70);
    timerText.setFillColor(sf::Color::White);

    // Modern Pill Button
    sf::RectangleShape btn({220.f, 60.f});
    btn.setFillColor(sf::Color(0, 122, 255));
    btn.setOrigin({110.f, 30.f});
    btn.setPosition({250.f, 370.f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (btn.getGlobalBounds().contains(window.mapPixelToCoords({mb->position.x, mb->position.y}))) {
                    isRunning = !isRunning;
                    btn.setFillColor(isRunning ? sf::Color(255, 59, 48) : sf::Color(0, 122, 255));
                }
            }
        }

        if (isRunning) {
            static float accum = 0;
            accum += clock.restart().asSeconds();
            if (accum >= 1.0f) {
                if (timeLeft > 0) timeLeft--;
                accum = 0;
            }
        } else { clock.restart(); }

        // Update Timer Text
        int m = timeLeft / 60;
        int s = timeLeft % 60;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
        timerText.setString(ss.str());
        timerText.setOrigin({timerText.getLocalBounds().size.x / 2.f, timerText.getLocalBounds().size.y / 2.f});
        timerText.setPosition({250.f, 190.f});

        window.clear(sf::Color(15, 15, 15));
        window.draw(ring);
        window.draw(timerText);
        window.draw(btn);
        dashboard.draw(window);
        window.display();
    }
    return 0;
}