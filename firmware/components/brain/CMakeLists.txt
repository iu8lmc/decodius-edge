idf_component_register(
    SRCS "brain.c"
    INCLUDE_DIRS "."
    REQUIRES esp_http_client json esp-tls
)
