#include "CarloRubVid/editing.hpp"

#include <SFML/Graphics.hpp>

void CarloRubVid::resize(sf::Image& img, const sf::Vector2u& new_size) {
    sf::Texture texture;
    texture.loadFromImage(img);

    sf::Sprite sprite(texture);
    sprite.setScale(new_size.x / img.getSize().x, new_size.y / img.getSize().y);

    sf::RenderTexture rtexture;
    rtexture.create(new_size.x, new_size.y);

    rtexture.clear(sf::Color::Transparent);
    rtexture.draw(sprite);
    rtexture.display();

    img = rtexture.getTexture().copyToImage();
}

void CarloRubVid::mov_bounds(sf::Image& img, const sf::FloatRect& new_bounds) {

    sf::Image new_img;
    new_img.create(new_bounds.width, new_bounds.height);

    for (register uint32_t y = new_bounds.top, new_y = 0u; y < new_bounds.height; y++, new_y++) {
        for (register uint32_t x = new_bounds.left, new_x = 0u; x < new_bounds.width; x++, new_x++) {
            new_img.setPixel(new_x, new_y, img.getPixel(x, y));
        }
    }

    img = new_img;
}