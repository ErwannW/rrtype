
#ifndef GENERIQUE_COMPONENT_HPP
#define GENERIQUE_COMPONENT_HPP

#include <queue>
#include <utility>
#include <string>
#include <memory>
#include <functional>
#include "../Level/ILevel.hpp"
#include "Reflection/Reflection.hpp"
#include "Entity.hpp"

/**
 * @struct SongComponent_t
 * @brief Component representing a basic sprite with position and texture properties.
 */
struct SongComponent {
    REFLECT_STRUCT(SongComponent)

    std::string texture_id; /**< Texture ID used for the sprite */
    bool isPlaying;         /**< Indicates if the song is playing */

    explicit SongComponent(std::string texture_id, bool isPlaying);
};

/**
 * @struct MouseConponent_t
 * @brief Component representing the mouse entities.
 */
struct MouseComponent {
    REFLECT_STRUCT(MouseComponent)

    int pos_x; /**< X position of the mouse (in percentage) */
    int pos_y; /**< Y position of the mouse (in percentage) */
    bool displayDefaultMouse; /**< Indicates if the render displays the default mouse */
    bool LeftButtonPressed; /**< Indicates if the left button is pressed */
    bool RightButtonPressed; /**< Indicates if the right button is pressed */

    MouseComponent(int pos_x, int pos_y, bool displayDefaultMouse, bool LeftButtonPressed, bool RightButtonPressed);
};

/**
 * @struct SpriteComponent
 * @brief Component representing a basic sprite with position and texture properties.
 */
struct SpriteComponent
{
    REFLECT_STRUCT(SpriteComponent)

    std::string texture_id; /**< Texture ID used for the sprite */
    float width;            /**< Width of the sprite in % of the screen */
    float height;           /**< Height of the sprite in % of the screen */

    std::string uuid;       /**< Uuid used to identify the component to the renderer */

    SpriteComponent(std::string texture_id, float width, float height);

    bool operator==(const SpriteComponent& other) const {
        return uuid == other.uuid;
    }

    bool operator!=(const SpriteComponent& other) const {
        return texture_id != other.texture_id;
    }
};

struct IdComponent
{
    REFLECT_STRUCT(IdComponent)

    explicit IdComponent(int id);
    int id = 0;
};

struct ButtonComponent {
    REFLECT_STRUCT(ButtonComponent)

    bool isDisabled; /**< Indicates if the button is disabled */
    bool isPressed; /**< Indicates if the button is pressed */
    bool mouse_exit_hover; /**< Indicates if the mouse is exiting the hover state */
    std::string texture_id_standard; /**< Texture ID used for the standard state */
    std::string texture_id_hover; /**< Texture ID used for the hover state */
    std::string texture_id_disabled; /**< Texture ID used for the disabled state */

    std::function<void()> onClick;

    explicit ButtonComponent(
        bool isDisabled = false,
        std::string texture_id_standard = "",
        std::string texture_id_hover = "",
        std::string texture_id_disabled = "",
        bool mouse_exit_hover = true,
        std::function<void()> onClick = nullptr,
        bool isPressed = false);
};



/**
 * @struct TextComponent
 * @brief Component representing the text properties of an entity.
 */
struct TextComponent {
    REFLECT_STRUCT(TextComponent)

    std::string text;      /**< Text displayed */
    std::string texture_id;
    float position_x;        /**< X position of the sprite */
    float position_y;        /**< Y position of the sprite */

    bool is_input;          /* Indicate if this text is inputable or not */
    int textMaxSize;        /* Text Maximum size */
    int display_position;   /* (Only if inputable) position in the text to be displayed */

    std::string uuid;       /**< Uuid used to identify the component to the renderer */

    explicit TextComponent(std::string text, std::string texture_id, float position_x = 0, float position_y = 0, bool is_input = false, int textMaxSize = 20, int display_position = -1);

    bool operator==(const TextComponent& other) const {
        return uuid == other.uuid;
    }
};


/**
 * @struct InputComponent_t
 * @brief Component representing the input states for an entity (e.g., button press).
 */
struct InputComponent {
    REFLECT_STRUCT(InputComponent)

    bool isHeld;     /**< Indicates if the input is held */
    bool isHovered;  /**< Indicates if the input is hover */
    std::string previousText;

    explicit InputComponent(bool isHeld = false, bool isHovered = false, std::string previousText = "");
};

/**
 * @struct MouseConponent_t
 * @brief Component representing the mouse entities.
 */
struct KeyBoardsComponents {
    REFLECT_STRUCT(KeyBoardsComponents)

    std::string input;

    explicit KeyBoardsComponents(std::string input = "");
};

/**
 * @struct SoundComponent_t
 * @brief Component representing the sound level.
 */
struct SoundComponent {
    REFLECT_STRUCT(SoundComponent)

    float volume;

    explicit SoundComponent(float volume = 50.f);
};

class GenericComponent
{
    public:
        GenericComponent() = default;
        ~GenericComponent() = default;
        
        std::shared_ptr<MGE::Entity>
        CreateButton(MGE::World& world, const std::string& spriteID, const std::string& textID, const std::string& text, float x, float y, bool isDisabled, float size, std::function<void()> onClick);
        static void ButtonSystem(MGE::World& world, const MGE::Entity& entity, SpriteComponent& spriteComponent, ButtonComponent& buttonComponent, MouseComponent& mouseConponent);

        std::shared_ptr<MGE::Entity>
        CreateInput(MGE::World &world, const std::string &spriteID, const std::string &textID, const std::string &text, int textMaxSize, float size_x, float size_y, bool isDisabled, float pos_x, float pos_y, std::function<void()> onClick);

};

#endif