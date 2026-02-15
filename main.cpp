#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

struct Task {
    std::string description;
    bool isDone;
};

class FocusApp {
public:
    sf::Font font;
    int totalFocusMins = 0;
    std::vector<Task> tasks;
    std::string currentInput;

    FocusApp() {
        // SFML 3.0 uses openFromFile
        if (!font.openFromFile("C:/Windows/Fonts/segoeui.ttf")) {}
        loadData();
    }

    void loadData() {
        std::ifstream file("focus_pro_data.txt");
        if (file.is_open()) {
            file >> totalFocusMins;
            std::string line;
            std::getline(file, line); 
            while (std::getline(file, line)) {
                if (!line.empty()) tasks.push_back({line, false});
            }
            file.close();
        }
    }

    void saveData() {
        std::ofstream file("focus_pro_data.txt");
        file << totalFocusMins << "\n";
        for (const auto& t : tasks) {
            if (!t.isDone) file << t.description << "\n";
        }
        file.close();
    }

    void addTask(std::string desc) {
        if (!desc.empty()) {
            tasks.push_back({desc, false});
            saveData();
        }
    }
};

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({550, 850}), "FocusPro - Apple Workspace", sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    FocusApp app;
    int timeLeft = 25 * 60;
    bool isRunning = false;
    sf::Clock clock;

    sf::CircleShape ring(130.f);
    ring.setOutlineThickness(8.f);
    ring.setOutlineColor(sf::Color(60, 60, 60));
    ring.setFillColor(sf::Color::Transparent);
    ring.setOrigin({130.f, 130.f});
    ring.setPosition({275.f, 180.f});

    sf::Text timerText(app.font);
    timerText.setCharacterSize(70);
    timerText.setFillColor(sf::Color::White);

    sf::RectangleShape startBtn({200.f, 50.f});
    startBtn.setFillColor(sf::Color(0, 122, 255));
    startBtn.setOrigin({100.f, 25.f});
    startBtn.setPosition({275.f, 340.f});

    sf::Text btnText(app.font, "", 20);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                app.saveData();
                window.close();
            }

            if (const auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                if (textEvent->unicode < 128) {
                    if (textEvent->unicode == 13) { 
                        app.addTask(app.currentInput);
                        app.currentInput.clear();
                    } else if (textEvent->unicode == 8 && !app.currentInput.empty()) {
                        app.currentInput.pop_back();
                    } else if (textEvent->unicode >= 32) {
                        app.currentInput += static_cast<char>(textEvent->unicode);
                    }
                }
            }

            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mPos = window.mapPixelToCoords({mb->position.x, mb->position.y});
                if (startBtn.getGlobalBounds().contains(mPos)) {
                    isRunning = !isRunning;
                }
                
                // FIXED: SFML 3.0 Rect Constructor
                for (size_t i = 0; i < app.tasks.size(); i++) {
                    sf::FloatRect taskArea({50.f, 540.f + (float)i * 35.f}, {400.f, 30.f});
                    if (taskArea.contains(mPos)) {
                        app.tasks[i].isDone = true;
                        app.saveData();
                    }
                }
            }
        }

        if (isRunning) {
            static float accum = 0;
            accum += clock.restart().asSeconds();
            if (accum >= 1.0f) {
                if (timeLeft > 0) timeLeft--;
                else {
                    app.totalFocusMins += 25;
                    app.saveData();
                    timeLeft = 25 * 60;
                    isRunning = false;
                }
                accum = 0;
            }
        } else { clock.restart(); }

        window.clear(sf::Color(10, 10, 10));

        int m = timeLeft / 60, s = timeLeft % 60;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
        timerText.setString(ss.str());
        sf::FloatRect tb = timerText.getLocalBounds();
        timerText.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
        timerText.setPosition({275.f, 165.f});

        btnText.setString(isRunning ? "PAUSE" : "START");
        sf::FloatRect bb = btnText.getLocalBounds();
        btnText.setOrigin({bb.size.x / 2.f, bb.size.y / 2.f});
        btnText.setPosition(startBtn.getPosition());

        window.draw(ring);
        window.draw(timerText);
        window.draw(startBtn);
        window.draw(btnText);

        sf::RectangleShape dashCard({460.f, 80.f});
        dashCard.setFillColor(sf::Color(255, 255, 255, 15));
        dashCard.setPosition({45.f, 390.f});
        window.draw(dashCard);

        sf::Text dashText(app.font, "Total Focused: " + std::to_string(app.totalFocusMins) + " mins", 18);
        dashText.setPosition({60.f, 405.f});
        window.draw(dashText);

        sf::Text inputDisplay(app.font, "> " + app.currentInput + (isRunning ? "" : "_"), 20);
        inputDisplay.setFillColor(sf::Color(0, 122, 255));
        inputDisplay.setPosition({50.f, 500.f});
        window.draw(inputDisplay);

        for (size_t i = 0; i < app.tasks.size(); i++) {
            if (!app.tasks[i].isDone) {
                sf::Text t(app.font, "[ ] " + app.tasks[i].description, 18);
                t.setPosition({50.f, 540.f + (i * 35.f)});
                window.draw(t);
            }
        }
        window.display();
    }
    return 0;
}