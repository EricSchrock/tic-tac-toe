#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE (3)

// https://man7.org/linux/man-pages/man5/terminal-colors.d.5.html
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"

typedef enum {
    STATE_X,
    STATE_O,
    STATE_X_WINS,
    STATE_O_WINS,
    STATE_TIE,
} STATE_E;

typedef enum {
    TILE_BLANK,
    TILE_X,
    TILE_O,
} TILE_E;

static STATE_E state = STATE_X;
static TILE_E board[BOARD_SIZE][BOARD_SIZE];

static void process_input(int* col, int* row);

static void update_state(int col, int row);
static bool check_win_conditions(void);
static bool check_tie_conditions(void);

static void render(void);
static void render_board(void);
static void render_state(void);

int main()
{
    render(); // Initial render so players can see the board on their first play

    while ((state == STATE_X) || (state == STATE_O)) {
        int col, row;

        process_input(&col, &row);
        update_state(col, row);
        render();
    }

    return EXIT_SUCCESS;
}

static void process_input(int* col, int* row)
{
    int slot;

    while (true) {
        printf("%s's turn\n", state == STATE_X ? "X" : "O");
        printf("Enter slot to place: ");
        int rc = scanf("%d", &slot);
        if (rc != 1) {
            printf("Slot must be a number\n");
            char c = getchar(); // Consume buffer
            continue;
        }

        if (slot <= 0) {
            printf("Slot must be greater than 0\n");
            continue;
        } else if (slot > (BOARD_SIZE * BOARD_SIZE)) {
            printf("Slot must be smaller than %d\n", (BOARD_SIZE * BOARD_SIZE) + 1);
            continue;
        }

        *col = (slot - 1) / BOARD_SIZE;
        *row = (slot - 1) % BOARD_SIZE;

        if (board[*col][*row] != TILE_BLANK) {
            printf("Slot must be empty\n");
            continue;
        }

        break;
    }
}

static void update_state(int col, int row)
{
    board[col][row] = state == STATE_X ? TILE_X : TILE_O;

    if (check_win_conditions()) {
        state = (state == STATE_X) ? STATE_X_WINS : STATE_O_WINS;
        return;
    }

    if (check_tie_conditions()) {
        state = STATE_TIE;
        return;
    }

    state = state ^ 1; // Toggle between STATE_X and STATE_O with XOR
}

static bool check_win_conditions(void)
{
    TILE_E check = state == STATE_X ? TILE_X : TILE_O;

    // Check rows
    bool row;
    for (int i = 0; i < BOARD_SIZE; i++) {
        row = true;

        for (int j = 0; j < BOARD_SIZE; j++){
            if (board[i][j] != check) {
                row = false;
            }
        }

        if (row) {
            break;
        }
    }

    // Check columns
    bool column;
    for (int j = 0; j < BOARD_SIZE; j++) {
        column = true;

        for (int i = 0; i < BOARD_SIZE; i++){
            if (board[i][j] != check) {
                column = false;
            }
        }

        if (column) {
            break;
        }
    }

    // Check diagonals
    bool right_sloping = true;
    bool left_sloping = true;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][i] != check) {
            right_sloping = false;
        }

        if (board[i][BOARD_SIZE - 1 - i] != check) {
            left_sloping = false;
        }
    }

    return row || column || right_sloping || left_sloping;
}

static bool check_tie_conditions(void)
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == TILE_BLANK) {
                return false;
            }
        }
    }

    return true;
}

static void render(void)
{
    render_board();
    render_state();
}

static void render_board(void)
{
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == TILE_BLANK) {
                printf(" %d ", (j + 1) + (i * BOARD_SIZE));
            } else if (board[i][j] == TILE_X) {
                printf(ANSI_COLOR_RED " x " ANSI_COLOR_RESET);
            } else if (board[i][j] == TILE_O) {
                printf(ANSI_COLOR_GREEN " o " ANSI_COLOR_RESET);
            }

            if (j < (BOARD_SIZE - 1)) {
                printf("|");
            }
        }

        printf("\n");

        if (i < (BOARD_SIZE - 1)) {
            for (int j = 0; j < (BOARD_SIZE * 4) - 1; j ++) {
                printf("-");
            }

            printf("\n");
        }
    }

    printf("\n");
}

static void render_state(void)
{
    if (state == STATE_X_WINS) {
        printf("X wins!\n\n");
    } else if (state == STATE_O_WINS) {
        printf("O wins!\n\n");
    } else if (state == STATE_TIE) {
        printf("Nobody wins!\n\n");
    }
}
