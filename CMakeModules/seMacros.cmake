MACRO(LINK_INTERNAL TRGTNAME)
    FOREACH(LINKLIB ${ARGN})
        IF(MSVC AND OSG_MSVC_VERSIONED_DLL)
            #when using versioned names, the .dll name differ from .lib name, there is a problem with that:
            #CMake 2.4.7, at least seem to use PREFIX instead of IMPORT_PREFIX  for computing linkage info to use into projects,
            # so we full path name to specify linkage, this prevent automatic inferencing of dependencies, so we add explicit depemdencies
            #to library targets used
        IF(NOT MSVC_IDE)
            TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${OUTPUT_LIBDIR}/${LINKLIB}.lib" debug "${OUTPUT_LIBDIR}/${LINKLIB}${CMAKE_DEBUG_POSTFIX}.lib")
        ELSE(NOT MSVC_IDE)
            TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${OUTPUT_LIBDIR}/${LINKLIB}" debug "${OUTPUT_LIBDIR}/${LINKLIB}${CMAKE_DEBUG_POSTFIX}")
        ENDIF(NOT MSVC_IDE)
            ADD_DEPENDENCIES(${TRGTNAME} ${LINKLIB})
        ELSE(MSVC AND OSG_MSVC_VERSIONED_DLL)
            TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${LINKLIB}" debug "${LINKLIB}${CMAKE_DEBUG_POSTFIX}")
        ENDIF(MSVC AND OSG_MSVC_VERSIONED_DLL)
    ENDFOREACH(LINKLIB)
ENDMACRO(LINK_INTERNAL TRGTNAME)
