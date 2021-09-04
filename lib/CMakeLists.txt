



# core will always be used no matter what -> include sources from it always

# have a flag option for EM vs ES simulation??



# add_subdirectory(core)

set(CORE_SRC_FILES 
    src/core/Output.cpp
    src/core/Solver.cpp
    src/core/Sources.cpp
    src/core/World.cpp
)

set(CORE_HEADER_FILES
    include/core/constants.hpp
    include/core/Field.h
    include/core/Input.hpp
    include/core/Output.hpp
    include/core/Solver.h
    include/core/Sources.h
    include/core/Utils.hpp
    include/core/World.h
)

set(ES_SRC_FILES
    src/core/es_pic/main.cpp
)
# set(ES_HEADER_FILES
#     include/core/es_pic/main.hpp
# )

# set(EM_SRC_FILES
#     main.cpp
# )
# set(EM_HEADER_FILES
#     main.hpp
# )


add_library(core)

target_sources(core
    PRIVATE ${CORE_SRC_FILES}
    PUBLIC  ${CORE_HEADER_FILES}
)

target_include_directories(core PUBLIC include)
target_compile_features(core PUBLIC cxx_std_17)







