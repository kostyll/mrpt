/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2014, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */
#ifndef CObservationReflectivity_H
#define CObservationReflectivity_H

#include <mrpt/slam/CObservation.h>
#include <mrpt/poses/CPose3D.h>

namespace mrpt
{
namespace slam
{
	DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE_LINKAGE( CObservationReflectivity , CObservation,OBS_IMPEXP )

	/** Declares a class derived from "CObservation" that encapsules a single short-range reflectivity measurement. 
	 *    This can be used for example to store readings from IR sensors (Lego Mindstorm NXT, etc...).
	 *
	 * \sa mrpt::slam::CReflectivityGridMap2D, CObservation
	 * \ingroup mrpt_obs_grp
	 */
	class OBS_IMPEXP CObservationReflectivity : public CObservation
	{
		// This must be added to any CSerializable derived class:
		DEFINE_SERIALIZABLE( CObservationReflectivity )

	 public:
		CObservationReflectivity( );	//!< Default constructor.
		virtual ~CObservationReflectivity(); 

		/** The read reflectivity level, in the range [0,1] (0=black, 1=white).
		  */
		float reflectivityLevel;

		/** The pose of this sensor in robot's local coordinates.
		  */
		CPose3D sensorPose;

		/** 1-sigma of the sensor Gaussian noise (in the same normalized units than \a reflectivityLevel)
		  */
		float  sensorStdNoise;
		
		// See base class docs
		void getSensorPose( CPose3D &out_sensorPose ) const { out_sensorPose = sensorPose; }
		// See base class docs
		void setSensorPose( const CPose3D &newSensorPose ) { sensorPose = newSensorPose; }
		// See base class docs
		virtual void getDescriptionAsText(std::ostream &o) const;

	}; // End of class def.
	DEFINE_SERIALIZABLE_POST_CUSTOM_BASE_LINKAGE( CObservationReflectivity , CObservation,OBS_IMPEXP )

	} // End of namespace
} // End of namespace

#endif
