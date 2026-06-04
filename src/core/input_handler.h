/**
 * @file input_handler.h
 * @brief Penanganan input keyboard non-blocking.
 *
 * Cross-platform: menggunakan termios pada Linux dan conio.h pada Windows.
 * Input dibaca tanpa buffering (raw mode) sehingga tidak perlu tekan Enter.
 */

#pragma once

#include "../utility/types.h"

namespace roguelike {

/**
 * @brief Tipe aksi yang bisa dilakukan pemain.
 */
enum class InputAction {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    OPEN_INVENTORY,
    OPEN_WEAPON_MENU,
    OPEN_UPGRADE_MENU,
    SAVE_GAME,
    QUIT,
    USE_ITEM_0, USE_ITEM_1, USE_ITEM_2, USE_ITEM_3, USE_ITEM_4,
    USE_ITEM_5, USE_ITEM_6, USE_ITEM_7, USE_ITEM_8, USE_ITEM_9,
    CONFIRM,
    CANCEL,
    NONE
};

/**
 * @brief Membaca dan mengkonversi input keyboard ke InputAction.
 */
class InputHandler {
public:
    /**
     * @brief Membaca satu karakter dari terminal (blocking).
     * @return InputAction yang sesuai dengan tombol yang ditekan.
     */
    static InputAction getInput();

    /**
     * @brief Mengkonversi InputAction ke Direction (untuk pergerakan).
     * @return Direction::NONE jika bukan aksi pergerakan.
     */
    static Direction actionToDirection(InputAction action);

private:
    /**
     * @brief Membaca satu karakter dari terminal tanpa echo.
     */
    static char readChar();
};

} // namespace roguelike
