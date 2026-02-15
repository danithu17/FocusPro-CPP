#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

struct Goal {
    std::string text;
    bool completed;
};

enum class Profile { Coding, Study, Creative };

class FocusApp {
public:
    sf::Font font;
    Profile currentProfile = Profile::Coding;
    sf::Color targetColor = sf::Color(0, 122, 255); 
    sf::Color currentColor = sf::Color(0, 122, 255);
    std::vector<Goal> dailyGoals;

    FocusApp() {
        if (!font.openFromFile("C:/Windows/Fonts/segoeui.ttf")) {}
        // Default Goals කිහිපයක්
        dailyGoals.push_back({"Finish C++ Logic", true});
        dailyGoals.push_back({"Design Mica UI", false});
        dailyGoals.push_back({"Fix Build Errors", false});
    }

    void setProfile(Profile p) {
        currentProfile = p;
        if (p == Profile::Coding) targetColor = sf::Color(0, 122, 255);
        else if (p == Profile::Study) targetColor = sf::Color(255, 149, 0); 
        else if (p == Profile::Creative) targetColor = sf::Color(175, 82, 222);
    }

    void updateTheme() {
        float speed = 0.05f;
        auto lerp = [&](float start, float end) { return start + speed * (end - start); };
        currentColor.r = static_cast<std::uint8_t>(lerp(currentColor.r, targetColor.r));
        currentColor.g = static_cast<std::uint8_t>(lerp(currentColor.g, targetColor.g));
        currentColor.b = static_cast<std::uint8_t>(lerp(currentColor.b, targetColor.b));
    }
};

int main() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({1000, 750}), "FocusPro - Apple Workspace", sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    FocusApp app;
    int timeLeft = 25 * 60;
    bool isRunning = false;
    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mPos = window.mapPixelToCoords({mb->position.x, mb->position.y});
                
                // Sidebar Selection
                if (sf::FloatRect({20.f, 100.f}, {210.f, 40.f}).contains(mPos)) app.setProfile(Profile::Coding);
                if (sf::FloatRect({20.f, 150.f}, {210.f, 40.f}).contains(mPos)) app.setProfile(Profile::Study);
                if (sf::FloatRect({20.f, 200.f}, {210.f, 40.f}).contains(mPos)) app.setProfile(Profile::Creative);

                // Start/Pause Button
                if (sf::FloatRect({410.f, 370.f}, {180.f, 50.f}).contains(mPos)) isRunning = !isRunning;
            }
        }

        app.updateTheme();

        if (isRunning) {
            static float accum = 0;
            accum += clock.restart().asSeconds();
            if (accum >= 1.0f) {
                if (timeLeft > 0) timeLeft--;
                accum = 0;
            }
        } else { clock.restart(); }

        window.clear(sf::Color(10, 10, 10));

        // Background Mica Gradient
        sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
        bg[0].position = {0, 0}; bg[0].color = sf::Color(25, 25, 30);
        bg[1].position = {1000, 0}; bg[1].color = sf::Color(10, 10, 10);
        bg[2].position = {0, 750}; bg[2].color = sf::Color(15, 15, 15);
        bg[3].position = {1000, 750}; bg[3].color = sf::Color(static_cast<std::uint8_t>(app.currentColor.r / 15), static_cast<std::uint8_t>(app.currentColor.g / 15), static_cast<std::uint8_t>(app.currentColor.b / 15));
        window.draw(bg);

        // Sidebar Panel
        sf::RectangleShape sidebar({250.f, 750.f});
        sidebar.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(sidebar);

        sf::Text title(app.font, "FOCUS PRO", 22);
        title.setPosition({40.f, 40.f});
        window.draw(title);

        std::vector<std::string> modes = {"Coding Mode", "Study Mode", "Creative Mode"};
        for (int i = 0; i < 3; i++) {
            sf::Text t(app.font, modes[i], 16);
            t.setPosition({50.f, 110.f + (static_cast<float>(i) * 50.f)});
            if (static_cast<int>(app.currentProfile) == i) t.setFillColor(app.currentColor);
            window.draw(t);
        }

        // Daily Goals Card (Right side)
        sf::RectangleShape card({300.f, 400.f});
        card.setFillColor(sf::Color(255, 255, 255, 10));
        card.setPosition({660.f, 100.f});
        window.draw(card);

        sf::Text goalTitle(app.font, "DAILY GOALS", 14);
        goalTitle.setFillColor(sf::Color(150, 150, 150));
        goalTitle.setPosition({680.f, 120.f});
        window.draw(goalTitle);

        for (size_t i = 0; i < app.dailyGoals.size(); i++) {
            sf::Text t(app.font, (app.dailyGoals[i].completed ? "[x] " : "[ ] ") + app.dailyGoals[i].text, 16);
            t.setPosition({680.f, 160.f + (static_cast<float>(i) * 40.f)});
            if (app.dailyGoals[i].completed) t.setFillColor(sf::Color(100, 100, 100));
            window.draw(t);
        }

        // Main Timer Ring (Center-ish)
        sf::CircleShape ring(130.f);
        ring.setOutlineThickness(3.f);
        ring.setOutlineColor(app.currentColor);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOrigin({130.f, 130.f});
        ring.setPosition({500.f, 220.f});
        window.draw(ring);

        int m = timeLeft / 60, s = timeLeft % 60;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
        sf::Text timerText(app.font, ss.str(), 80);
        sf::FloatRect tb = timerText.getLocalBounds();
        timerText.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
        timerText.setPosition({500.f, 210.f});
        window.draw(timerText);

        // Control Button
        sf::RectangleShape btn({180.f, 50.f});
        btn.setFillColor(app.currentColor);
        btn.setPosition({410.f, 370.f});
        window.draw(btn);

        sf::Text btTxt(app.font, isRunning ? "PAUSE" : "START", 18);
        sf::FloatRect btb = btTxt.getLocalBounds();
        btTxt.setPosition({500.f - btb.size.x / 2.f, 382.f});
        window.draw(btTxt);

        window.display();
    }
    return 0;
}