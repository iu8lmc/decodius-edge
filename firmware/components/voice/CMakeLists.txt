idf_component_register(
    SRCS "voice.c"
    INCLUDE_DIRS "."
    REQUIRES esp_http_client audio
)
