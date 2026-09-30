#include "cli.h"
#include "bsp/bsp_uart.h"
#include "app/app_state.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define CLI_LINE_MAX    64
#define CLI_ARGV_MAX    4
#define OUT_BUF_SIZE    48

static char     s_line[CLI_LINE_MAX];
static size_t   s_len = 0;

static int cmd_help(int argc, char *argv[]);
static int cmd_status(int argc, char *argv[]);
static int cmd_fan(int argc, char *argv[]);
static const cli_cmd_t s_cmds[] = {
    {"help", cmd_help, "list commands"},
    {"status", cmd_status, "show controller state"},
    {"fan", cmd_fan, "set fan speed"}
};

static const char *const MODE_STR[] = { "OFF", "COOL", "HEAT", "FAN" };

/*
e.g
Input: "  wifi   connect " -> meanwhile '''''w''i''f''i'''''''...'\0'
max = 4
Output: "\0\0wifi\0\0\0connect\0" -> tokens = 3
*/
static int tokenize(char * line, char * argv[], int max)
{
    int argc = 0;
    char * p = line;
    while (*p && argc < max) {
        while (*p == ' ') *p++ = '\0';
        if (*p == '\0') break;
        argv[argc++] = p;
        while (*p && *p != ' ') p++;
    }
    return argc;
}

static void cli_execute(char * line)
{
    char * argv[CLI_ARGV_MAX];
    int argc = tokenize(line, argv, CLI_ARGV_MAX);
    if (argc == 0) return;

    for (size_t i = 0; i < sizeof(s_cmds) / sizeof(s_cmds[0]); ++i) {
        if (strcmp(argv[0], s_cmds[i].name) == 0) {
            s_cmds[i].fn(argc, argv);
            return;
        }
    }
    bsp_uart_write("Unknown command, try 'help'\r\n");
}

void cli_feed(char c)
{
    if (c == '\r' || c == '\n') {
        if (s_len == 0) return;
        s_line[s_len] = '\0';
        bsp_uart_write("\r\n");
        cli_execute(s_line);
        s_len = 0;
        bsp_uart_write("> ");
    } else if (c == 0x08 || c == 0x7F) {    // backspace / delete
        if (s_len > 0) { s_len--; bsp_uart_write("\b \b"); }
    } else if (c >= 0x20 && c < 0x7F) {     // printable ASCII only
        if (s_len < CLI_LINE_MAX - 1) {     // leave room for '\0'
            s_line[s_len++] = c;
            char echo[2] = { c, '\0' };
            bsp_uart_write(echo);
        }
    }
}

static int cmd_help(int argc, char *argv[])
{
    (void)argc; (void)argv;
    for (size_t i = 0; i < sizeof(s_cmds) / sizeof(s_cmds[0]); ++i) {
        bsp_uart_write(s_cmds[i].name);
        bsp_uart_write(" - ");
        bsp_uart_write(s_cmds[i].help);
        bsp_uart_write("\r\n");
    }
    return 0;
}

static int cmd_status(int argc, char *argv[])
{
    (void)argv;
    if (argc != 1) {
        bsp_uart_write("usage: status\r\n");
        return -1;
    }

    const hvac_state_t * s = app_state_get();
    char buf[OUT_BUF_SIZE];

    snprintf(buf, sizeof(buf), "Temperature: %d.%d C\r\n", s->temp_x10 / 10, s->temp_x10 % 10);
    bsp_uart_write(buf);
    snprintf(buf, sizeof buf, "Target:      %d.%d C\r\n", s->target_x10 / 10, s->target_x10 % 10);
    bsp_uart_write(buf);
    snprintf(buf, sizeof buf, "Fan:         %u %%\r\n", s->fan_percent);
    bsp_uart_write(buf);
    snprintf(buf, sizeof buf, "Mode:        %s\r\n", MODE_STR[s->mode]);
    bsp_uart_write(buf);

    return 0;
}

static int cmd_fan(int argc, char *argv[])
{
    if (argc != 2) {
        bsp_uart_write("usage: fan <0-100>\r\n");
        return -1;
    }

    char * end;
    long v = strtol(argv[1], &end, 10);
    if (end == argv[1] || *end != 0) {
        bsp_uart_write("fan: invalid number\r\n");
        return -1;
    }
    if (v < 0 || v > 100 || !app_state_set_fan((uint8_t)v)) {
        bsp_uart_write("fan: must be 0-100\r\n");
        return -1;
    }
    bsp_uart_write("OK\r\n");
    return 0;
}
