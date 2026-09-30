#ifndef _CLI_H_
#define _CLI_H_

typedef int (*cli_fn_t)(int argc, char * argv[]);

typedef struct
{
    const char * name;
    cli_fn_t    fn;
    const char * help;
} cli_cmd_t;

void cli_feed(char c);

#endif
