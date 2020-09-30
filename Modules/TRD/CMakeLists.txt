# ---- Library ----

add_library(QcTRD)

target_sources(QcTRD PRIVATE src/SimpleTrdTask.cxx
			                       src/SimpleTrdCheck.cxx
			                       src/SimpleTrdPostProcessing.cxx
														 src/RawDataQcPostProccessing.cxx
														 src/RawDataQcCheck.cxx
														 src/RawDataQcTask.cxx)

target_include_directories(
  QcTRD
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
					${CMAKE_CURRENT_SOURCE_DIR}/src
					${O2_ROOT}/include/GPU)

target_link_libraries(QcTRD
											PUBLIC
												QualityControl)


add_root_dictionary(QcTRD
                    HEADERS include/TRD/SimpleTrdTask.h
                            include/TRD/SimpleTrdCheck.h
                            include/TRD/SimpleTrdPostProcessing.h
														include/TRD/RawDataQcTask.h
				                    include/TRD/RawDataQcCheck.h
				                    include/TRD/RawDataQcPostProccessing.h
                    LINKDEF include/TRD/LinkDef.h
                    BASENAME QcTRD)

# ---- Test(s) ----

set(TEST_SRCS test/testQcTRD.cxx)

foreach(test ${TEST_SRCS})
  get_filename_component(test_name ${test} NAME)
  string(REGEX REPLACE ".cxx" "" test_name ${test_name})

  add_executable(${test_name} ${test})
  target_link_libraries(${test_name}
                        PUBLIC QcTRD
                        PRIVATE QcTRD Boost::unit_test_framework)
  add_test(NAME ${test_name} COMMAND ${test_name})
  #set_property(TARGET ${test_name}

  set_tests_properties(${test_name} PROPERTIES TIMEOUT 20)
endforeach()


# ---- Executables ----

set(EXE_SRCS src/runTRD.cxx
						 src/runTRDRawReader.cxx)

set(EXE_NAMES
  o2-qc-run-trd
	o2-qc-run-trdrawreader)

list(LENGTH EXE_SRCS count)
math(EXPR count "${count}-1")
foreach(i RANGE ${count})
  list(GET EXE_SRCS ${i} src)
  list(GET EXE_NAMES ${i} name)
  add_executable(${name} ${src})
  target_link_libraries(${name} PRIVATE QcTRD ROOT::Tree)
endforeach()

# ---- Install ----

install(TARGETS QcTRD ${EXE_NAMES}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# ---- Install config files ----

install(FILES SimpleTrd.json
							TRDQCConf.json
              DESTINATION etc)
