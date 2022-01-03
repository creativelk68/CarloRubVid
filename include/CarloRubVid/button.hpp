#pragma once

#include <SFML/Graphics.hpp>

#include <cstdint>
#include <string>

namespace CarloRubVid {

    class Button : public sf::Drawable {

        // Data for the box
        bool m_is_hovered;
        sf::Vector2i m_pos;
        sf::Vector2u m_size;
        sf::Color m_col, m_col_hovered;

        // Data for the text
        uint32_t m_font_size;
        sf::Color m_text_col;
        std::string m_text;
        const sf::Font& m_font;

    public:

        Button(
            const sf::Vector2i& position = sf::Vector2i(), const sf::Vector2u& size = sf::Vector2u(), const std::string& text = std::string(),
            const sf::Font& font = sf::Font(), const uint32_t& font_size = 30u, const sf::Color& color_default = sf::Color::Red, const sf::Color& color_hovered = sf::Color::Blue,
            const sf::Color& text_color = sf::Color::White
        );

        inline void set_position(const sf::Vector2i& new_position)   { m_pos = new_position; }
        inline void set_position(const int32_t& x, const int32_t& y) { m_pos.x = x; m_pos.y = y; }

        inline void move(const sf::Vector2i& offset)         { m_pos += offset; }
        inline void move(const int32_t& x, const int32_t& y) { m_pos.x += x; m_pos.y += y; }

        inline void set_size(const sf::Vector2u& new_size) { m_size = new_size; }
        inline void set_size(const uint32_t& new_width, const uint32_t& new_height) { m_size.x = new_width; m_size.y = new_height; }

        inline void set_default_color(const sf::Color& new_default_color) { m_col = new_default_color; }
        
        inline void set_hovered_color(const sf::Color& new_hovered_color) { m_col_hovered = new_hovered_color; }

        inline void set_hovered(const bool& is_hovered) { m_is_hovered = is_hovered; }

        inline bool is_hovered() const { return m_is_hovered; }
        bool is_hovered(const sf::Vector2i& mouse_position) const;

        inline std::string get_text() const { return m_text; }

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    };
} // namespace Werner