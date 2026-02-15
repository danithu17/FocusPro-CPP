#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

struct Goal {
    std::string text;
    bool isDone;
};

enum class Profile { Coding, Study, Creative };

class FocusApp {
public:
    sf::Font font;
    Profile currentProfile = Profile::Coding;
    sf::Color targetColor{0, 122, 255}; 
    sf::Color currentColor{0, 122, 255};
    std::vector<Goal> goals;

    FocusApp() {
        if (!font.openFromFile("C:/Windows/Fonts/segoeui.ttf")) {}
        goals.push_back({"Fix Build System", true});
        goals.push_back({"Implement Daily Goals", false});
        goals.push_back({"Add Lofi Sounds", false});
    }

    void updateTheme(float dt) {
        float speed = 5.0f * dt;
        auto lerp = [&](std::uint8_t start, std::uint8_t end) {
            return static_cast<std::uint8_t>(start + speed * (end - start));
        };
        currentColor.r = lerp(currentColor.r, targetColor.r);
        currentColor.g = lerp(currentColor.g, targetColor.g);
        currentColor.b = lerp(currentColor.b, targetColor.b);
    }

    void setProfile(Profile p) {
        currentProfile = p;
        if (p == Profile::Coding) targetColor = sf::Color{0, 122, 255};
        else if (p == Profile::Study) targetColor = sf::Color{255, 149, 0}; 
        else if (p == Profile::Creative) targetColor = sf::Color{175, 82, 222};
    }
};

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({1000, 750}), "FocusPro Premium", sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    FocusApp app;
    sf::Clock deltaClock;
    int timeLeft = 25 * 60;
    bool isRunning = false;
    sf::Clock timerClock;

    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mPos = window.mapPixelToCoords({mb->position.x, mb->position.y});
                // Sidebar logic
                if (sf::FloatRect({20.f, 100.f}, {210.f, 40.f}).contains(mPos)) app.setProfile(Profile::Coding);
                if (sf::FloatRect({20.f, 150.f}, {210.f, 40.f}).contains(mPos)) app.setProfile(Profile::Study);
                if (sf::FloatRect({20.f, 200.f}, {210.f, 40.f}).contains(mPos)) app.setProfile(Profile::Creative);
                // Timer toggle
                if (sf::FloatRect({410.f, 370.f}, {180.f, 50.f}).contains(mPos)) isRunning = !isRunning;
            }
        }

        app.updateTheme(dt);
        if (isRunning && timerClock.getElapsedTime().asSeconds() >= 1.f) {
            if (timeLeft > 0) timeLeft--;
            timerClock.restart();
        }

        window.clear(sf::Color{10, 10, 10});

        // 1. Mica Background
        sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
        bg[0].position = {0, 0}; bg[0].color = sf::Color{25, 25, 30};
        bg[1].position = {1000, 0}; bg[1].color = sf::Color{10, 10, 10};
        bg[2].position = {0, 750}; bg[2].color = sf::Color{15, 15, 15};
        bg[3].position = {1000, 750}; bg[3].color = sf::Color(app.currentColor.r/15, app.currentColor.g/15, app.currentColor.b/15);
        window.draw(bg);

        // 2. Sidebar
        sf::RectangleShape sidebar({250.f, 750.f});
        sidebar.setFillColor(sf::Color{0, 0, 0, 180});
        window.draw(sidebar);
        sf::Text title(app.font, "FOCUS PRO", 22);
        title.setPosition({40.f, 40.f});
        window.draw(title);

        // 3. Goals Card (Glassmorphism)
        sf::RectangleShape card({320.f, 450.f});
        card.setFillColor(sf::Color{255, 255, 255, 12});
        card.setPosition({640.f, 100.f});
        window.draw(card);
        sf::Text gTitle(app.font, "DAILY GOALS", 14);
        gTitle.setFillColor(sf::Color{180, 180, 180});
        gTitle.setPosition({660.f, 120.f});
        window.draw(gTitle);

        for (size_t i = 0; i < app.goals.size(); i++) {
            sf::Text t(app.font, (app.goals[i].isDone ? "[x] " : "[ ] ") + app.goals[i].text, 16);
            t.setPosition({670.f, 160.f + (i * 40.f)});
            if (app.goals[i].isDone) t.setFillColor(sf::Color{100, 100, 100});
            window.draw(t);
        }

        // 4. Timer Ring
        sf::CircleShape ring(130.f);
        ring.setOutlineThickness(3.f);
        ring.setOutlineColor(app.currentColor);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOrigin({130.f, 130.f});
        ring.setPosition({440.f, 220.f});
        window.draw(ring);

        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << timeLeft/60 << ":" << std::setw(2) << std::setfill('0') << timeLeft%60;
        sf::Text timerText(app.font, ss.str(), 80);
        sf::FloatRect tb = timerText.getLocalBounds();
        timerText.setOrigin({tb.size.x/2.f, tb.size.y/2.f});
        timerText.setPosition({440.f, 210.f});
        window.draw(timerText);

        window.display();
    }
    return 0;
}