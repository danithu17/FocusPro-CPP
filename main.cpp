#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>

enum class Profile { Coding, Study, Creative };

struct Task {
    std::string description;
    bool isDone;
};

class FocusApp {
public:
    sf::Font font;
    std::vector<Task> dailyGoals;
    Profile currentProfile = Profile::Coding;
    
    // Animation variables
    sf::Color targetColor = sf::Color(0, 122, 255);
    sf::Color currentColor = sf::Color(0, 122, 255);

    FocusApp() {
        if (!font.openFromFile("C:/Windows/Fonts/segoeui.ttf")) {}
        dailyGoals.push_back({"Finish C++ Project", false});
        dailyGoals.push_back({"Study UI/UX Design", false});
        dailyGoals.push_back({"Fix Build Errors", false});
    }

    void switchProfile(Profile p) {
        currentProfile = p;
        if (p == Profile::Coding) targetColor = sf::Color(0, 122, 255);
        if (p == Profile::Study) targetColor = sf::Color(255, 149, 0);
        if (p == Profile::Creative) targetColor = sf::Color(175, 82, 222);
    }

    // Smoothly interpolate between colors
    void updateAnimation() {
        auto lerp = [](float start, float end, float t) { return start + t * (end - start); };
        float speed = 0.05f;
        currentColor.r = (sf::Uint8)lerp(currentColor.r, targetColor.r, speed);
        currentColor.g = (sf::Uint8)lerp(currentColor.g, targetColor.g, speed);
        currentColor.b = (sf::Uint8)lerp(currentColor.b, targetColor.b, speed);
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
                
                // Sidebar Selection (Smooth profile switch)
                if (sf::FloatRect({20.f, 100.f}, {210.f, 40.f}).contains(mPos)) app.switchProfile(Profile::Coding);
                if (sf::FloatRect({20.f, 150.f}, {210.f, 40.f}).contains(mPos)) app.switchProfile(Profile::Study);
                if (sf::FloatRect({20.f, 200.f}, {210.f, 40.f}).contains(mPos)) app.switchProfile(Profile::Creative);

                if (sf::FloatRect({540.f, 370.f}, {180.f, 50.f}).contains(mPos)) isRunning = !isRunning;
            }
        }

        app.updateAnimation(); // Run smooth color logic

        if (isRunning) {
            static float accum = 0;
            accum += clock.restart().asSeconds();
            if (accum >= 1.0f) {
                if (timeLeft > 0) timeLeft--;
                accum = 0;
            }
        } else { clock.restart(); }

        window.clear(sf::Color(10, 10, 10));

        // 1. MICA BACKGROUND EFFECT
        sf::VertexArray gradient(sf::PrimitiveType::TriangleStrip, 4);
        gradient[0].position = {0, 0}; gradient[0].color = sf::Color(25, 25, 30);
        gradient[1].position = {1000, 0}; gradient[1].color = sf::Color(10, 10, 10);
        gradient[2].position = {0, 750}; gradient[2].color = sf::Color(15, 15, 15);
        gradient[3].position = {1000, 750}; gradient[3].color = sf::Color(app.currentColor.r/10, app.currentColor.g/10, app.currentColor.b/10);
        window.draw(gradient);

        // 2. SIDEBAR
        sf::RectangleShape sidebar({250.f, 750.f});
        sidebar.setFillColor(sf::Color(0, 0, 0, 160));
        window.draw(sidebar);

        // Sidebar Content
        sf::Text title(app.font, "FOCUS PRO", 24);
        title.setPosition({35.f, 40.f});
        window.draw(title);

        std::vector<std::string> cats = {"Coding", "Study", "Creative"};
        for(int i=0; i<3; i++) {
            sf::Text t(app.font, cats[i], 18);
            t.setPosition({50.f, 110.f + (i * 50.f)});
            if(app.currentProfile == (Profile)i) t.setFillColor(app.currentColor);
            window.draw(t);
        }

        // 3. MAIN TIMER RING (Smoothly changing color)
        sf::CircleShape ring(130.f);
        ring.setOutlineThickness(4.f);
        ring.setOutlineColor(app.currentColor);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOrigin({130.f, 130.f});
        ring.setPosition({630.f, 220.f});
        window.draw(ring);

        int m = timeLeft / 60, s = timeLeft % 60;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
        sf::Text timerText(app.font, ss.str(), 85);
        sf::FloatRect tb = timerText.getLocalBounds();
        timerText.setOrigin({tb.size.x/2.f, tb.size.y/2.f});
        timerText.setPosition({630.f, 210.f});
        window.draw(timerText);

        // 4. GLASS CARD (Goals)
        sf::RectangleShape card({380.f, 220.f});
        card.setFillColor(sf::Color(255, 255, 255, 12));
        card.setPosition({440.f, 480.f});
        window.draw(card);

        sf::Text goalHeader(app.font, "DAILY GOALS", 14);
        goalHeader.setFillColor(sf::Color(180, 180, 180));
        goalHeader.setPosition({460.f, 495.f});
        window.draw(goalHeader);

        for(int i=0; i<app.dailyGoals.size(); i++) {
            sf::Text t(app.font, "[ ] " + app.dailyGoals[i].description, 17);
            t.setPosition({470.f, 530.f + (i * 40.f)});
            window.draw(t);
        }

        // 5. START BUTTON
        sf::RectangleShape btn({180.f, 50.f});
        btn.setFillColor(app.currentColor);
        btn.setPosition({540.f, 370.f});
        window.draw(btn);

        sf::Text btTxt(app.font, isRunning ? "PAUSE" : "START", 18);
        sf::FloatRect bbt = btTxt.getLocalBounds();
        btTxt.setPosition({630.f - bbt.size.x/2.f, 382.f});
        window.draw(btTxt);

        window.display();
    }
    return 0;
}