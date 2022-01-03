#include "CarloRubVid/init.hpp"
#include "CarloRubVid/loader.hpp"

#include <tuple>
#include <string>
#include <vector>
#include <fstream>

std::vector<std::string> CarloRubVid::parse_args(const int32_t& argc, char** argv) {

    std::vector<std::string> loaded_args;

    for (register uint32_t i = 1u; i < argc; i++) {
        loaded_args.push_back(std::string("../images_in/") + argv[i]);
    }

    return loaded_args;
}

static sf::Image render_bg_image() {

    sf::RenderTexture rtexture_bg;
    constexpr float a = 700.0f;
    rtexture_bg.create(a, a);
    rtexture_bg.clear();

    for (register uint32_t y = 0u; y < 100; y++) {
        for (register uint32_t x = 0u; x < 100; x++) {
            sf::RectangleShape square(sf::Vector2f(a / 100.0f, a / 100.0f));
            square.setPosition(x * (a / 100.0f), y * (a / 100.0f));
            
            if ((y % 2 == 0 && x % 2 == 0) || (y % 2 && x % 2)) { square.setFillColor(sf::Color(0x66, 0x66, 0x66)); }

            rtexture_bg.draw(square);
        }
    }

    rtexture_bg.display();

    return rtexture_bg.getTexture().copyToImage();
}

static sf::Image render_coordinate_system(const sf::Font& font) {

    sf::RectangleShape x_axis(sf::Vector2f(735.0f, 3.0f));
    x_axis.setFillColor(sf::Color(0x44, 0x44, 0x44));
    x_axis.setPosition(59.0f, 30.0f);

    sf::RectangleShape y_axis(sf::Vector2f(3.0f, 721.0f));
    y_axis.setFillColor(sf::Color(0x44, 0x44, 0x44));
    y_axis.setPosition(59.0f, 30.0f);

    std::array<sf::Text, 22u> marks = {
        sf::Text("0%",   font, 18u),  sf::Text("10%",  font, 18u),
        sf::Text("20%",  font, 18u),  sf::Text("30%",  font, 18u),
        sf::Text("40%",  font, 18u),  sf::Text("50%",  font, 18u),
        sf::Text("60%",  font, 18u),  sf::Text("70%",  font, 18u),
        sf::Text("80%",  font, 18u),  sf::Text("90%",  font, 18u),
        sf::Text("100%", font, 18u),  sf::Text("0%",   font, 18u),
        sf::Text("10%",  font, 18u),  sf::Text("20%",  font, 18u),
        sf::Text("30%",  font, 18u),  sf::Text("40%",  font, 18u),
        sf::Text("50%",  font, 18u),  sf::Text("60%",  font, 18u),
        sf::Text("70%",  font, 18u),  sf::Text("80%",  font, 18u),
        sf::Text("90%",  font, 18u),  sf::Text("100%", font, 18u),
    };

    std::array<float, 22u> positions_marks = {
        85.0f, 155.0f, 225.0f, 295.0f, 365.0f, 435.0f, 505.0f, 575.0f, 645.0f, 715.0f, 785.0f,
        41.0f, 111.0f, 181.0f, 251.0f, 321.0f, 391.0f, 461.0f, 531.0f, 601.0f, 671.0f, 741.0f
    };

    for (register uint32_t i = 0u; i < marks.size(); i++) {
        if (i < positions_marks.size() / 2u) { marks[i].setPosition(positions_marks[i], .0f); }
        else                                 { marks[i].setPosition(.0f, positions_marks[i]); }
    }

    std::array<sf::Vertex, 22u> marks_x = {
        sf::Vertex(sf::Vector2f(94.0f,  25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(94.0f, 38.0f),  sf::Color::White),
        sf::Vertex(sf::Vector2f(164.0f, 25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(164.0f, 38.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(234.0f, 25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(234.0f, 38.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(304.0f, 25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(304.0f, 38.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(374.0f, 25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(374.0f, 38.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(444.0f, 25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(444.0f, 38.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(514.0f, 25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(514.0f, 38.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(584.0f, 25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(584.0f, 38.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(654.0f, 25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(654.0f, 38.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(724.0f, 25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(724.0f, 38.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(794.0f, 25.0f), sf::Color::White), sf::Vertex(sf::Vector2f(794.0f, 38.0f), sf::Color::White)
    };

    std::array<sf::Vertex, 22u> marks_y = {
        sf::Vertex(sf::Vector2f(54.0f,  50.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 50.0f),  sf::Color::White),
        sf::Vertex(sf::Vector2f(54.0f, 120.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 120.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(54.0f, 190.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 190.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(54.0f, 260.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 260.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(54.0f, 330.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 330.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(54.0f, 400.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 400.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(54.0f, 470.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 470.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(54.0f, 540.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 540.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(54.0f, 610.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 610.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(54.0f, 680.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 680.0f), sf::Color::White),
        sf::Vertex(sf::Vector2f(54.0f, 750.0f), sf::Color::White), sf::Vertex(sf::Vector2f(67.0f, 750.0f), sf::Color::White)
    };

    sf::RenderTexture rtexture;
    rtexture.create(831.0f, 775.0f);
    rtexture.clear(sf::Color::Transparent);

    rtexture.draw(x_axis);
    rtexture.draw(y_axis);
    rtexture.draw(marks_x.data(), marks_x.size(), sf::PrimitiveType::Lines);
    rtexture.draw(marks_y.data(), marks_y.size(), sf::PrimitiveType::Lines);
    
    for (register uint32_t i = 0u; i < marks.size(); i++) {
        rtexture.draw(marks[i]);
    }

    rtexture.display();

    return rtexture.getTexture().copyToImage();
}

CarloRubVid::GUIContainer::EditImgGUI CarloRubVid::create_edit_img_gui(const sf::Font& font, const std::vector<sf::Image>& imgs, const uint32_t& open_img_idx) {
    CarloRubVid::GUIContainer::EditImgGUI gui = {
        sf::RectangleShape(sf::Vector2f(1500.0f, 22.0f)),
        Button(
            sf::Vector2i(60, 10),
            sf::Vector2u(120u, 20u),
            "New image",
            font,
            19u,
            sf::Color(0x33, 0x33, 0x33),
            sf::Color(0x44, 0x44, 0x44)
        ),
        Button(
            sf::Vector2i(180, 10),
            sf::Vector2u(120u, 20u),
            "Import image",
            font,
            19u,
            sf::Color(0x33, 0x33, 0x33),
            sf::Color(0x44, 0x44, 0x44)
        ),
        Button(
            sf::Vector2i(300, 10),
            sf::Vector2u(120u, 20u),
            "Practice beat",
            font,
            19u,
            sf::Color(0x33, 0x33, 0x33),
            sf::Color(0x44, 0x44, 0x44)
        ),
        sf::Text("150 days remaining", font, 19u),
        CarloRubVid::Tabbar(imgs, font),
        render_bg_image(),
        render_coordinate_system(font),
        open_img_idx
    };

    return gui;
}

CarloRubVid::GUIContainer CarloRubVid::create_gui(const sf::Font& font, const std::vector<sf::Image>& images, const char* bg_path, const char* audio_path) {

    std::vector<std::string> frame_filepaths = CarloRubVid::list_files(bg_path);
    std::vector<std::pair<sf::Sprite, sf::Texture>> frames;
    frames.reserve(frame_filepaths.size());

    for (register uint32_t i = 0u; i < frame_filepaths.size(); i++) {
        sf::Texture texture;
        texture.loadFromFile(bg_path + frame_filepaths[i]);
        sf::Sprite sprite(texture);
        sprite.setPosition(.0f, -200.0f);
        sprite.setScale(1500.0f / texture.getSize().x, 1100.0f / texture.getSize().y);

        frames.push_back(std::make_pair(sprite, texture));
    }


    GUIContainer gui = {
        0u,
        {
            new CarloRubVid::GUIContainer::MainMenuGUI::Video(frames, 23.98f, audio_path),
            Button(
                sf::Vector2i(750, 350),
                sf::Vector2u(200u, 50u),
                "Edit images",
                font,
                19u,
                sf::Color(0xAA, 0x00, 0xAA),
                sf::Color(0xBB, 0x00, 0xBB)
            ),
            Button(
                sf::Vector2i(750, 450),
                sf::Vector2u(200u, 50u),
                "Practice beat",
                font,
                19u,
                sf::Color(0xAA, 0x00, 0xAA),
                sf::Color(0xBB, 0x00, 0xBB)
            )
        },
        {
            sf::RectangleShape(sf::Vector2f(1500.0f, 22.0f)),
            Button(
                sf::Vector2i(60, 10),
                sf::Vector2u(120u, 20u),
                "New image",
                font,
                19u,
                sf::Color(0x33, 0x33, 0x33),
                sf::Color(0x44, 0x44, 0x44)
            ),
            Button(
                sf::Vector2i(180, 10),
                sf::Vector2u(120u, 20u),
                "Import image",
                font,
                19u,
                sf::Color(0x33, 0x33, 0x33),
                sf::Color(0x44, 0x44, 0x44)
            ),
            Button(
                sf::Vector2i(300, 10),
                sf::Vector2u(120u, 20u),
                "Practice beat",
                font,
                19u,
                sf::Color(0x33, 0x33, 0x33),
                sf::Color(0x44, 0x44, 0x44)
            ),
            sf::Text("140 days remaining", font, 19u),
            CarloRubVid::Tabbar(images, font),
            render_bg_image(),
            render_coordinate_system(font),
            0u
        },
        {
            sf::Text("Select a song to practice", font),
            std::vector<sf::RectangleShape>(),
            CarloRubVid::GUIContainer::PracticeModeGUI::Song()
        }
    };

    gui.gui_main_menu.video->play();

    gui.gui_edit_img.txt_days_rem.setPosition(1300.0f, .0f);
    gui.gui_edit_img.toolbar.setFillColor(sf::Color(0x33, 0x33, 0x33));

    gui.gui_practice_mode.txt.setPosition(750.0f - gui.gui_practice_mode.txt.getLocalBounds().width / 2,
                                          450.0f - gui.gui_practice_mode.txt.getLocalBounds().height / 2);
                                          

    return gui;
}

CarloRubVid::OptionalGUIContainer CarloRubVid::create_optional_gui(const sf::Font& font, const std::vector<std::string>& filenames,
const std::vector<std::string>& songnames) {

    OptionalGUIContainer guis = {
        2u,
        { 
            0u,
            0u,
            font,
            sf::Text("Width: 0px", font),
            sf::Text("Height: 0px", font),
            Button(
                sf::Vector2i(),
                sf::Vector2u(30u, 20u),
                "\\/",
                font,
                18u,
                sf::Color(0x22, 0x22, 0x22),
                sf::Color(0x33, 0x33, 0x33)
            ),
            Button(
                sf::Vector2i(),
                sf::Vector2u(30u, 20u),
                "/\\",
                font,
                18u,
                sf::Color(0x22, 0x22, 0x22),
                sf::Color(0x33, 0x33, 0x33)
            ),
            Button(
                sf::Vector2i(),
                sf::Vector2u(30u, 20u),
                "\\/",
                font,
                18u,
                sf::Color(0x22, 0x22, 0x22),
                sf::Color(0x33, 0x33, 0x33)
            ),
            Button(
                sf::Vector2i(),
                sf::Vector2u(30u, 20u),
                "/\\",
                font,
                18u,
                sf::Color(0x22, 0x22, 0x22),
                sf::Color(0x33, 0x33, 0x33)
            ),
            sf::Text("Just to give you a feeling for sizes in pixels,\nthis line here is exactly 100px wide:", font),
            sf::RectangleShape(sf::Vector2f(100.0f, 10.0f)),
            Button(
                sf::Vector2i(95, 175),
                sf::Vector2u(180u, 50u),
                "Create new image",
                font,
                20u,
                sf::Color(0x22, 0x22, 0x22),
                sf::Color(0x33, 0x33, 0x33)
            )
        },
        {
            std::vector<sf::Vertex>(),
            std::vector<Button>()
        },
        {
            std::vector<sf::Vertex>(),
            std::vector<Button>()
        }
    };

    guis.gui_new_img.txt_width.setPosition(.0f, 25.0f);
    guis.gui_new_img.button_down_width.set_position(guis.gui_new_img.txt_width.getLocalBounds().width + 20, 60);
    guis.gui_new_img.button_up_width.set_position(guis.gui_new_img.txt_width.getLocalBounds().width + 20, 35);

    guis.gui_new_img.txt_height.setPosition(.0f, 75.0f);
    guis.gui_new_img.button_down_height.set_position(guis.gui_new_img.txt_height.getLocalBounds().width + 20, 110);
    guis.gui_new_img.button_up_height.set_position(guis.gui_new_img.txt_height.getLocalBounds().width + 20, 85);

    guis.gui_new_img.txt_demo.setPosition(.0f, 300.0f);
    guis.gui_new_img.demo.setPosition(5.0f, 400.0f);
    guis.gui_new_img.demo.setFillColor(sf::Color::Red);

    guis.gui_select_img.vertices_lines.reserve((filenames.size() + 1u) * 2u);
    guis.gui_select_img.listed_files.reserve(filenames.size());

    guis.gui_select_song.vertices_lines.reserve((songnames.size() + 1u) * 2u);
    guis.gui_select_song.listed_songs.reserve(songnames.size());

    for (register uint32_t i = 0u; i < (filenames.size() + 1) * 2; i++) {
        guis.gui_select_img.vertices_lines.emplace_back(sf::Vector2f((i % 2u) ? 700u : 0u, (i % 2u) ? ((i - 1) * 25.0f + 35.0f) : (i * 25.0f + 35.0f)), sf::Color::White);
    }

    for (register uint32_t i = 0u; i < filenames.size(); i++) {
        sf::Text txt(filenames[i], font);

        guis.gui_select_img.listed_files.emplace_back(
            sf::Vector2i(txt.getLocalBounds().width / 2, 50.0f * i + 59.0f),
            sf::Vector2u(txt.getLocalBounds().width, 49u),
            filenames[i],
            font,
            30u,
            sf::Color::Transparent,
            sf::Color(0x22, 0x22, 0x22)
        );
    }


    for (register uint32_t i = 0u; i < (songnames.size() + 1u) * 2; i++) {
        guis.gui_select_song.vertices_lines.emplace_back(sf::Vector2f((i % 2u) ? 700u : 0u, (i % 2u) ? ((i - 1) * 25.0f + 35.0f) : (i * 25.0f + 35.0f)), sf::Color::White);
    }

    for (register uint32_t i = 0u; i < songnames.size(); i++) {
        sf::Text txt(songnames[i], font);

        guis.gui_select_song.listed_songs.emplace_back(
            sf::Vector2i(txt.getLocalBounds().width / 2, 50.0f * i + 59.0f),
            sf::Vector2u(txt.getLocalBounds().width, 49u),
            songnames[i],
            font,
            30u,
            sf::Color::Transparent,
            sf::Color::Transparent
        );
    }

    return guis;
}