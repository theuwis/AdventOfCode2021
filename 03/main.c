#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_NOF_ENTRIES (1000)

static char input[MAX_NOF_ENTRIES][13];
static uint32_t count;

static void _parse_line(char *line)
{
    assert((count < MAX_NOF_ENTRIES) && "Too many entries in input");
    strncpy(input[count], line, sizeof(input[0]));
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

/* Returns -1 if equal, 0 if more zero, 1 if more 1 */
static int8_t _get_most_common(uint8_t pos, char list[][13], size_t list_len)
{
    uint32_t ones_count = 0;
    uint32_t zeroes_count = 0;

    for (uint32_t i = 0; i < list_len; i++) {
        if (list[i][pos] == '1') {
            ones_count++;
        } else {
            zeroes_count++;
        }
    }

    if (ones_count > zeroes_count) {
        return 1;
    }

    if (ones_count < zeroes_count) {
        return 0;
    }

    return -1;
}

static void _update_list(uint8_t pos, char most_common, char list[][13], size_t *list_len)
{
    size_t new_len = 0;

    for (uint32_t i = 0; i < *list_len; i++) {
        if (list[i][pos] == most_common) {
            memcpy(list[new_len], list[i], 13);
            new_len++;
        }
    }

    *list_len = new_len;
}

int main(void)
{
    _parse_input();

    size_t line_len = strlen(input[0]);
    uint32_t count_ones[line_len];
    memset(count_ones, 0, line_len * sizeof(count_ones[0]));

    for (uint32_t i = 0; i < count; i++) {
        for (uint32_t j = 0; j < line_len; j++) {
            if (input[i][j] == '1') {
                count_ones[j]++;
            }
        }
    }

    uint32_t gamma = 0;
    uint32_t epsilon = 0;

    for (uint32_t i = 0; i < line_len; i++) {
        if (count_ones[i] > (count / 2)) {
            gamma += (1 << (line_len - i - 1));
        } else {
            epsilon += (1 << (line_len - i - 1));
        }
    }

    printf("ans1 = %u\n", gamma * epsilon);

    char oxy_list[count][13];
    memcpy(oxy_list, input, sizeof(oxy_list));
    size_t oxy_len = count;

    uint8_t oxy_pos = 0;
    int8_t most_common;
    char most_common_char;
    do {
        most_common = _get_most_common(oxy_pos, oxy_list, oxy_len);

        if (most_common == -1) {
            most_common_char = '1';
        } else if (most_common == 0) {
            most_common_char = '0';
        } else if (most_common == 1) {
            most_common_char = '1';
        }

        _update_list(oxy_pos, most_common_char, oxy_list, &oxy_len);

        oxy_pos++;
    } while (oxy_len > 1);

    char co2_list[count][13];
    memcpy(co2_list, input, sizeof(co2_list));
    size_t co2_len = count;

    uint8_t co2_pos = 0;
    char least_common_char;
    do {
        most_common = _get_most_common(co2_pos, co2_list, co2_len);

        if (most_common == -1) {
            least_common_char = '0';
        } else if (most_common == 0) {
            least_common_char = '1';
        } else if (most_common == 1) {
            least_common_char = '0';
        }

        _update_list(co2_pos, least_common_char, co2_list, &co2_len);

        co2_pos++;
    } while (co2_len > 1);

    uint32_t oxy_rating = (uint32_t) strtol(oxy_list[0], NULL, 2);
    uint32_t co2_rating = (uint32_t) strtol(co2_list[0], NULL, 2);
    printf("ans2 = %u", oxy_rating * co2_rating);

    return EXIT_SUCCESS;
}