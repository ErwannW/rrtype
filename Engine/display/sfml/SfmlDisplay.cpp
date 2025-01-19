/*
** EPITECH PROJECT, 2024
** display
** File description:
** SfmlDisplay
*/

#include "SfmlDisplay.hpp"
#include <BuildTarget.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "Engine.hpp"

const static std::map<sf::Keyboard::Key, keyboardKey> sfToMGEKeys = {
    { sf::Keyboard::Key::Space, keyboardKey::Space },
    { sf::Keyboard::Key::Up   , keyboardKey::Arrow_up },
    { sf::Keyboard::Key::Z    , keyboardKey::Z },
    { sf::Keyboard::Key::Down , keyboardKey::Arrow_down },
    { sf::Keyboard::Key::S    , keyboardKey::S },
    { sf::Keyboard::Key::Left , keyboardKey::Arrow_left },
    { sf::Keyboard::Key::Q    , keyboardKey::Q },
    { sf::Keyboard::Key::Right, keyboardKey::Arrow_right },
    { sf::Keyboard::Key::D    , keyboardKey::D },
};

void SfmlDisplay::PreDraw()
{
    if (_window.isOpen())
        _window.clear(sf::Color::Black);
}

void SfmlDisplay::PostDraw()
{
    if (!_window.isOpen())
        return;

    poolevent();
    _window.display();
}

void SfmlDisplay::InitDisplay()
{
    _videoMode = sf::VideoMode::getDesktopMode();

    if (MGE::BuildTarget::CurrentBuildConfiguration == MGE::BuildTarget::BuildConfiguration::Debug)
        _window.create(_videoMode, "R-Type - Debug Build", sf::Style::Default);
    else
        _window.create(_videoMode, "R-Type", sf::Style::Fullscreen);

    _window.setFramerateLimit(60);
    _windowSize = _window.getSize();
    _view_impaired_mode = false;
    _sound_level = 50.f;
}

void SfmlDisplay::poolevent()
{
    for (auto& keyState : keysStates | std::views::values) {
        switch (keyState) {
            case KeyState::Pressed:
                keyState = KeyState::Down;
                break;
            case KeyState::Released:
                keyState = KeyState::Up;
                break;
            default:
                break;
        }
    }

    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window.close();
            MGE::Engine::Current().Quit();
            return;
        } else if (_event.type == sf::Event::KeyPressed) {
            const auto it = sfToMGEKeys.find(_event.key.code);

            if (it != sfToMGEKeys.end())
                keysStates[it->second] = KeyState::Pressed;
        } else if (_event.type == sf::Event::KeyReleased) {
            const auto it = sfToMGEKeys.find(_event.key.code);

            if (it != sfToMGEKeys.end())
                keysStates[it->second] = KeyState::Released;
        }
    }
}

void SfmlDisplay::AddWinElem(std::shared_ptr<WinElement> winElem)
{
    _winElems.push_back(winElem);
}

/**
 * @brief Invert the R,G and B value of a specific pixel
 * @param color The pixel whose value are to be inverted
 * @return The new inverted pixel
 */
static sf::Color InvertColor(const sf::Color &color)
{
    return sf::Color(255 - color.r, 255 - color.g, 255 - color.b, color.a);
}

/**
 * @brief Draw a sprite on the screen
 * @param TextureID The ID of the sprite to draw
 * @param pos_x The horizontal position (in percentage) of the center of the sprite
 * @param pos_y The vertical position (in percentage) of the center of the sprite
 * @param scale_x The horizontal scale (in percentage) of the sprite
 * @param scale_y The vertical scale (in percentage) of the sprite
 * @return void
 */
void SfmlDisplay::DrawSprite(const std::string &name, int pos_x, int pos_y, int scale_x, int scale_y) {
    if (!_window.isOpen()) return;

    if (!_sprites.contains(name)) {
        auto &spriteData = _sprites[name];
        spriteData.texture.loadFromFile("./Assets" +  lua.getString("getSpritePath", name));
        spriteData.sprite.setTexture(spriteData.texture);

        spriteData.isAnimated = lua.getFloat("getSpriteFrameRate", name) > 0.0f;
        spriteData.frames = lua.getSpriteFrames(name);
        spriteData.currentFrame = 0;
        spriteData.frameRate = lua.getFloat("getSpriteFrameRate", name);
        spriteData.frameTime = spriteData.frameRate > 0.0f ? 1.0f / spriteData.frameRate : 0.0f;
        spriteData.elapsedTime = 0.0f;
        spriteData.isInverted = false;
    }

    if ((_view_impaired_mode && !_sprites[name].isInverted) || (!_view_impaired_mode && _sprites[name].isInverted)) {
        sf::Image image = _sprites[name].texture.copyToImage();
        for (unsigned int x = 0; x < image.getSize().x; x++) {
            for (unsigned int y = 0; y < image.getSize().y; y++) {
                sf::Color pixel = image.getPixel(x, y);
                image.setPixel(x, y, InvertColor(pixel));
            }
        }
        _sprites[name].texture.loadFromImage(image);
        _sprites[name].isInverted = !(_sprites[name].isInverted);
    }

    auto &spriteData = _sprites[name];
    Frame frame = spriteData.frames[spriteData.isAnimated ? spriteData.currentFrame : 0];

    if (spriteData.isAnimated) {
        spriteData.elapsedTime += 1.0f / 60.0f;
        if (spriteData.elapsedTime >= spriteData.frameRate) {
            spriteData.currentFrame = (spriteData.currentFrame + 1) % spriteData.frames.size();
            spriteData.elapsedTime = 0.0f;
            frame = spriteData.frames[spriteData.currentFrame];
        }
    }

    spriteData.sprite.setTextureRect(sf::IntRect(frame.x, frame.y, frame.width, frame.height));
    spriteData.sprite.setOrigin(frame.width / 2.0f, frame.height / 2.0f);

    float scaleX = (float)_windowSize.x * scale_x / 100.0f / frame.width;
    float scaleY = (float)_windowSize.y * scale_y / 100.0f / frame.height;
    float scale = std::min(scaleX, scaleY);
    spriteData.sprite.setScale(scale, scale);

    spriteData.sprite.setPosition(
        (float)_windowSize.x * pos_x / 100.0f,
        (float)_windowSize.y * pos_y / 100.0f
    );

    _window.draw(spriteData.sprite);
}

/**
 * @brief GetValue the sound volume from a lua file OR/AND play or stop a sound
 * @param soundName The name of the sound to get
 * @param isPlaying If the sound is playing or not
 * @return void
 */
void SfmlDisplay::PlaySong(std::string const &name, bool isPlaying) {
    if (!_window.isOpen()) return;

    if (!_sounds.contains(name)) {
        auto &soundData = _sounds[name];
        soundData.buffer.loadFromFile("./Assets" + lua.getString("getSoundPath", name));
        soundData.sound.setBuffer(soundData.buffer);
        soundData.volume = lua.getFloat("getSoundVolume", name);
        soundData.sound.setVolume(_sound_level);
        soundData.loop = lua.getBool("getSoundLoop", name);
        soundData.sound.setLoop(soundData.loop);
        if (isPlaying)
            soundData.sound.play();
    } else {

        if (_sounds[name].sound.getStatus() != sf::Sound::Playing && isPlaying) {
            _sounds[name].sound.play();
        } else if (_sounds[name].sound.getStatus() == sf::Sound::Playing && !isPlaying && _sounds[name].sound.getLoop() == true) {
            _sounds[name].sound.stop();
        }
    }
}



/**
 * @brief Convert a hex color to a RGB color
 * @param hexColor The hex color to convert
 * @return The RGB color
 */
sf::Color SfmlDisplay::hexToRgb(const std::string& hexColor) {
    if (hexColor.size() != 7 || hexColor[0] != '#') {
        throw std::invalid_argument("Invalid hex color format");
    }

    unsigned int r, g, b;
    std::stringstream ss;
    ss << std::hex << hexColor.substr(1, 2);
    ss >> r;
    ss.clear();
    ss << std::hex << hexColor.substr(3, 2);
    ss >> g;
    ss.clear();
    ss << std::hex << hexColor.substr(5, 2);
    ss >> b;

    return sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b));
}

/**
 * @brief Draw a text on the screen
 * @param TextureID The ID of the text to draw
 * @param text The text to display
 * @param pos_x The horizontal position (in percentage) of the center of the text
 * @param pos_y The vertical position (in percentage) of the center of the text
 */
//void SfmlDisplay::DrawText(std::string const &TextureID, std::string const &text, float pos_x, float pos_y) {
void SfmlDisplay::DrawText(TextComponent &textComponent) {
    if (!_window.isOpen()) {
        return;
    }
    std::string const &TextureID = textComponent.texture_id;
    if (_texts.find(TextureID) == _texts.end()) {
        auto &textData = _texts[TextureID];
        textData.font.loadFromFile("./Assets" + lua.getString("getFontPath", TextureID));
        textData.fontSize = lua.getFloat("getFontSize", TextureID);
        textData.fontColor = this->hexToRgb(lua.getString("getFontColor", TextureID));
        textData.padding_left = lua.getFloat("getFontPaddingLeft", TextureID);
        textData.padding_top = lua.getFloat("getFontPaddingTop", TextureID);
    }

    std::string fitText = textComponent.text;
    if (textComponent.is_input) {
        if (fitText.size() > textComponent.textMaxSize) {
            if (textComponent.display_position == -1) {
                textComponent.display_position = static_cast<int>(fitText.size()) - textComponent.textMaxSize;
            }
            if (_event.type == _event.KeyPressed) {
                if (_event.key.code == sf::Keyboard::Left) {
                    if (textComponent.display_position > 0) {
                        textComponent.display_position--;
                    }
                } else if (_event.key.code == sf::Keyboard::Right) {
                    if (textComponent.display_position < (fitText.size() - textComponent.textMaxSize)) {
                        textComponent.display_position++;
                    }
                }
            }
            fitText = fitText.substr(textComponent.display_position, textComponent.textMaxSize);
        } else {
            textComponent.display_position = 0;
        }
    }

    auto &textData = _texts[TextureID];
    sf::Text sfText;
    sfText.setFont(textData.font);
    sfText.setString(fitText);
    sfText.setCharacterSize(static_cast<unsigned int>(textData.fontSize));
    sfText.setFillColor(textData.fontColor);
    sfText.setOrigin(sfText.getLocalBounds().width / 2.0f, sfText.getLocalBounds().height / 2.0f);
    sfText.setPosition(
        (float)_windowSize.x * textComponent.position_x / 100.0f + (float)_windowSize.x * textData.padding_left / 100.0f,
        (float)_windowSize.y * textComponent.position_y / 100.0f + (float)_windowSize.x * textData.padding_top / 100.0f
    );
    _window.draw(sfText);
}

/**
 * @brief Check if a button is pressed
 * @param x The horizontal position (in percentage) of the center of the button
 * @param y The vertical position (in percentage) of the center of the button
 * @param TextureID The ID of the button
 * @param pos_x The horizontal position (in percentage) of the center of the button
 * @param pos_y The vertical position (in percentage) of the center of the button
 * @param scale_x The horizontal scale (in percentage) of the button
 * @param scale_y The vertical scale (in percentage) of the button
 * @return true if the button is hover, false otherwise
 */
bool SfmlDisplay::buttonGestion(float x, float y, const std::string &TextureID, float pos_x, float pos_y, float scale_x, float scale_y)
{
    if (!_window.isOpen())
        return false;

    if (_sprites.find(TextureID) != _sprites.end()) {
        auto &spriteData = _sprites[TextureID];

        float scaleX = (float)_windowSize.x * scale_x / 100.0f / spriteData.frames[spriteData.currentFrame].width;
        float scaleY = (float)_windowSize.y * scale_y / 100.0f / spriteData.frames[spriteData.currentFrame].height;
        float scale = std::min(scaleX, scaleY);

        float width = spriteData.frames[spriteData.currentFrame].width * scale;
        float height = spriteData.frames[spriteData.currentFrame].height * scale;

        x = (float)_windowSize.x * x / 100.0f;
        y = (float)_windowSize.y * y / 100.0f;

        float left = x - width / 2.0f;
        float right = x + width / 2.0f;
        float top = y - height / 2.0f;
        float bottom = y + height / 2.0f;

        sf::Vector2i mousePos = sf::Mouse::getPosition(_window);
        if (mousePos.x >= left && mousePos.x <= right && mousePos.y >= top && mousePos.y <= bottom) {
            return true;
        }
    }

    return false;
}

/**
 * @brief GetValue the text entered by the user
 * @param previousText The previous text entered by the user
 * @return void
 */
void SfmlDisplay::getText(std::string &previousText, TextComponent &textComponent) {
    if (_event.type == sf::Event::TextEntered) {
        if (_event.text.unicode < 128) {
            previousText = static_cast<char>(_event.text.unicode);
        }
    }

    if (_event.type == sf::Event::KeyReleased) {
        if (!previousText.empty()) {
            if (previousText == "\b") {
                if (!textComponent.text.empty()) {
                    textComponent.text.pop_back();
                    if (textComponent.display_position > 0) {
                        textComponent.display_position--;
                    }
                }
            } else {
                textComponent.text += previousText;
                textComponent.display_position++;
            }
            previousText.clear();
        }
    }
}

const std::map<keyboardKey, KeyState> &SfmlDisplay::getKeyBind()
{
    return keysStates;
}

void SfmlDisplay::StopAllSong() {
    for (auto &pair : _sounds) {
        if (pair.second.sound.getStatus() == sf::Sound::Playing) {
            pair.second.sound.stop();
            pair.second.sound.setLoop(false);
        }
    }
}