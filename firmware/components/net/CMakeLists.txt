idf_component_register(
    SRCS "net.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi_remote esp_netif esp-tls esp_event
)
