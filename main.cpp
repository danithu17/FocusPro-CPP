#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

void killProcess(string processName) {
    string cmd = "taskkill /F /IM " + processName + " > nul 2>&1";
    system(cmd.c_str());
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(500, 600), "FocusPro v3.3", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) return -1;

    int timeLeft = 25 * 60;
    bool isRunning = false;
    sf::Clock clock;
    float accumulator = 0.0f;

    sf::CircleShape ring(150.f);
    ring.setOutlineThickness(10.f);
    ring.setOutlineColor(sf::Color(46, 204, 113));
    ring.setFillColor(sf::Color::Transparent);
    ring.setOrigin(150.f, 150.f);
    ring.setPosition(250.f, 300.f);

    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(80);
    timerText.setFillColor(sf::Color::White);

    sf::RectangleShape btn(sf::Vector2f(220.f, 65.f));
    btn.setFillColor(sf::Color(46, 204, 113));
    btn.setOrigin(110.f, 32.5f);
    btn.setPosition(250.f, 520.f);

    vector<string> apps = {"chrome.exe", "msedge.exe", "discord.exe"};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mPos(sf::Mouse::getPosition(window));
                if (btn.getGlobalBounds().contains(mPos)) {
                    isRunning = !isRunning;
                    btn.setFillColor(isRunning ? sf::Color(231, 76, 60) : sf::Color(46, 204, 113));
                }
            }
        }

        if (isRunning) {
            accumulator += clock.restart().asSeconds();
            if (accumulator >= 1.0f) {
                if (timeLeft > 0) {
                    timeLeft--;
                    for (auto& app : apps) killProcess(app);
                }
                accumulator = 0.0f;
            }
        } else { clock.restart(); }

        int m = timeLeft / 60;
        int s = timeLeft % 60;
        timerText.setString((m < 10 ? "0" : "") + to_string(m) + ":" + (s < 10 ? "0" : "") + to_string(s));
        
        sf::FloatRect textBounds = timerText.getLocalBounds();
        timerText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        timerText.setPosition(250.f, 285.f);

        window.clear(sf::Color(30, 39, 46));
        window.draw(ring);
        window.draw(timerText);
        window.draw(btn);
        window.display();
    }
    return 0;
}