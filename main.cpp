#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode({500, 600}), "FocusPro v3.1", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) return -1;

    sf::SoundBuffer startBuffer;
    sf::Sound startSound;
    if (startBuffer.loadFromFile("start.wav")) {
        startSound.setBuffer(startBuffer);
    }

    int totalSeconds = 25 * 60;
    int timeLeft = totalSeconds;
    bool isRunning = false;
    sf::Clock clock;
    float accumulator = 0.0f;

    sf::CircleShape ring(150.f);
    ring.setPointCount(100);
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineThickness(10.f);
    ring.setOutlineColor(sf::Color(46, 204, 113));
    ring.setOrigin({150.f, 150.f});
    ring.setPosition({250.f, 300.f});

    sf::Text timerText(font);
    timerText.setCharacterSize(80);
    timerText.setFillColor(sf::Color::White);

    sf::RectangleShape btn(sf::Vector2f(220.f, 65.f));
    btn.setFillColor(sf::Color(46, 204, 113));
    btn.setOrigin({110.f, 32.5f});
    btn.setPosition({250.f, 520.f});

    sf::Text btnText(font);
    btnText.setCharacterSize(22);
    btnText.setFillColor(sf::Color::White);
    btnText.setString("START FOCUS");

    vector<string> apps = {"chrome.exe", "msedge.exe", "discord.exe"};

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            
            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (btn.getGlobalBounds().contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y))) {
                    isRunning = !isRunning;
                    if (isRunning) startSound.play();
                    btn.setFillColor(isRunning ? sf::Color(231, 76, 60) : sf::Color(46, 204, 113));
                    btnText.setString(isRunning ? "STOP SESSION" : "START FOCUS");
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
        } else {
            clock.restart();
        }

        int m = timeLeft / 60;
        int s = timeLeft % 60;
        timerText.setString((m < 10 ? "0" : "") + to_string(m) + ":" + (s < 10 ? "0" : "") + to_string(s));
        
        sf::FloatRect textBounds = timerText.getLocalBounds();
        timerText.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
        timerText.setPosition({250.f, 285.f});

        sf::FloatRect btnTextBounds = btnText.getLocalBounds();
        btnText.setOrigin({btnTextBounds.size.x / 2.0f, btnTextBounds.size.y / 2.0f});
        btnText.setPosition({250.f, 515.f});

        window.clear(sf::Color(30, 39, 46));
        window.draw(ring);
        window.draw(timerText);
        window.draw(btn);
        window.draw(btnText);
        window.display();
    }
    return 0;
}