#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>

#include "CarloRubVid/init.hpp"

namespace CarloRubVid {

    struct OptionalWindow {
    
        uint32_t type_idx;
        sf::RenderWindow* window;

        OptionalWindow() : type_idx(0u), window(nullptr) {}

        inline ~OptionalWindow() {
            if (this->window == nullptr) return;

            this->window->close();
            delete this->window;
        }

        static void poll_events(OptionalWindow&);

        static bool update(OptionalWindow&, GUIContainer&, OptionalGUIContainer&, std::vector<sf::Image>&, std::vector<std::string>&);

        static void draw(OptionalWindow&, const OptionalGUIContainer&);
    };

} // namespace CarloRubVid
