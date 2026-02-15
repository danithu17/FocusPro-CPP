#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

enum class Profile { Coding, Study, Creative };

class FocusApp {
public:
    sf::Font font;
    Profile currentProfile = Profile::Coding;
    sf::Color targetColor = sf::Color(0, 122, 255); // Apple Blue
    sf::Color currentColor = sf::Color(0, 122, 255);

    FocusApp() {
        // Windows default font එක load කිරීම
        font.openFromFile("C:/Windows/Fonts/segoeui.ttf");
    }

    void setProfile(Profile p) {
        currentProfile = p;
        if (p == Profile::Coding) targetColor = sf::Color(0, 122, 255);
        else if (p == Profile::Study) targetColor = sf::Color(255, 149, 0); // Orange
        else if (p == Profile::Creative) targetColor = sf::Color(175, 82, 222); // Purple
    }

    // Smooth color animation logic (Lerp)
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
                
                // Profile selection (Sidebar logic)
                if (sf::FloatRect({20.f, 100.f}, {220.f, 40.f}).contains(mPos)) app.setProfile(Profile::Coding);
                if (sf::FloatRect({20.f, 150.f}, {220.f, 40.f}).contains(mPos)) app.setProfile(Profile::Study);
                if (sf::FloatRect({20.f, 200.f}, {220.f, 40.f}).contains(mPos)) app.setProfile(Profile::Creative);

                // Start/Pause button logic
                if (sf::FloatRect({540.f, 370.f}, {180.f, 50.f}).contains(mPos)) isRunning = !isRunning;
            }
        }

        app.updateTheme(); // Animate colors

        if (isRunning) {
            static float accum = 0;
            accum += clock.restart().asSeconds();
            if (accum >= 1.0f) {
                if (timeLeft > 0) timeLeft--;
                accum = 0;
            }
        } else { clock.restart(); }

        window.clear(sf::Color(10, 10, 10));

        // 1. MICA/GLASS BACKGROUND
        sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
        bg[0].position = {0, 0}; bg[0].color = sf::Color(20, 20, 25);
        bg[1].position = {1000, 0}; bg[1].color = sf::Color(10, 10, 10);
        bg[2].position = {0, 750}; bg[2].color = sf::Color(15, 15, 15);
        bg[3].position = {1000, 750}; bg[3].color = sf::Color(app.currentColor.r / 12, app.currentColor.g / 12, app.currentColor.b / 12);
        window.draw(bg);

        // 2. SIDEBAR
        sf::RectangleShape sidebar({250.f, 750.f});
        sidebar.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(sidebar);

        sf::Text title(app.font, "FOCUS PRO", 22);
        title.setPosition({40.f, 40.f});
        window.draw(title);

        // Sidebar Menu
        std::vector<std::string> modes = {"Coding Mode", "Study Mode", "Creative Mode"};
        for (int i = 0; i < 3; i++) {
            sf::Text t(app.font, modes[i], 16);
            t.setPosition({50.f, 110.f + (i * 50.f)});
            if (static_cast<int>(app.currentProfile) == i) t.setFillColor(app.currentColor);
            window.draw(t);
        }

        // 3. MAIN TIMER RING
        sf::CircleShape ring(140.f);
        ring.setOutlineThickness(3.f);
        ring.setOutlineColor(app.currentColor);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOrigin({140.f, 140.f});
        ring.setPosition({630.f, 220.f});
        window.draw(ring);

        // TIMER TEXT
        int m = timeLeft / 60, s = timeLeft % 60;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
        sf::Text timerText(app.font, ss.str(), 85);
        sf::FloatRect bounds = timerText.getLocalBounds();
        timerText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        timerText.setPosition({630.f, 210.f});
        window.draw(timerText);

        // 4. START BUTTON (Apple Glass Style)
        sf::RectangleShape btn({180.f, 50.f});
        btn.setFillColor(app.currentColor);
        btn.setPosition({540.f, 370.f});
        window.draw(btn);

        sf::Text btText(app.font, isRunning ? "PAUSE" : "START", 18);
        sf::FloatRect btBounds = btText.getLocalBounds();
        btText.setPosition({630.f - btBounds.size.x / 2.f, 382.f});
        window.draw(btText);

        window.display();
    }
    return 0;
}