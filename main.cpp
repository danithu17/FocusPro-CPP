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
    sf::RenderWindow window(sf::VideoMode({850, 650}), "FocusPro - Apple Desktop", sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    FocusApp app;
    int timeLeft = 25 * 60;
    bool isRunning = false;
    sf::Clock clock;

    sf::RectangleShape sidebar({250.f, 650.f});
    sidebar.setFillColor(sf::Color(30, 30, 30));

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
                
                // FIXED: SFML 3.0 Rect constructor for Sidebar Click Detection
                if (sf::FloatRect({20.f, 100.f}, {210.f, 40.f}).contains(mPos)) app.activeCategory = "Inbox";
                if (sf::FloatRect({20.f, 150.f}, {210.f, 40.f}).contains(mPos)) app.activeCategory = "Work";
                if (sf::FloatRect({20.f, 200.f}, {210.f, 40.f}).contains(mPos)) app.activeCategory = "Study";

                // FIXED: Start button rect
                if (sf::FloatRect({450.f, 320.f}, {200.f, 50.f}).contains(mPos)) isRunning = !isRunning;
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

        window.draw(sidebar);
        sf::Text sideTitle(app.font, "Focus-To-Do", 22);
        sideTitle.setPosition({30.f, 40.f});
        window.draw(sideTitle);

        std::vector<std::string> cats = {"Inbox", "Work", "Study", "Gym"};
        for(int i=0; i < (int)cats.size(); i++) {
            sf::Text catText(app.font, cats[i], 18);
            catText.setPosition({50.f, 100.f + (i * 50.f)});
            if(app.activeCategory == cats[i]) catText.setFillColor(sf::Color(0, 122, 255));
            window.draw(catText);
        }

        window.draw(ring);
        int m = timeLeft / 60, s = timeLeft % 60;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
        sf::Text timerText(app.font, ss.str(), 60);
        sf::FloatRect tb = timerText.getLocalBounds();
        timerText.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
        timerText.setPosition({550.f, 185.f});
        window.draw(timerText);

        sf::RectangleShape btn({160.f, 45.f});
        btn.setFillColor(sf::Color(0, 122, 255));
        btn.setPosition({470.f, 320.f});
        window.draw(btn);
        
        sf::Text bt(app.font, isRunning ? "PAUSE" : "START", 18);
        sf::FloatRect bbt = bt.getLocalBounds();
        bt.setPosition({550.f - bbt.size.x/2.f, 330.f});
        window.draw(bt);

        sf::RectangleShape card({500.f, 150.f});
        card.setFillColor(sf::Color(255, 255, 255, 10));
        card.setPosition({300.f, 450.f});
        window.draw(card);
        
        sf::Text stat(app.font, "Session: " + app.activeCategory + " | Total: " + std::to_string(app.totalFocusMins) + "m", 16);
        stat.setPosition({320.f, 470.f});
        window.draw(stat);

        window.display();
    }
    return 0;
}