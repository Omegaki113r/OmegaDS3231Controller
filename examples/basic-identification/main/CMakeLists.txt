idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES OmegaDS3231Controller driver
                    EMBED_TXTFILES developer.txt)
