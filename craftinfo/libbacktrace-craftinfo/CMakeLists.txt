cmake_minimum_required( VERSION 3.13...99.99)

project( libbacktrace-craftinfo LANGUAGES NONE)


# install build script, if any into "${DEPENDENCY_DIR}/bin"

file( GLOB SCRIPTS
   bin/libbacktrace-build
   bin/libbacktrace-build.*
)

list( FILTER SCRIPTS EXCLUDE REGEX ".*[.]example")

if( SCRIPTS)
	install( FILES ${SCRIPTS} DESTINATION "bin"
	         PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
	                     GROUP_EXECUTE GROUP_READ
	                     WORLD_EXECUTE WORLD_READ
	)
endif()

# install craftinfos

file( GLOB DEFINITION_DIRS
   definition*
)

if( DEFINITION_DIRS)
	foreach( DIR ${DEFINITION_DIRS} )
	   install( DIRECTORY "${DIR}" DESTINATION "share/mulle-craft/libbacktrace"
	            USE_SOURCE_PERMISSIONS
	            PATTERN "*._DS_Store" EXCLUDE
	   )
	endforeach()
else()
   message( SEND_ERROR "Remove craftinfo \"libbacktrace\" as it is empty with:\n   mulle-sourcetree remove craftinfo/libbacktrace\n   rm -rf craftinfo/libbacktrace")
endif()
