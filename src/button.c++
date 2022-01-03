#include "CarloRubVid/button.hpp"

CarloRubVid::Button::Button
(
    const sf::Vector2i& position, const sf::Vector2u& size, const std::string& text, const sf::Font& font, const uint32_t& font_size, const sf::Color& color_default,
    const sf::Color& color_hovered, const sf::Color& text_color
) : m_pos(position), m_size(size), m_text(text), m_font(font), m_font_size(font_size), m_col(color_default), m_col_hovered(color_hovered), m_text_col(text_color) {}


bool CarloRubVid::Button::is_hovered(const sf::Vector2i& mouse_position) const
{
    sf::RectangleShape shape(static_cast<sf::Vector2f>(m_size));
    shape.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
    shape.setPosition(static_cast<sf::Vector2f>(m_pos));
    
    const sf::FloatRect bounds = shape.getGlobalBounds();

    return bounds.contains(static_cast<sf::Vector2f>(mouse_position));
}


void CarloRubVid::Button::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    sf::RectangleShape button(static_cast<sf::Vector2f>(m_size));
    button.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
    button.setPosition(static_cast<sf::Vector2f>(m_pos));

    if (m_is_hovered) button.setFillColor(m_col_hovered);
    else              button.setFillColor(m_col);
    
    window.draw(button, states);

    sf::Text txt(m_text, m_font, m_font_size);
    txt.setFillColor(m_text_col);
    txt.setPosition(m_pos.x - txt.getLocalBounds().width / 2.0f, m_pos.y - txt.getLocalBounds().height / 2.0f);
    window.draw(txt, states);
}