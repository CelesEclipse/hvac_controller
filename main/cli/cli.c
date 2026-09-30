#include "cli.h"
#include "bsp/bsp_uart.h"
#include <stddef.h>
#include <string.h>

#define CLI_LINE_MAX    64
#define CLI_ARGV_MAX    4

static char     s_line[CLI_LINE_MAX];
static size_t   s_len = 0;

static int cmd_help(int argc, char *argv[]);
static int cmd_status(int argc, char *argv[]);
static const cli_cmd_t s_cmds[] = {
    {"help", cmd_help, "list commands"},
    {"status", cmd_status, "show controller state"}
};

/*
This shit is quite bizarre (AI wrote it) so I will visualize here for myself
e.g
Input: "  wifi   connect " -> meanwhile '''''w''i''f''i'''''''...'\0'
max = 4
Output: "\0\0wifi\0\0connect\0" -> tokens = 2
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

    bsp_uart_write("Temperature: 27.4 C\r\n");
    bsp_uart_write("Target: 24.0 C\r\n");
    bsp_uart_write("Fan:         65 %\r\n");
    bsp_uart_write("Mode:        COOL\r\n");
    bsp_uart_write("Alarm:       NONE\r\n");
    return 0;
}
