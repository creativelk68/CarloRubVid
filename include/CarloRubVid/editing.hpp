#pragma once

#include <SFML/Graphics.hpp>

namespace CarloRubVid {
    
    /* Resizing an image by scaling it to the new size
    *
    *  Parameters:
    *  - img: The image which should be resized (passed by reference, so it will be directly edited)
    *  - new_size: The size of the resized image
    */
    void resize(sf::Image& img, const sf::Vector2u& new_size);

    /* Resizing an image by cutting everything off which is not insode the new bounds
    *
    * Parameters:
    * - img: The image which should be resized (passed by reference, so it will be directly edited)
    * - new_bounds: The bounds which will be applied for the edited image
    */
    void mov_bounds(sf::Image& img, const sf::FloatRect& new_bounds);

} // namespace CarloRubVid

    
