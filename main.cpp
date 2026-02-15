#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

struct Task {
    std::string description;
    std::string category;
    bool isDone;
};

class FocusApp {
public:
    sf::Font font;
    int totalFocusMins = 0;
    std::vector<Task> tasks;
    std::string currentInput;
    std::string activeCategory = "Work";

    FocusApp() {
        font.openFromFile("C:/Windows/Fonts/segoeui.ttf");
        loadData();
    }

    void loadData() {
        std::ifstream file("focus_pro_data.txt");
        if (file.is_open()) {
            file >> totalFocusMins;
            std::string line;
            std::getline(file, line); 
            while (std::getline(file, line)) {
                if (!line.empty()) tasks.push_back({line, "Work", false});
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
};

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    // පොඩ්ඩක් පළල වැඩි කළා Sidebar එක දාන්න
    sf::RenderWindow window(sf::VideoMode({850, 650}), "FocusPro - Apple Desktop", sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    FocusApp app;
    int timeLeft = 25 * 60;
    bool isRunning = false;
    sf::Clock clock;

    // Sidebar Background
    sf::RectangleShape sidebar({250.f, 650.f});
    sidebar.setFillColor(sf::Color(30, 30, 30));

    // Timer Circle
    sf::CircleShape ring(100.f);
    ring.setOutlineThickness(6.f);
    ring.setOutlineColor(sf::Color(0, 122, 255));
    ring.setFillColor(sf::Color::Transparent);
    ring.setOrigin({100.f, 100.f});
    ring.setPosition({550.f, 200.f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                app.saveData();
                window.close();
            }

            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mPos = window.mapPixelToCoords({mb->position.x, mb->position.y});
                
                // Sidebar category selection
                if (sf::FloatRect({20, 100, 210, 40}).contains(mPos)) app.activeCategory = "Inbox";
                if (sf::FloatRect({20, 150, 210, 40}).contains(mPos)) app.activeCategory = "Work";
                if (sf::FloatRect({20, 200, 210, 40}).contains(mPos)) app.activeCategory = "Study";

                // Start button logic
                if (sf::FloatRect({450, 320, 200, 50}).contains(mPos)) isRunning = !isRunning;
            }
        }

        if (isRunning) {
            static float accum = 0;
            accum += clock.restart().asSeconds();
            if (accum >= 1.0f) {
                if (timeLeft > 0) timeLeft--;
                else { app.totalFocusMins += 25; app.saveData(); timeLeft = 25 * 60; isRunning = false; }
                accum = 0;
            }
        } else { clock.restart(); }

        window.clear(sf::Color(18, 18, 18));

        // Draw Sidebar
        window.draw(sidebar);
        sf::Text sideTitle(app.font, "Focus-To-Do", 22);
        sideTitle.setPosition({30, 40});
        window.draw(sideTitle);

        std::vector<std::string> cats = {"Inbox", "Work", "Study", "Gym"};
        for(int i=0; i<cats.size(); i++) {
            sf::Text catText(app.font, cats[i], 18);
            catText.setPosition({50, 100.f + (i * 50.f)});
            if(app.activeCategory == cats[i]) catText.setFillColor(sf::Color(0, 122, 255));
            window.draw(catText);
        }

        // Draw Main Area (Timer)
        window.draw(ring);
        int m = timeLeft / 60, s = timeLeft % 60;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
        sf::Text timerText(app.font, ss.str(), 60);
        timerText.setOrigin({timerText.getLocalBounds().size.x/2, timerText.getLocalBounds().size.y/2});
        timerText.setPosition({550.f, 185.f});
        window.draw(timerText);

        // Start/Pause Button
        sf::RectangleShape btn({160, 45});
        btn.setFillColor(sf::Color(0, 122, 255));
        btn.setPosition({470, 320});
        window.draw(btn);
        sf::Text bt(app.font, isRunning ? "PAUSE" : "START", 18);
        bt.setPosition({515, 330});
        window.draw(bt);

        // Task Summary Card
        sf::RectangleShape card({500, 150});
        card.setFillColor(sf::Color(255, 255, 255, 10));
        card.setPosition({300, 450});
        window.draw(card);
        sf::Text stat(app.font, "Session: " + app.activeCategory + " | Total: " + std::to_string(app.totalFocusMins) + "m", 16);
        stat.setPosition({320, 470});
        window.draw(stat);

        window.display();
    }
    return 0;
}