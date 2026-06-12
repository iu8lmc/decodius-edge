idf_component_register(
    SRCS
        "main.c"
        "app_decodius.c"
    INCLUDE_DIRS "."
    REQUIRES
        ui_lvgl net brain decodium voice audio wakeword hamqth cluster store
        nvs_flash esp_timer
)
