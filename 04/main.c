#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_NOF_NUMBERS (100)
#define MAX_NOF_BOARDS (100)

static uint32_t numbers[MAX_NOF_NUMBERS];
static uint32_t boards[5][5][MAX_NOF_BOARDS];
static bool drawn[5][5][MAX_NOF_BOARDS];
static uint32_t nof_numbers;
static uint32_t nof_boards;
static uint32_t count;

static void _parse_line(char *line)
{
    if (count == 0) {
        char *token = NULL;
        token = strtok(line, ",");

        while (token) {
            assert((nof_numbers < MAX_NOF_NUMBERS) && "Too many numbers");
            numbers[nof_numbers] = atoi(token);
            nof_numbers++;
            token = strtok(NULL, ",");
        }
    } else if (line[0]) {
        assert((nof_boards < MAX_NOF_BOARDS) && "Too many boards");
        static uint32_t board_line = 0;
        char *p = line;
        uint32_t board_number = 0;

        while (*p) {
            if (isdigit(*p)) {
                boards[board_line][board_number][nof_boards] = (uint32_t) strtol(p, &p, 10);
                board_number++;
            } else {
                p++;
            }
        }

        board_line++;

        if (board_line == 5) {
            board_line = 0;
            nof_boards++;
        }
    }

    count++;
}

static void _parse_input(void)
{
    char *data_read = NULL;
    size_t len;
    ssize_t read;
    FILE *fp;

    if (!(fp = fopen("input.txt", "r"))) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&data_read, &len, fp)) != -1) {
        data_read[strcspn(data_read, "\r\n")] = '\0';
        _parse_line(data_read);
    }

    free(data_read);
    fclose(fp);
}

static void _draw_number(uint32_t number)
{
    for (uint32_t board = 0; board < nof_boards; board++) {
        for (uint32_t y = 0; y < 5; y++) {
            for (uint32_t x = 0; x < 5; x++) {
                if (boards[y][x][board] == number) {
                    drawn[y][x][board] = true;
                }
            }
        }
    }
}

static uint32_t _get_unmarked_sum(uint32_t board_idx)
{
    uint32_t unmarked_sum = 0;

    for (uint32_t y = 0; y < 5; y++) {
        for (uint32_t x = 0; x < 5; x++) {
            if (!drawn[y][x][board_idx]) {
                unmarked_sum += boards[y][x][board_idx];
            }
        }
    }

    return unmarked_sum;
}

static bool _winner_found(uint32_t *winner_idx)
{
    for (uint32_t board = 0; board < nof_boards; board++) {
        /* Horizontal check. */
        for (uint32_t y = 0; y < 5; y++) {
            if (drawn[y][0][board] && drawn[y][1][board] && drawn[y][2][board] && drawn[y][3][board] &&
                drawn[y][4][board]) {
                *winner_idx = board;
                return true;
            }
        }

        /* Vertical check. */
        for (uint32_t x = 0; x < 5; x++) {
            if (drawn[0][x][board] && drawn[1][x][board] && drawn[2][x][board] && drawn[3][x][board] &&
                drawn[4][x][board]) {
                *winner_idx = board;
                return true;
            }
        }
    }

    return false;
}

static bool _loser_found(uint32_t *loser_idx)
{
    uint32_t nof_winners = 0;

    for (uint32_t board = 0; board < nof_boards; board++) {
        bool winner = false;

        /* Horizontal check. */
        for (uint32_t y = 0; y < 5; y++) {
            if (drawn[y][0][board] && drawn[y][1][board] && drawn[y][2][board] && drawn[y][3][board] &&
                drawn[y][4][board]) {
                winner = true;
                break;
            }
        }

        /* Vertical check. */
        if (!winner) {
            for (uint32_t x = 0; x < 5; x++) {
                if (drawn[0][x][board] && drawn[1][x][board] && drawn[2][x][board] && drawn[3][x][board] &&
                    drawn[4][x][board]) {
                    winner = true;
                    break;
                }
            }
        }

        if (winner) {
            nof_winners++;
        } else {
            *loser_idx = board;
        }
    }

    return (nof_winners == nof_boards);
}

int main(void)
{
    _parse_input();

    uint32_t draw_number_idx = 0;

    /* Find winner. */
    uint32_t winner_idx;
    while (!_winner_found(&winner_idx)) {
        _draw_number(numbers[draw_number_idx++]);
    }

    uint32_t unmarked_sum_winner = _get_unmarked_sum(winner_idx);
    printf("ans1 = %u\n", unmarked_sum_winner * numbers[draw_number_idx - 1]);

    /* Find loser. */
    uint32_t loser_idx;
    while (!_loser_found(&loser_idx)) {
        _draw_number(numbers[draw_number_idx++]);
    }

    uint32_t unmarked_sum_loser = _get_unmarked_sum(loser_idx);
    printf("ans2 = %u\n", unmarked_sum_loser * numbers[draw_number_idx - 1]);

    return EXIT_SUCCESS;
}