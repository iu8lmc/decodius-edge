idf_component_register(
    SRCS "cluster.c"
    INCLUDE_DIRS "."
    REQUIRES esp_http_client json
)
