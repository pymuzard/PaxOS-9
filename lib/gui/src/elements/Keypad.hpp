//
// Created by Charles on 13/03/2024.
//

#ifndef KEYPAD_HPP
#define KEYPAD_HPP

#include "../ElementBase.hpp"
#include "Box.hpp"
#include "Canvas.hpp"
#include "Filter.hpp"
#include "Image.hpp"
#include "Label.hpp"

namespace gui::elements
{

    // 0x0_ => Control chars
    constexpr char KEY_NULL = 0x00;
    constexpr char KEY_EXIT = 0x01;

    constexpr char KEY_BACKSPACE = 0x11;

    constexpr char KEY_CALL = 0x21;

    class Keypad final : public ElementBase
    {
    public:
        enum keyPadType
        {
            KEYPAD_NUM,     // Keypad with num only
            KEYPAD_CALL,    // keypad set up for calls
            KEYPAD_PASSWORD // keypad with num only,; not displaying the typed num
        };

        explicit Keypad(const std::string &defaultText = "", keyPadType type = keyPadType::KEYPAD_NUM, int _passwordLength = 4);

        ~Keypad() override;

        void render() override;

        void widgetUpdate() override;

        /**
         * Returns the content of the keyboard's input AND CLEARS IT.
         * @return the content of the keyboard's input
         */
        std::string getText();

        void setPasswordLength(int len);

        /**
         * @brief
         *
         * @return true
         * @return false
         */
        [[nodiscard]] bool hasExitKeyBeenPressed() const;

        [[nodiscard]] bool passwordToBeChecked() const;

    private:
        std::string m_buffer;
        std::string m_defaultText;
        keyPadType m_type;
        char **m_layout;

        Label *m_label;

        int passwordLength;
        Canvas *m_canvasPassword;

        Image *m_backspaceIcon;
        Image *m_keypadExitIcon;
        Image *m_keypad_call;

        bool m_exit = false;
        bool m_passwordToBeChecked = false;

        Canvas *m_keysCanvas;
        Box *m_trackpadActiveBox;
        int keyInterval;
        int nbCol;
        int nbRow;

        Image *m_trackpadActiveIcon;
        uint8_t m_trackpadTicks;
        int32_t m_trackpadLastDeltaX;

        void drawKeys();

        [[nodiscard]] char getKey(int16_t x, int16_t y) const;

        void processKey(char key);

        void drawInputBox() const;

        void trackpadUpdate();

        [[nodiscard]] bool isPointInTrackpad(int16_t x, int16_t y) const;

        [[nodiscard]] bool isTrackpadActive() const;

        void addChar(char value);

        void removeChar();
    };
} // gui::elements

#endif // KEYPAD_HPP
