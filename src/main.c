#include <ncurses.h>
#include <unistd.h>
#include "../headers/constants.h"
#include "../headers/canvas.h"
#include "../headers/snake.h"
#include "../headers/apple.h"
#include "../headers/utils.h"

void display_message(Canvas* canvas, const char* message) {
    int col = canvas->cols + 2;
    int row = (canvas->rows - 1) / 2;
    mvprintw(row, col, "%s", message);
    refresh();
}

void display_controls(Canvas* canvas) {
    display_message(canvas, "Controls: [UP, RIGHT, DOWN, LEFT] to move, q to quit");
}

int main() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    Canvas* canvas = canvas_init(5, 5);

    Position snake_position;
    canvas_get_random_free_position(canvas, snake_position);
    Snake* snake = snake_init(canvas, snake_position, DIRECTION_EAST, 3);

    Apple* apple = apple_init(canvas);

    apple_move(apple, canvas);

    canvas_render(canvas);
    display_controls(canvas);

    timeout(0);

    bool going = true;
    bool allow_speeding = false;

    while (going) {
        int total_delay = 1000;
        int step_delay = 10;
        int steps = total_delay / step_delay;

        for (int i = 0; i < steps; i++) {
            Direction current_direction = snake->direction;
            Direction previous_direction = current_direction;
            Direction new_direction = current_direction;
            bool skip_delay = false;
            int ch = getch();

            if (ch != ERR) {
                switch (ch) {
                    case KEY_UP: {
                        if (allow_speeding && current_direction == DIRECTION_NORTH) {
                            skip_delay = true;
                        } else if (current_direction != DIRECTION_SOUTH) {
                            new_direction = DIRECTION_NORTH;
                        }

                        break;
                    }
                    case KEY_RIGHT: {
                        if (allow_speeding && current_direction == DIRECTION_EAST) {
                            skip_delay = true;
                        } else if (current_direction != DIRECTION_WEST) {
                            new_direction = DIRECTION_EAST;
                        }

                        break;
                    }
                    case KEY_DOWN: {
                        if (allow_speeding && current_direction == DIRECTION_SOUTH) {
                            skip_delay = true;
                        } else if (current_direction != DIRECTION_NORTH) {
                            new_direction = DIRECTION_SOUTH;
                        }

                        break;
                    }
                    case KEY_LEFT: {
                        if (allow_speeding && current_direction == DIRECTION_WEST) {
                            skip_delay = true;
                        } else if (current_direction != DIRECTION_EAST) {
                            new_direction = DIRECTION_WEST;
                        }

                        break;
                    }
                    case 'q': {
                        skip_delay = true;
                        going = false;

                        break;
                    }
                }
            }

            if (previous_direction != new_direction) {
                snake->direction = new_direction;
                skip_delay = true;
            }

            if (skip_delay) {
                break;
            }

            msleep(step_delay);
        }

        Position next_head_position;
        get_next_node_position(snake, canvas, 0, next_head_position);

        if (positions_collide(next_head_position, apple->position)) {
            snake_grow(snake, canvas);
            apple_move(apple, canvas);
        }

        snake_move(snake, canvas);
        canvas_render(canvas);

        bool game_over = snake_self_collides(snake);
        bool game_won = canvas_count_free_positions(canvas) == 0;

        if (!game_over && !game_won) {
            display_controls(canvas);
        } else {
            if (game_over) {
                display_message(canvas, "You Failed! Press any key to exit.");
            } else {
                display_message(canvas, "You Win! Press any key to exit.");
            }

            timeout(-1);
            getch();
            going = false;
        }

        refresh();
    }

    apple_free(apple);
    snake_free(snake);
    canvas_free(canvas);

    endwin();

    return 0;
}
