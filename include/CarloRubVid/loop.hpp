#ifndef CARLO_RUB_VID_LOOP
#define CARLO_RUB_VID_LOOP


#include <SFML/Graphics.hpp>

#include "CarloRubVid/button.hpp"

#include "CarloRubVid/init.hpp"
#include "CarloRubVid/tabbar.hpp"
#include "CarloRubVid/optional_loop.hpp"


namespace CarloRubVid {

    struct PolledEventSet {
        bool mouse_moved;
        sf::Vector2i new_mouse_pos;
    };
    

    PolledEventSet poll_events(sf::RenderWindow&, CarloRubVid::GUIContainer&);

    void update(CarloRubVid::PolledEventSet&, CarloRubVid::GUIContainer&, CarloRubVid::OptionalGUIContainer&, CarloRubVid::OptionalWindow&);

    void draw(sf::RenderWindow& window, const CarloRubVid::GUIContainer& gui, const std::vector<sf::Image>& imgs);

} // namespace CarloRubVid

#endif // #ifndef CARLO_RUB_VID_LOOP