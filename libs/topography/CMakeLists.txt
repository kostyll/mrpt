#---------------------------------------------
# Macro declared in "DeclareMRPTLib.cmake":
#---------------------------------------------
define_mrpt_lib(
	# Lib name
	topography
	# Dependencies
	mrpt-base
	mrpt-obs
	mrpt-opengl
	mrpt-scanmatching
	)

IF(BUILD_mrpt-topography) 
	IF(CMAKE_MRPT_HAS_WXWIDGETS)
		target_link_libraries(mrpt-topography ${wxWidgets_LIBRARIES})
	ENDIF(CMAKE_MRPT_HAS_WXWIDGETS)
ENDIF(BUILD_mrpt-topography) 

