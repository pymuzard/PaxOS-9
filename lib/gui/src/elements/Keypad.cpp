//
// Created by Charles on 13/03/2024.
//

#include "keypad.hpp"

#include <iostream>
#include <graphics.hpp>
#include <libsystem.hpp>
#include <Surface.hpp>

#include "Box.hpp"
#include "Filter.hpp"
#include "Image.hpp"

#include <libsystem.hpp>

namespace gui::elements
{

    Keypad::Keypad(const std::string &defaultText, keyPadType _type, int _passwordLength)
    {
        m_type = _type;

        if (m_type != Keypad::KEYPAD_PASSWORD)
        {
            m_buffer = defaultText;
            m_defaultText = defaultText;
        }
        else
        {
            m_buffer = "";
            m_defaultText = "";
        }
        passwordLength = _passwordLength;

        m_width = graphics::getScreenWidth();
        m_height = graphics::getScreenHeight();

        m_x = 0;
        m_y = 0;

        m_hasEvents = true;

        m_keysCanvas = new Canvas(10, 100, m_width - 2 * 10, m_height - 100 - 2 * 10);
        addChild(m_keysCanvas);

        // Create label for text
        if (m_type == Keypad::KEYPAD_PASSWORD)
        {
            m_canvasPassword = new Canvas(10, 10, m_width - 2 * 10, 100 - 2 * 10);
            m_canvasPassword->fillRect(0, 0, m_canvasPassword->getWidth(), m_canvasPassword->getHeight(), libsystem::paxoConfig::getBackgroundColor());
            addChild(m_canvasPassword);
        }

        else
        {
            m_label = new Label(10, 10, m_width - 2 * 10, 100 - 2 * 10);
            // m_label->setFont(graphics::ARIAL);
            m_label->setFontSize(32);
            m_label->setHorizontalAlignment(Label::Alignement::CENTER);
            m_label->setVerticalAlignment(Label::Alignement::CENTER);
            m_label->setCursorEnabled(true);
            m_label->setCursorIndex(static_cast<int16_t>(m_buffer.length()));
            addChild(m_label);
        }

        m_trackpadActiveBox = new Box(std::floor((m_width - 64) / 2), 100, 96, 96);
        m_trackpadActiveBox->setBackgroundColor(TFT_BLACK);
        m_trackpadActiveBox->setRadius(8);
        addChild(m_trackpadActiveBox);

        m_trackpadActiveIcon = new Image(storage::Path("system/keyboard/trackpad_active_icon.png"), 16, 16, 64, 64);
        m_trackpadActiveIcon->load(TFT_BLACK);
        m_trackpadActiveBox->addChild(m_trackpadActiveIcon);

        m_trackpadActiveBox->disable();

        switch (m_type)
        {
        case Keypad::KEYPAD_NUM:
            nbCol = 3;
            nbRow = 4;
            m_layout = new char *[nbRow];
            m_layout[0] = new char[3]{'1', '2', '3'};
            m_layout[1] = new char[3]{'4', '5', '6'};
            m_layout[2] = new char[3]{'7', '8', '9'};
            m_layout[3] = new char[3]{KEY_EXIT, '0', KEY_BACKSPACE};

            m_keypadExitIcon = new Image(storage::Path("system/keyboard/keypad_exit.png"), 0, 0, 40, 40);
            m_backspaceIcon = new Image(storage::Path("system/keyboard/backspace.png"), 0, 0, 40, 40);
            addChild(m_keypadExitIcon);
            addChild(m_backspaceIcon);

            break;
        case Keypad::KEYPAD_PASSWORD:

            nbCol = 3;
            nbRow = 4;
            m_layout = new char *[nbRow];
            m_layout[0] = new char[3]{'1', '2', '3'};
            m_layout[1] = new char[3]{'4', '5', '6'};
            m_layout[2] = new char[3]{'7', '8', '9'};
            m_layout[3] = new char[3]{KEY_EXIT, '0', KEY_BACKSPACE};

            m_keypadExitIcon = new Image(storage::Path("system/keyboard/keypad_exit.png"), 0, 0, 40, 40);
            m_backspaceIcon = new Image(storage::Path("system/keyboard/backspace.png"), 0, 0, 40, 40);
            addChild(m_keypadExitIcon);
            addChild(m_backspaceIcon);

            break;
        case Keypad::KEYPAD_CALL:

            nbCol = 3;
            nbRow = 5;
            m_layout = new char *[nbRow];
            m_layout[0] = new char[3]{'1', '2', '3'};
            m_layout[1] = new char[3]{'4', '5', '6'};
            m_layout[2] = new char[3]{'7', '8', '9'};
            m_layout[3] = new char[3]{'+', '0', '#'};
            m_layout[4] = new char[3]{KEY_EXIT, KEY_CALL, KEY_BACKSPACE};

            m_keypadExitIcon = new Image(storage::Path("system/keyboard/keypad_exit.png"), 0, 0, 40, 40);
            m_keypad_call = new Image(storage::Path("system/keyboard/phone.png"), 0, 0, 40, 40);
            m_backspaceIcon = new Image(storage::Path("system/keyboard/backspace.png"), 0, 0, 40, 40);
            addChild(m_keypadExitIcon);
            addChild(m_keypad_call);
            addChild(m_backspaceIcon);

            break;
        default:
            std::cerr << "Keypad has no KeypadType !!!" << std::endl;
        }

        m_trackpadTicks = 0;
        m_trackpadLastDeltaX = 0;
    }

    Keypad::~Keypad() = default;

    void Keypad::render()
    {
        m_surface->fillRect(0, 0, m_width, m_height, m_backgroundColor);

        // Input box
        drawInputBox();

        if (!isTrackpadActive())
        {
            // Draw keys
            drawKeys();
        }
    }

    void Keypad::widgetUpdate()
    {
        if (isTouched())
        {
            // Get touch position
            int16_t touchX, touchY;
            getLastTouchPosRel(&touchX, &touchY);

            const char pressedKey = getKey(touchX, touchY);
            if (pressedKey == KEY_NULL)
            {
                return;
            }

            processKey(pressedKey);
            drawInputBox();
        }

        if (m_type != Keypad::KEYPAD_PASSWORD)
            trackpadUpdate();
    }

    std::string Keypad::getText()
    {
        const std::string output = m_buffer;

        m_buffer = "";

        return output;
    }

    void Keypad::drawKeys()
    {
        // Reset default settings
        m_keysCanvas->fillRect(0, 0, m_keysCanvas->getWidth(), m_keysCanvas->getHeight(), libsystem::paxoConfig::getScreenColor());
        keyInterval = 2;

        int keyHeight = std::floor((m_keysCanvas->getHeight() - (nbRow + 1) * keyInterval) / nbRow);

        color_t keyColor = libsystem::paxoConfig::getBorderColor();
        color_t textColor = libsystem::paxoConfig::getTextColor();
        color_t backgroundColor = libsystem::paxoConfig::getBackgroundColor();

        for (int i = 0; i < nbRow; i++)
        {
            int keyWidth = std::floor((m_keysCanvas->getWidth() - (nbCol + 1) * keyInterval) / nbCol);

            for (int j = 0; j < nbCol; j++)
            {

                switch (m_layout[i][j])
                {
                case KEY_CALL:
                    m_keysCanvas->fillRoundRect(keyInterval + j * (keyWidth + keyInterval), keyInterval + i * (keyHeight + keyInterval), keyWidth, keyHeight, 5, COLOR_LIGHT_GREEN);

                    m_keypad_call->setX(m_keysCanvas->getX() + keyInterval + j * (keyWidth + keyInterval) + std::floor((keyWidth - m_keypad_call->getWidth()) / 2));
                    m_keypad_call->setY(m_keysCanvas->getY() + keyInterval + i * (keyHeight + keyInterval) + std::floor((keyHeight - m_keypad_call->getHeight()) / 2));
                    m_keypad_call->setTransparentColor(backgroundColor);
                    break;
                case KEY_BACKSPACE:
                    m_backspaceIcon->setX(m_keysCanvas->getX() + keyInterval + j * (keyWidth + keyInterval) + std::floor((keyWidth - m_backspaceIcon->getWidth()) / 2));
                    m_backspaceIcon->setY(m_keysCanvas->getY() + keyInterval + i * (keyHeight + keyInterval) + std::floor((keyHeight - m_backspaceIcon->getHeight()) / 2));
                    break;
                case KEY_EXIT:
                    m_keypadExitIcon->setX(m_keysCanvas->getX() + keyInterval + j * (keyWidth + keyInterval) + std::floor((keyWidth - m_keypadExitIcon->getWidth()) / 2));
                    m_keypadExitIcon->setY(m_keysCanvas->getY() + keyInterval + i * (keyHeight + keyInterval) + std::floor((keyHeight - m_keypadExitIcon->getHeight()) / 2));
                    break;
                default:
                    std::string keyString = std::string(1, m_layout[i][j]);
                    m_keysCanvas->drawTextCenteredInRect(keyInterval + j * (keyWidth + keyInterval), keyInterval + i * (keyHeight + keyInterval), keyWidth, keyHeight, keyString, textColor, true, true, 32);
                }
                m_keysCanvas->drawRoundRect(keyInterval + j * (keyWidth + keyInterval), keyInterval + i * (keyHeight + keyInterval), keyWidth, keyHeight, 5, keyColor);
            }
        }
    }

    char Keypad::getKey(const int16_t x, const int16_t y) const
    {

        if (!isPointInTrackpad(x, y))
        {
            return KEY_NULL;
        }

        int keyHeight = std::floor((m_keysCanvas->getHeight() - (nbRow + 1) * keyInterval) / nbRow);

        // m_keysCanvas->drawRoundRect(keyInterval + i * (keyWidth + keyInterval), keyInterval + j * (keyHeight + keyInterval), keyWidth, keyHeight,
        uint8_t row;
        uint8_t column;

        row = std::floor((y - m_keysCanvas->getY()) / (keyHeight + keyInterval));

        int keyWidth = std::floor((m_keysCanvas->getWidth() - (nbCol + 1) * keyInterval) / nbCol);

        column = std::floor((x - m_keysCanvas->getX()) / (keyWidth + keyInterval));

        return m_layout[row][column];
    }

    /**
     * Execute the needed action for the key
     * @param key The key to process
     */
    void Keypad::processKey(const char key)
    {
        m_passwordToBeChecked = false;
        m_exit = false;

        switch (key)
        {
        case KEY_EXIT:
            m_buffer = m_defaultText; // Reset text
            m_exit = true;
            break;
        case KEY_CALL:
            m_exit = true;
            break;
        case KEY_BACKSPACE:
            removeChar();
            break;
        default:
            if (m_type == Keypad::KEYPAD_PASSWORD && m_buffer.length() == passwordLength)
            {
                m_passwordToBeChecked = true;
                return;
            }
            addChar(key);
            break;
        }
    }

    void Keypad::drawInputBox() const
    {
        //        m_label->setTextColor(libsystem::paxoConfig::getTextColor());
        if (m_type == Keypad::KEYPAD_PASSWORD)
        {
            int keyPasswordWidth = 16;
            int interval = std::floor((m_canvasPassword->getWidth() - keyPasswordWidth * (passwordLength)) / (passwordLength + 1));

            color_t borderColor = libsystem::paxoConfig::getBorderColor();
            color_t backgroundColor = libsystem::paxoConfig::getBackgroundColor();
            color_t fillColor = libsystem::paxoConfig::getTextColor();

            for (int i = 0; i < m_buffer.length(); i++)
            {

                m_canvasPassword->fillCircle(interval + (keyPasswordWidth + interval) * i, std::floor(m_canvasPassword->getHeight() / 2), std::floor(keyPasswordWidth / 2), fillColor);
            }
            for (int i = m_buffer.length(); i < passwordLength; i++)
            {
                m_canvasPassword->fillCircle(interval + (keyPasswordWidth + interval) * i, std::floor(m_canvasPassword->getHeight() / 2), std::floor(keyPasswordWidth / 2), backgroundColor);
                m_canvasPassword->drawCircle(interval + (keyPasswordWidth + interval) * i, std::floor(m_canvasPassword->getHeight() / 2), std::floor(keyPasswordWidth / 2), borderColor);
            }
        }
        else
        {
            m_label->setText(m_buffer);
            m_label->setCursorEnabled(true);
        }

        // Draw text
    }

    bool Keypad::passwordToBeChecked() const
    {

        return m_passwordToBeChecked;
    }

    bool Keypad::hasExitKeyBeenPressed() const
    {
        return m_exit;
    }

    void Keypad::setPasswordLength(int len)
    {
        passwordLength = len;
        drawInputBox();
    }

    void Keypad::trackpadUpdate()
    {
        int16_t rawTouchX, rawTouchY;
        graphics::getTouchPos(&rawTouchX, &rawTouchY);

        const bool wasTrackpadActive = isTrackpadActive();

        // Check if finger is on screen
        if ((rawTouchX != -1 && rawTouchY != -1) && isPointInTrackpad(originTouchX, originTouchY))
        {
            // libsystem::log("[TRACKPAD] Raw Touch : " + std::to_string(rawTouchX) + ", " + std::to_string(rawTouchY) + ".");
            // libsystem::log("[TRACKPAD] Last Touch : " + std::to_string(m_lastTouchX) + ", " + std::to_string(m_lastTouchY) + ".");
            // libsystem::log("[TRACKPAD] Origin Touch : " + std::to_string(originTouchX) + ", " + std::to_string(originTouchY) + ".");

            if (m_trackpadTicks < UINT8_MAX)
            {
                m_trackpadTicks++;
            }

            if (isTrackpadActive())
            {
                if (m_trackpadTicks == 10)
                {
                    // Do once, only when trackpad was just enabled

                    // libsystem::log("[TRACKPAD] Reset.");

                    m_trackpadActiveBox->enable();

                    m_trackpadLastDeltaX = 0;

                    localGraphicalUpdate();
                }

                const int32_t deltaX = rawTouchX - originTouchX;
                std::string deltaXString = std::to_string(deltaX);

                constexpr int32_t stepsByChar = 8;
                const int32_t toMove = (deltaX - m_trackpadLastDeltaX) / stepsByChar;

                // libsystem::log("[TRACKPAD] Delta X : " + std::to_string(deltaX) + ".");
                // libsystem::log("[TRACKPAD] To Move : " + std::to_string(toMove) + ".");

                if (toMove > 0)
                {
                    for (int i = 0; i < toMove; i++)
                    {
                        m_label->setCursorIndex(static_cast<int16_t>(m_label->getCursorIndex() + 1));
                    }
                }
                else if (toMove < 0)
                {
                    for (int i = 0; i < -toMove; i++)
                    {
                        m_label->setCursorIndex(static_cast<int16_t>(m_label->getCursorIndex() - 1));
                    }
                }

                if (abs(toMove) > 0)
                {
                    m_label->forceUpdate();
                    // m_trackpadActiveBox->forceUpdate();
                }

                m_trackpadLastDeltaX += toMove * stepsByChar;
            }
        }
        else
        {
            m_trackpadTicks = 0;

            if (wasTrackpadActive)
            {
                // Do once

                // m_trackpadFilter->disable();
                m_trackpadActiveBox->disable();

                localGraphicalUpdate();
            }
        }
    }

    bool Keypad::isPointInTrackpad(const int16_t x, const int16_t y) const
    {
        if (x < m_keysCanvas->getX() || x > m_keysCanvas->getX() + m_keysCanvas->getWidth())
            return false;
        if (y < m_keysCanvas->getY() || y > m_keysCanvas->getY() + m_keysCanvas->getHeight())
            return false;

        return true;
    }

    bool Keypad::isTrackpadActive() const
    {
        return m_trackpadTicks >= 10;
    }

    void Keypad::addChar(const char value)
    {
        if (m_type == Keypad::KEYPAD_PASSWORD)
        {
            m_buffer += value;
            return;
        }

        m_buffer.insert(m_label->getCursorIndex(), 1, value);

        // Update cursor position
        m_label->setText(m_buffer);
        m_label->setCursorIndex(m_label->getCursorIndex() + 1);
    }

    void Keypad::removeChar()
    {

        if (m_buffer.empty())
        {
            return;
        }
        if (m_type == Keypad::KEYPAD_PASSWORD)
        {
            m_buffer.pop_back();
            return;
        }
        if (m_label->getCursorIndex() <= 0)
        {
            return;
        }

        m_buffer.erase(m_label->getCursorIndex() - 1, 1);

        m_label->setCursorIndex(static_cast<int16_t>(m_label->getCursorIndex() - 1));
    }
} // gui::elements
