#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <time.h>
#include <chrono>
#include <vector>
#include <tuple>
#include <fstream>

#include "CarloRubVid/button.hpp"
#include "CarloRubVid/tabbar.hpp"


#include <iostream>

#ifndef CARLO_RUB_VID_INIT
#define CARLO_RUB_VID_INIT

namespace CarloRubVid {

    // The container for all the elements of the GUI (acronyme for Graphical User Interface) of the main window
    struct GUIContainer {
        uint32_t type_idx;

        struct MainMenuGUI {

            class Video : public sf::Drawable {
                bool m_is_playing;
                std::chrono::time_point<std::chrono::system_clock> m_start_time;
                uint8_t m_fps;
                float m_duration;
                std::vector<std::pair<sf::Sprite, sf::Texture>> m_frames;
                sf::Music* m_soundtrack_ptr;


            public:

                Video(const std::vector<std::pair<sf::Sprite, sf::Texture>>& frames, const uint8_t& fps, const char* audio_path)
                : m_frames(frames), m_fps(fps), m_is_playing(false), m_soundtrack_ptr(new sf::Music()) {
                    m_soundtrack_ptr->openFromFile(audio_path);
                    m_soundtrack_ptr->setLoop(true);

                    m_duration = static_cast<float>(frames.size()) / static_cast<float>(fps);
                }

                inline ~Video() {
                    m_soundtrack_ptr->stop();
                    delete m_soundtrack_ptr;
                }

                inline void play() {
                    m_is_playing = true;
                    m_start_time = std::chrono::high_resolution_clock::now();
                    m_soundtrack_ptr->play();
                }

                inline void pause() {
                    m_is_playing = false;
                    m_soundtrack_ptr->pause();
                }

                inline void stop() {
                    m_is_playing = false;
                    m_soundtrack_ptr->stop();
                }

                inline void restart() {
                    stop();
                    play();
                }

                inline void update() { if (std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - m_start_time).count() > m_duration) { restart(); } }

                virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const {
                    const float elapsed_time = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - m_start_time).count();

                    uint32_t frame_idx = m_fps * elapsed_time;
                    if (frame_idx >= m_frames.size()) return;

                    sf::Sprite sprite = std::get<0>(m_frames[frame_idx]);
                    sprite.setTexture(std::get<1>(m_frames[frame_idx]));
                    window.draw(sprite);
                }

                Video& operator=(const Video& other) {
                    m_is_playing = other.m_is_playing;
                    m_start_time = other.m_start_time;
                    m_fps        = other.m_fps;
                    m_frames     = other.m_frames;
                    m_duration   = other.m_duration;

                    delete m_soundtrack_ptr;
                    m_soundtrack_ptr = other.m_soundtrack_ptr;

                    return *this;
                }

            } *video;

            Button button_edit_img, button_practice_beat;

        } gui_main_menu;

        struct EditImgGUI {
            sf::RectangleShape toolbar;
            Button button_new_img;
            Button button_import_imgs;
            Button button_practice_mode;
            sf::Text txt_days_rem;
            CarloRubVid::Tabbar tabbar;
            sf::Image bg_image;
            sf::Image coordinate_system;
            uint32_t open_image_idx;

            EditImgGUI& operator=(const EditImgGUI& other) {
                this->tabbar = other.tabbar;
                this->open_image_idx = other.open_image_idx;
                return *this;
            }
        } gui_edit_img;

        struct PracticeModeGUI {
            sf::Text txt;
            std::vector<sf::RectangleShape> beat_boxes;

            class Song {
                sf::Music* m_music_ptr;
                float m_offset;
                float m_bpm;
                sf::Clock m_clock;
            public:

                inline Song() : m_music_ptr(nullptr), m_bpm(.0f), m_offset(std::stof("inf")) {}

                Song(const std::string& filepath) {
                    m_music_ptr = new sf::Music();
                    if (!m_music_ptr->openFromFile(filepath + ".ogg")) throw std::runtime_error("Couldn't open song at " + filepath + ".ogg");

                    std::ifstream file_song_data(filepath + ".wsg");

                    if (!file_song_data.is_open()) throw std::runtime_error("Couldn't open song data file at " + filepath + ".wsg");

                    std::string line;

                    std::getline(file_song_data, line);

                    file_song_data.close();

                    m_bpm = std::stof(line);
                }

                inline ~Song() { destroy(); }

                inline void play() { if (!m_music_ptr) return; m_music_ptr->play(); m_clock.restart(); }

                inline void stop() { if (m_music_ptr) m_music_ptr->stop(); }

                void destroy() {
                    if (!m_music_ptr) return;
                    stop();
                    delete m_music_ptr;
                    m_music_ptr = nullptr; }

                void set_song(const std::string& filepath) {
                    if (!m_music_ptr) {
                        m_music_ptr = new sf::Music();
                    } else {
                        stop();
                    }

                    m_music_ptr->openFromFile(filepath + ".ogg");

                    std::ifstream file_song_data(filepath + ".wsg");

                    if (!file_song_data.is_open()) throw std::runtime_error("Couldn't open song data file at " + filepath + ".wsg");

                    std::string line, s_offset, s_bpm;

                    std::getline(file_song_data, line);

                    file_song_data.close();

                    bool sep = false;
                    for (register uint32_t i = 0u; i < line.length(); i++) {
                        if (line[i] == ';') {
                            sep = true;
                            continue;
                        }

                        if (sep) s_bpm    += line[i];
                        else     s_offset += line[i];
                    }

                    m_offset = std::stof(s_offset);
                    m_bpm = std::stof(s_bpm);
                }

                void add_beat_box(std::vector<sf::RectangleShape>& beat_boxes) {
                    static sf::Clock delta;

                    if (!m_music_ptr || m_music_ptr->getStatus() != sf::SoundStream::Status::Playing) return;

                    if (m_music_ptr->getPlayingOffset().asSeconds() < m_offset) return;

                    if (m_clock.getElapsedTime().asSeconds() * (m_bpm / 60.0f) >= 1) {
                        m_clock.restart();

                        sf::RectangleShape beat_box(sf::Vector2f(30.0f, 100.0f));
                        beat_box.setFillColor(sf::Color::Magenta);
                        beat_box.setPosition(1500.0f, 650.0f);
                        beat_boxes.push_back(beat_box);
                    }

                    // Update beat boxes
                    const float dt = delta.restart().asSeconds();
                    for (register uint32_t i = beat_boxes.size() - 1u; i < beat_boxes.size(); i--) {
                        beat_boxes[i].move(-300.0f * dt, .0f);

                        if (beat_boxes[i].getPosition().x < .0f) { beat_boxes.erase(beat_boxes.begin() + i); }
                    }
                }
                
            } song;

            inline ~PracticeModeGUI() { song.destroy(); }
            
        } gui_practice_mode;

    };

    // The container for all the GUIs the optional window can have (not at the same time, of course)
    struct OptionalGUIContainer {

        /* The type_idx is an unisgned 32-bit integer representing the type of the GUI of the optional window. 0 stands for the window being closed, 1 = GUI for
        *  creating a new image, 2 = GUI for selecting an image from a file.*/
        uint32_t type_idx;

        // The container of all the elements of the GUI for the optional window for creating a new imgage
        struct NewImgGUI {
            uint32_t width, height;
            const sf::Font& font;
            sf::Text txt_width, txt_height;
            Button button_down_width, button_up_width, button_down_height, button_up_height;
            sf::Text txt_demo;
            sf::RectangleShape demo;
            Button button_confirm;
        } gui_new_img;

        // The container of all the elements of the GUI for the optional window for selecting an imgage from a file
        struct SelectImgGUI {
            std::vector<sf::Vertex> vertices_lines;
            std::vector<Button> listed_files;
        } gui_select_img;

        struct SelectSongGUI {
            std::vector<sf::Vertex> vertices_lines;
            std::vector<Button> listed_songs;
        } gui_select_song;
    };

    /* Returning all additional arguments from the parameters of the main-function (for example, if the appication is launched by an UNIX-Terminal
    *  by running "./application img1.png img2.png", a vector = {"img1.png", "img2.png"} would be returned).
    *  
    * Parameters:
    * - argc: A const reference to the argc-parameter of the main-function, representing the count (therefore the 'c' after 'arg') of arguments.
    * - argv: The argv-parameter of the main-function, representing the values (therefore the 'v' after 'arg') of the parameters.
    * 
    * Returns: A vector of std::strings representing the additional arguments from the main-function
    */
    std::vector<std::string> parse_args(const int& argc, char** argv);

    /* Creates and returns a GUI for the main window.
    *
    *  Parameters:
    *  - font: The Font which should be applied for the texts in the GUI.
    *  - imgs: All loaded images which should and will be shown up in the GUI (eg. in the tabbar and the shown image for editing)
    *  - movie_path: The path to the movie played in background in main menu
    * 
    *  Returns: A GUI with all the nessecairy elements in right color, position and alignment. 
    */
    GUIContainer create_gui(const sf::Font& font, const std::vector<sf::Image>& imgs, const char* movie_path, const char* video_soundtrack_path);

    /* Creates and returns a GUI for the editing images screen of the main window.
    *
    *  Parameters:
    *  - font: The font which should be applied for the texts in the GUI.
    *  - imgs: All loaded images which should and will be shown up in the GUI (eg. in the tabbar and the shown image for editing)
    *  - open_img_idx: 
    */
    GUIContainer::EditImgGUI create_edit_img_gui(const sf::Font& font, const std::vector<sf::Image>& imgs, const uint32_t& open_img_idx);

    /* Creates and returns a GUI for the optional window.
    *
    *  Parameters:
    *  - font: The Font which should be applied for the texts in the GUI.
    *  - filenames: A std::vector with all filenames of the images which are supposed to be listed in the file selection menu in the optional GUI.
    * 
    *  Returns: An OptionalGUIContainer for the optional window.
    */
    OptionalGUIContainer create_optional_gui(const sf::Font& font, const std::vector<std::string>& filenames, const std::vector<std::string>& songnames);

} // namespace CarloRubVid

#endif // #ifndef CARLO_RUB_VID_INIT