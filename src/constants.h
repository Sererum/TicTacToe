#pragma once

enum Mark {
    None = 0, Zero = 1, Cross = 2
};

#define WIN_LEN 5
#define SIZE_CELL 40
#define GET_NEXT_MARK(mark) ((mark == Zero) ? Cross : Zero)
#define BACK_COLOR QColor(25, 3, 39)
#define FOREGROUND_COLOR QColor(2, 222, 252)
#define BOARD_COLOR QColor(222, 111, 222)

#define SIZE_MENU_FIGS 150