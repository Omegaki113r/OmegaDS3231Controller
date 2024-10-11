set( HEADER_DIRECTORIES inc )
set( SOURCES    src/OmegaDS3231Controller.cpp 
                )

idf_component_register  (   INCLUDE_DIRS    ${HEADER_DIRECTORIES}  
                            SRCS            ${SOURCES}
                            
                        )

set_property(TARGET ${COMPONENT_LIB} PROPERTY C_STANDARD 23)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)