#include "CarloRubVid/tabbar.hpp"
#include "CarloRubVid/loop.hpp"

#include <iostream>

using namespace CarloRubVid;

// Constructors
Tabbar::Tabbar(const std::vector<sf::Image>& imgs, const sf::Font& font) {
    m_tabs.reserve(imgs.size());

    for (register uint32_t i = 0u; i < imgs.size(); i++) {
        m_tabs.emplace_back(
            imgs[i],
            Button(
                sf::Vector2i(100 * i + 50, 72),
                sf::Vector2u(100u, 100u),
                "",
                font,
                0u,
                sf::Color(0x22, 0x22, 0x22),
                sf::Color(0x33, 0x33, 0x33)
            )
        );
    }
}

Tabbar::Tab::Tab(const sf::Image& image, const Button&& button) : image(image), button(button) {}


// Methods

void Tabbar::update(CarloRubVid::PolledEventSet& events, uint32_t& image_idx) {

    if (events.mouse_moved) {

        for (register uint32_t i = 0u; i < m_tabs.size(); i++) {
            if (m_tabs[i].button.is_hovered(events.new_mouse_pos)) {
                m_tabs[i].button.set_hovered(true);
                continue;
            }

            m_tabs[i].button.set_hovered(false);
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        for (register uint32_t i = 0u; i < m_tabs.size(); i++) {
            if (m_tabs[i].button.is_hovered()) {
                image_idx = i;
            }
        }
    }
}

void Tabbar::draw(sf::RenderTarget& window, sf::RenderStates states) const {

    sf::RectangleShape bar(sf::Vector2f(1500.0f, 100.0f));
    bar.setPosition(.0f, 22.0f);
    bar.setFillColor(sf::Color(0x22, 0x22, 0x22));

    window.draw(bar, states);

    for (register uint32_t i = 0u; i < m_tabs.size(); i++) {

        window.draw(m_tabs[i].button);

        sf::Texture texture;
        texture.loadFromImage(m_tabs[i].image);

        sf::Sprite sprite(texture);
        sprite.setScale(100.0f / texture.getSize().x, 100.0f / texture.getSize().y);

        sf::RenderTexture rtexture;
        rtexture.create(100.0f, 100.0f);
        rtexture.draw(sprite);
        rtexture.display();

        sf::Image img = rtexture.getTexture().copyToImage();

        for (register uint32_t y = 0u; y < 100u; y++) {
            for (register uint32_t x = 0u; x < 100u; x++) {
                if (img.getPixel(x, y) == sf::Color::Transparent) {
                    if ((y % 2 == 0 && x % 2 == 0) || (y % 2 && x % 2)) img.setPixel(x, y, sf::Color(0x66, 0x66, 0x66));
                    else                                                img.setPixel(x, y, sf::Color::White);
                }
            }
        }

        texture.loadFromImage(img);
        sprite = sf::Sprite(texture);
        sprite.setPosition(100.0f * i, 22.0f);
        window.draw(sprite, states);
    }
}