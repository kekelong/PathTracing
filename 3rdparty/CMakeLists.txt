set(third_party_folder "ThirdParty")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if(NOT TARGET glm)
    add_subdirectory(glm)
    set_target_properties(glm PROPERTIES FOLDER ${third_party_folder}/glm)
endif()

