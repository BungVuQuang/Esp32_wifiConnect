set(pri_req led_strip)
idf_component_register(SRCS "ws2812b.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})