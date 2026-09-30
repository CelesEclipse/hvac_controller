idf_component_register(
    SRCS    "main.c"
            "drivers/led.c"
            "bsp/bsp_gpio.c"
            "bsp/bsp_timer.c"
            "bsp/bsp_uart.c"
            "cli/cli.c"
            "app/app_state.c"
    INCLUDE_DIRS ".")
