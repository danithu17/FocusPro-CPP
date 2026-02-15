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
    std::string activeCategory = "Work";
    std::string currentTrack = "Rainy Forest";

    FocusApp() {
        // SFML 3.0 uses openFromFile returning bool
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
};

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({900, 700}), "FocusPro - Apple Workspace", sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    FocusApp app;
    int timeLeft = 25 * 60;
    int initialTime = 25 * 60;
    bool isRunning = false;
    sf::Clock clock;

    // Sidebar Background
    sf::RectangleShape sidebar({260.f, 700.f});
    sidebar.setFillColor(sf::Color(25, 25, 25));

    // Player Card Background
    sf::RectangleShape playerCard({220.f, 100.f});
    playerCard.setFillColor(sf::Color(255, 255, 255, 15));
    playerCard.setPosition({20.f, 560.f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                app.saveData();
                window.close();
            }

            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mPos = window.mapPixelToCoords({mb->position.x, mb->position.y});
                
                // Sidebar Selection
                if (sf::FloatRect({20.f, 80.f}, {220.f, 40.f}).contains(mPos)) app.activeCategory = "Deep Work";
                if (sf::FloatRect({20.f, 130.f}, {220.f, 40.f}).contains(mPos)) app.activeCategory = "Study";
                
                // Track Selection
                if (sf::FloatRect({20.f, 560.f}, {220.f, 50.f}).contains(mPos)) app.currentTrack = "Lo-Fi Beats";
                if (sf::FloatRect({20.f, 610.f}, {220.f, 50.f}).contains(mPos)) app.currentTrack = "Rainy Forest";

                // Start Button Click
                if (sf::FloatRect({490.f, 350.f}, {180.f, 50.f}).contains(mPos)) isRunning = !isRunning;
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

        window.clear(sf::Color(15, 15, 15));

        // 1. Sidebar & Player
        window.draw(sidebar);
        sf::Text sideTitle(app.font, "FOCUS PRO", 24);
        sideTitle.setPosition({30.f, 30.f});
        window.draw(sideTitle);

        window.draw(playerCard);
        sf::Text nowPlaying(app.font, "NOW PLAYING", 12);
        nowPlaying.setFillColor(sf::Color(150, 150, 150));
        nowPlaying.setPosition({35.f, 570.f});
        window.draw(nowPlaying);

        sf::Text trackName(app.font, app.currentTrack, 16);
        trackName.setPosition({35.f, 590.f});
        window.draw(trackName);

        // 2. Timer Circle
        sf::CircleShape ring(120.f);
        ring.setOutlineThickness(5.f);
        ring.setOutlineColor(sf::Color(40, 40, 40));
        ring.setFillColor(sf::Color::Transparent);
        ring.setOrigin({120.f, 120.f});
        ring.setPosition({580.f, 220.f});
        window.draw(ring);

        // 3. Timer Text
        int m = timeLeft / 60, s = timeLeft % 60;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
        sf::Text timerText(app.font, ss.str(), 75);
        sf::FloatRect tb = timerText.getLocalBounds();
        timerText.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
        timerText.setPosition({580.f, 205.f});
        window.draw(timerText);

        // 4. Progress Bar
        float progressWidth = 400.f * (1.0f - (float)timeLeft / initialTime);
        sf::RectangleShape progBar({progressWidth, 4.f});
        progBar.setFillColor(sf::Color(0, 122, 255));
        progBar.setPosition({380.f, 450.f});
        window.draw(progBar);

        // 5. Start Button (FIXED LINE)
        sf::RectangleShape btn({180.f, 50.f}); // සාමාන්‍ය Rect එකක් දැන්
        btn.setFillColor(isRunning ? sf::Color(255, 50, 50) : sf::Color(0, 122, 255));
        btn.setPosition({490.f, 350.f});
        window.draw(btn);
        
        sf::Text btText(app.font, isRunning ? "PAUSE" : "START", 18);
        sf::FloatRect bbt = btText.getLocalBounds();
        btText.setPosition({580.f - bbt.size.x/2.f, 362.f});
        window.draw(btText);

        // 6. Quote
        sf::Text quote(app.font, "\"Stay focused, be present.\"", 18);
        quote.setFillColor(sf::Color(100, 100, 100));
        quote.setPosition({450.f, 600.f});
        window.draw(quote);

        window.display();
    }
    return 0;
}