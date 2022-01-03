#ifndef CARLO_RUB_VID_TABBAR
#define CARLO_RUB_VID_TABBAR


#include <SFML/Graphics.hpp>

#include <vector>

#include "CarloRubVid/button.hpp"


namespace CarloRubVid {

    // Defined in loop.h
    struct PolledEventSet;

    class Tabbar : public sf::Drawable {

        struct Tab {
            sf::Image image;
            Button button;

            Tab(const sf::Image&, const Button&&);

            Tab& operator=(const Tab& other) {
                this->image = other.image;
                return *this;
            }
        };
        
        std::vector<Tab> m_tabs;

    public:

        // Constructor
        Tabbar() = delete;
        Tabbar(const std::vector<sf::Image>&, const sf::Font&);


        // Methods

        void update(CarloRubVid::PolledEventSet& events, uint32_t& img_idx);

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    };
} // namespace CarloRubVid

#endif // #ifndef CARLORUB_TABBAR