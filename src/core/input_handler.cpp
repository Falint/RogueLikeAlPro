/**
 * @file input_handler.cpp
 * @brief Implementasi InputHandler — raw terminal input cross-platform.
 */

#include "input_handler.h"

/* Cross-platform terminal input */
#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

namespace roguelike {

InputAction InputHandler::getInput() {
    char ch = readChar();

    switch (ch) {
        /* Pergerakan */
        case 'w': case 'W': return InputAction::MOVE_UP;
        case 's': case 'S': return InputAction::MOVE_DOWN;
        case 'a': case 'A': return InputAction::MOVE_LEFT;
        case 'd': case 'D': return InputAction::MOVE_RIGHT;

        /* Menu */
        case 'i': case 'I': return InputAction::OPEN_INVENTORY;
        case 'e': case 'E': return InputAction::OPEN_WEAPON_MENU;
        case 'u': case 'U': return InputAction::OPEN_UPGRADE_MENU;

        /* System */
        case 'p': case 'P': return InputAction::SAVE_GAME;
        case 'q': case 'Q': return InputAction::QUIT;
        case '\n': case '\r': return InputAction::CONFIRM;
        case 27:  return InputAction::CANCEL; // ESC key

        /* Nomor item 0-9 */
        case '0': return InputAction::USE_ITEM_0;
        case '1': return InputAction::USE_ITEM_1;
        case '2': return InputAction::USE_ITEM_2;
        case '3': return InputAction::USE_ITEM_3;
        case '4': return InputAction::USE_ITEM_4;
        case '5': return InputAction::USE_ITEM_5;
        case '6': return InputAction::USE_ITEM_6;
        case '7': return InputAction::USE_ITEM_7;
        case '8': return InputAction::USE_ITEM_8;
        case '9': return InputAction::USE_ITEM_9;

        default: return InputAction::NONE;
    }
}

Direction InputHandler::actionToDirection(InputAction action) {
    switch (action) {
        case InputAction::MOVE_UP:    return Direction::UP;
        case InputAction::MOVE_DOWN:  return Direction::DOWN;
        case InputAction::MOVE_LEFT:  return Direction::LEFT;
        case InputAction::MOVE_RIGHT: return Direction::RIGHT;
        default: return Direction::NONE;
    }
}

char InputHandler::readChar() {
#ifdef _WIN32
    return static_cast<char>(_getch());
#else
    struct termios oldSettings, newSettings;
    tcgetattr(STDIN_FILENO, &oldSettings);

    newSettings = oldSettings;
    newSettings.c_lflag &= ~static_cast<tcflag_t>(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    char ch = '\0';
    if (read(STDIN_FILENO, &ch, 1) < 0) {
        ch = '\0';
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);

    return ch;
#endif
}

} // namespace roguelike
