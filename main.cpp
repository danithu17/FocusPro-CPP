#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

class Dashboard {
public:
    sf::RectangleShape background;
    sf::Text totalFocusTime;
    sf::Text badgesText;

    Dashboard(sf::Font& font) {
        background.setSize(sf::Vector2f(440, 120));
        background.setFillColor(sf::Color(255, 255, 255, 40)); // Apple Glass Look
        background.setOutlineThickness(1);
        background.setOutlineColor(sf::Color(255, 255, 255, 80));
        background.setPosition(30, 430);

        totalFocusTime.setFont(font);
        totalFocusTime.setCharacterSize(18);
        totalFocusTime.setFillColor(sf::Color::White);
        totalFocusTime.setPosition(50, 450);
        totalFocusTime.setString("Total Focus: 00h 00m");

        badgesText.setFont(font);
        badgesText.setCharacterSize(18);
        badgesText.setFillColor(sf::Color(200, 200, 200));
        badgesText.setPosition(50, 485);
        badgesText.setString("Badges: 🏆 Newbie");
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(totalFocusTime);
        window.draw(badgesText);
    }
};

int main() {
    // Anti-aliasing for Apple smoothness
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(500, 650), "FocusPro - Apple Experience", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/segoeui.ttf")) return -1;

    Dashboard db(font);

    // Timer Logic
    int timeLeft = 25 * 60;
    bool isRunning = false;
    sf::Clock clock;

    // UI Elements (Minimalist Apple Style)
    sf::CircleShape timerRing(120.f);
    timerRing.setOutlineThickness(5);
    timerRing.setOutlineColor(sf::Color(100, 100, 100));
    timerRing.setFillColor(sf::Color::Transparent);
    timerRing.setOrigin(120, 120);
    timerRing.setPosition(250, 200);

    sf::Text timerDisplay;
    timerDisplay.setFont(font);
    timerDisplay.setCharacterSize(60);
    timerDisplay.setFillColor(sf::Color::White);

    // Modern Button
    sf::RectangleShape btn(sf::Vector2f(200, 50));
    btn.setCornersRadius(25); // මේක SFML 3 වල තියෙන්නේ, 2.6 නම් manually අඳින්න ඕනේ
    btn.setFillColor(sf::Color(0, 122, 255)); // Apple Blue
    btn.setPosition(150, 350);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (btn.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    isRunning = !isRunning;
                }
            }
        }

        if (isRunning) {
            float dt = clock.restart().asSeconds();
            static float accum = 0;
            accum += dt;
            if (accum >= 1.0f) {
                if (timeLeft > 0) timeLeft--;
                accum = 0;
            }
        } else { clock.restart(); }

        // Update UI
        int m = timeLeft / 60;
        int s = timeLeft % 60;
        timerDisplay.setString((m < 10 ? "0" : "") + std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s));
        timerDisplay.setOrigin(timerDisplay.getLocalBounds().width / 2, timerDisplay.getLocalBounds().height / 2);
        timerDisplay.setPosition(250, 185);

        window.clear(sf::Color(25, 25, 25)); // Dark Apple Theme
        window.draw(timerRing);
        window.draw(timerDisplay);
        window.draw(btn);
        db.draw(window);
        window.display();
    }

    return 0;
}