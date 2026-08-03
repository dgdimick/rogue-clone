/* Optional ncurses color support. */
#include "rogue.h"

enum {
    CP_MONSTER_LOW = 1,
    CP_MONSTER_MID,
    CP_MONSTER_HIGH,
    CP_PLAYER,
    CP_OBJECT,
    CP_STAIRS,
    CP_TRAP
};

static bool colors_enabled = false;

void rogue_init_colors(void)
{
    short bg = COLOR_BLACK;

    if (!has_colors())
        return;

    start_color();
#ifdef NCURSES_VERSION
    if (use_default_colors() == OK)
        bg = -1;
#endif
    init_pair(CP_MONSTER_LOW, COLOR_RED, bg);
    init_pair(CP_MONSTER_MID, COLOR_YELLOW, bg);
    init_pair(CP_MONSTER_HIGH, COLOR_MAGENTA, bg);
    init_pair(CP_PLAYER, COLOR_CYAN, bg);
    init_pair(CP_OBJECT, COLOR_YELLOW, bg);
    init_pair(CP_STAIRS, COLOR_GREEN, bg);
    init_pair(CP_TRAP, COLOR_RED, bg);
    colors_enabled = true;
}

static int pair_for_symbol(chtype ch)
{
    unsigned char c = (unsigned char)(ch & A_CHARTEXT);

    if (c >= 'A' && c <= 'I') return CP_MONSTER_LOW;
    if (c >= 'J' && c <= 'R') return CP_MONSTER_MID;
    if (c >= 'S' && c <= 'Z') return CP_MONSTER_HIGH;

    switch (c) {
    case '@': return CP_PLAYER;
    case '%': return CP_STAIRS;
    case '^': return CP_TRAP;
    case '?': case '!': case '*': case ':': case '/':
    case ']': case ')': case '=': case ',': return CP_OBJECT;
    default: return 0;
    }
}

int rogue_draw_map_char(int row, int col, chtype ch)
{
    int pair = colors_enabled ? pair_for_symbol(ch) : 0;
    int result;

    if (pair)
        wattron(stdscr, COLOR_PAIR(pair) | (pair == CP_PLAYER ? A_BOLD : 0));

    result = mvaddch(row, col, ch);

    if (pair)
        wattroff(stdscr, COLOR_PAIR(pair) | (pair == CP_PLAYER ? A_BOLD : 0));

    return result;
}

/* Return only the displayed character, never ncurses color/attribute bits. */
short rogue_read_screen_char(int row, int col)
{
    return (short)(mvinch(row, col) & A_CHARTEXT);
}
