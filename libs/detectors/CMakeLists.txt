#---------------------------------------------
# Macro declared in "DeclareMRPTLib.cmake":
#---------------------------------------------
define_mrpt_lib(
	# Lib name
	detectors
	# Deps:
	mrpt-vision
	mrpt-maps
	mrpt-gui
	mrpt-slam
	# And deps of the deps:
	mrpt-base
	mrpt-opengl
	mrpt-obs	
	)

IF(BUILD_mrpt-detectors) 
	IF(CMAKE_MRPT_HAS_OPENCV)
		target_link_libraries(mrpt-detectors ${OpenCV_LIBRARIES})
	ENDIF(CMAKE_MRPT_HAS_OPENCV)

ENDIF(BUILD_mrpt-detectors) 

