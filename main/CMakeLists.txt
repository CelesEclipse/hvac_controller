idf_component_register(
    SRCS    "main.c"
            "drivers/led.c"
            "bsp/bsp_gpio.c"
            "bsp/bsp_timer.c"
            "bsp/bsp_uart.c"
            "cli/cli.c"
            "app/app_state.c"
            "drivers/temp_sensor_mock.c"
            "services/control_services.c"
    INCLUDE_DIRS ".")
