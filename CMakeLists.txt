#########################################################################################################
# Package: RooLagrangianMorphing #############################################################################

cmake_minimum_required(VERSION 2.8)
project( RooLagrangianMorphing )

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(HAS_PARENT 0)
else()
  set(HAS_PARENT 1)
endif()
  
set(CMAKE_BUILD_TYPE Debug)

IF(${HAS_PARENT})
  message("announcing RooLagrangianMorphing")
  set(HAS_RooLagrangianMorphing 1 PARENT_SCOPE)
  set(ROOFITUTILS_DIR ${PROJECT_SOURCE_DIR} PARENT_SCOPE)
ENDIF()

find_package( AnalysisBase QUIET) 

IF(${AnalysisBase_FOUND})
  # Set up the usage of CTest:
  IF(NOT ${HAS_PARENT})
    atlas_ctest_setup() # Set up the project: 
    atlas_project( RooLagrangianMorphing 1.0.0 
      USE AnalysisBase ${AnalysisBase_VERSION} ) 

    # Generate an environment setup script: 
    lcg_generate_env( SH_FILE ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh )
    install( FILES ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh    DESTINATION . ) 
    
    # Set up the usage of CPack: 
    set(CMAKE_INSTALL_PREFIX /InstallArea/x86_64-slc6-gcc49-opt)
    atlas_cpack_setup()
  ENDIF()

  # register this as a package to ASG
  atlas_subdir( RooLagrangianMorphing )
ENDIF()

find_package( ROOT REQUIRED COMPONENTS Core RIO MathCore Matrix HistFactory RooFitCore RooFit Minuit Hist RooStats )
FOREACH(incfile ${ROOT_USE_FILE})
  include(${incfile})
ENDFOREACH()  

if(${BOOST})
  find_package( Boost 1.55 )
  if(${Boost_FOUND})
    set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS} -DUSE_UBLAS=1")
    set(CMAKE_DICT_FLAGS "-DUSE_UBLAS=1")
    include_directories ("${Boost_INCLUDE_DIRS}")
    message("using BOOST ${BOOST_VERSION}")
  else()
    message("did not find suitable BOOST version, using standard double precision")
  endif()
else()
  message("not using BOOST")
endif()


# set some variables for easier handling
set(RooLagrangianMorphingLinkDef ${PROJECT_SOURCE_DIR}/Root/LinkDef.h)
file(GLOB RooLagrangianMorphingSources Root/[A-Z]*.cxx)
file(GLOB RooLagrangianMorphingHeaders RooLagrangianMorphing/[A-Z]*.h)
file(GLOB Tests "test/*.sh")

set(SETUP setup.sh)
file(WRITE ${SETUP} "#!/bin/bash\n")
file(APPEND ${SETUP} "# this is an auto-generated setup script\n" )

if(${AnalysisBase_FOUND})
  # this section reflects the standard ASG way of configuring CMake
  # it is executed when compiling within an ASG environment
  find_package( GTest )
  set(CMAKE_INSTALL_PREFIX /InstallArea/x86_64-slc6-gcc62-opt)
  add_definitions( ${CMAKE_DICT_FLAGS} )

  atlas_add_root_dictionary( RooLagrangianMorphing RooLagrangianMorphingCintDict
    ROOT_HEADERS ${RooLagrangianMorphingHeaders} ${RooLagrangianMorphingLinkDef}
    EXTERNAL_PACKAGES ROOT
  )
  atlas_add_library( RooLagrangianMorphing
    ${RooLagrangianMorphingHeaders} ${RooLagrangianMorphingSources} ${RooLagrangianMorphingCintDict}
    PUBLIC_HEADERS RooLagrangianMorphing
    PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} 
    PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES}
  )

  atlas_platform_id( BINARY_TAG )

  # Add all targets to the build-tree export set
  export(TARGETS RooLagrangianMorphing RooLagrangianMorphingPkg FILE "${PROJECT_BINARY_DIR}/RooLagrangianMorphingTargets.cmake")

  file(APPEND ${SETUP} "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/${BINARY_TAG}/lib\n")
  file(APPEND ${SETUP} "export DYLD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/${BINARY_TAG}/lib\n")
  file(APPEND ${SETUP} "export ROOT_INCLUDE_PATH=\${ROOT_INCLUDE_PATH}:${CMAKE_BINARY_DIR}/${BINARY_TAG}/include\n")
  file(APPEND ${SETUP} "export ROOLAGRANGIANMORPHING_SRC=${CMAKE_CURRENT_SOURCE_DIR}\n")
  file(APPEND ${SETUP} "export ROOLAGRANGIANMORPHING_BIN=${CMAKE_BINARY_DIR}/${BINARY_TAG}/lib\n")

  foreach(TestScript ${Tests})
    get_filename_component(TestName ${TestScript} NAME_WE)
    add_test( NAME ${TestName} COMMAND bash ${TestScript} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/tutorial )    
  endforeach()
  
ELSE()
  # this section is a standalone compilation segment
  # it is executed when compiling outside an ASG environment

  # register all the files and directories
  include_directories ("${PROJECT_SOURCE_DIR}" "${ROOT_INCLUDE_DIRS}")

  # generate the dictionary source code
  ROOT_GENERATE_DICTIONARY(G__RooLagrangianMorphing ${RooLagrangianMorphingHeaders} LINKDEF ${RooLagrangianMorphingLinkDef} OPTIONS ${CMAKE_DICT_FLAGS})

  # register the shared object to include both sources and dictionaries
  add_library( RooLagrangianMorphing SHARED ${RooLagrangianMorphingSources} G__RooLagrangianMorphing.cxx)

  # link everything together at the end
  target_link_libraries( RooLagrangianMorphing ${ROOT_LIBRARIES} )

  # Add all targets to the build-tree export set
  export(TARGETS RooLagrangianMorphing FILE "${PROJECT_BINARY_DIR}/RooLagrangianMorphingTargets.cmake")

  file(APPEND ${SETUP} "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${CMAKE_CURRENT_BINARY_DIR}\n")
  file(APPEND ${SETUP} "export ROOLAGRANGIANMORPHING_SRC=${CMAKE_CURRENT_SOURCE_DIR}\n")
  file(APPEND ${SETUP} "export ROOLAGRANGIANMORPHING_BIN=${CMAKE_CURRENT_BINARY_DIR}\n")
  file(APPEND ${SETUP} "export PATH=\${PATH}:${CMAKE_SOURCE_DIR}/scripts\n")

  include(CTest)
  enable_testing()

  foreach(TestScript ${Tests})
    get_filename_component(TestName ${TestScript} NAME_WE)
    add_test( NAME ${TestName} COMMAND bash ${TestScript} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/tutorial )
  endforeach()
  
ENDIF()

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE RooLagrangianMorphing)

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/RooLagrangianMorphing" )
set(CONF_LIBRARY_DIRS "${PROJECT_BINARY_DIR}")
set(CONF_LIBRARIES    RooLagrangianMorphing)
configure_file(RooLagrangianMorphingConfig.cmake.in
  "${PROJECT_BINARY_DIR}/RooLagrangianMorphingConfig.cmake" @ONLY)

# Install the RooLagrangianMorphingConfig.cmake and RooLagrangianMorphingConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/RooLagrangianMorphingConfig.cmake"
  "${PROJECT_BINARY_DIR}/RooLagrangianMorphingConfigVersion.cmake"
  DESTINATION "${PROJECT_SOURCE_DIR}" COMPONENT dev)

# general post-compile installation of the package 
# required for other packages to discover this one via CMake
install( 
  TARGETS RooLagrangianMorphing
  EXPORT RooLagrangianMorphingConfig
  DESTINATION cmake
  )
