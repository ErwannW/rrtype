/*
** EPITECH PROJECT, 2024
** display
** File description:
** SfmlDisplay
*/


#ifndef SFMLDISPLAY_HPP_
#define SFMLDISPLAY_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "../../GeneriqueEntity/GeneriqueComponent.hpp"

#include "../Lua/Lua.hpp"
#include "../IDisplay.hpp"

enum class keyboardKey : uint8 {
    None, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Arrow_up, Arrow_down, Arrow_left, Arrow_right,
    Space, Enter, Escape, Backspace, Pause
};

enum class KeyState : uint8 {
    Up,
    Pressed,
    Down,
    Released
};

constexpr bool IsPressedOrDown(const KeyState state)
{
    return state == KeyState::Pressed || state == KeyState::Down;
}

constexpr bool IsReleasedOrUp(const KeyState state)
{
    return state == KeyState::Released || state == KeyState::Up;
}

class WinElement {
public:
    virtual ~WinElement() = default;
    virtual void drawElem(sf::RenderWindow &window) = 0;
};

struct SpriteManagement {
    sf::Texture texture;
    sf::Sprite sprite;
    bool isAnimated;
    std::vector<Frame> frames;
    float frameRate;
    float frameTime;
    float elapsedTime;
    int currentFrame;
    bool isInverted;
};

struct SoundManagement {
    sf::SoundBuffer buffer;
    sf::Sound sound;
    float volume;
    bool loop;
};

struct TextManagement {
    sf::Font font;
    float fontSize;
    sf::Color fontColor;
    bool isButton;
    float padding_left;
    float padding_top;
};

class SfmlDisplay : public IDisplay {
    public:
        ~SfmlDisplay() override {
        };

        void PreDraw() override;
        void PostDraw() override;

        void InitDisplay() override;

        void poolevent();

        void AddWinElem(std::shared_ptr<WinElement> winElem);

        bool IsMouseLeftPressed() override {
            return sf::Mouse::isButtonPressed(sf::Mouse::Left);
        }

        bool IsMouseRightPressed() override {
            return sf::Mouse::isButtonPressed(sf::Mouse::Right);
        }

        void DrawSprite(std::string const &TextureID, int pos_x, int pos_y, int scale_x, int scale_y);
        //void DrawText(std::string const &TextureID, std::string const &text, float pos_x, float pos_y);
        void DrawText(TextComponent &TextComponent);


        void PlaySong(std::string const &name, bool isPlaying);
        void StopAllSong(void);

        bool buttonGestion(float x, float y, const std::string &TextureID, float pos_x, float pos_y, float scale_x, float scale_y);

        sf::Color hexToRgb(const std::string& hexColor);

        std::pair<float, float> getMousePosInPercent() override {
            sf::Vector2i mousePos = sf::Mouse::getPosition(_window);
            sf::Vector2u windowSize = _window.getSize();
            return {static_cast<float>(mousePos.x) / windowSize.x * 100.0f, static_cast<float>(mousePos.y) / windowSize.y * 100.0f};
        }

        void getText(std::string &previousText, TextComponent &Text);

        const std::map<keyboardKey, KeyState> &getKeyBind();

        void SwitchDisplayMode()
        {
            if (_view_impaired_mode) {
                _view_impaired_mode = false;
            } else {
                _view_impaired_mode = true;
            }
        }

        bool GetDisplayMode() const
        {
            return _view_impaired_mode;
        }

        float GetSoundLevel() const
        {
            return _sound_level;
        }

        void ChangeSoundLevel(const float variation)
        {
            _sound_level += variation;
            if (_sound_level > 100.f) {
                _sound_level = 100.f;
            } else if (_sound_level < 0.f) {
                _sound_level = 0.f;
            }
            for (auto &songs : _sounds) {
                songs.second.sound.setVolume(_sound_level);
            }
        }

        sf::RenderWindow _window;
        sf::VideoMode _videoMode;
        sf::Event _event;
        std::vector<std::shared_ptr<WinElement>> _winElems;
        sf::Vector2u _windowSize;

    private:
        std::map<keyboardKey, KeyState> keysStates;

        Lua lua;
        bool _view_impaired_mode;
        float _sound_level;
        std::map<std::string, SpriteManagement> _sprites;
        std::map<std::string, SoundManagement> _sounds;
        std::map<std::string, TextManagement> _texts;

};

#endif /* !SFMLDISPLAY_HPP_ */