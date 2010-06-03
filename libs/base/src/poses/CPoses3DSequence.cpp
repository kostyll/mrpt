/* +---------------------------------------------------------------------------+
   |          The Mobile Robot Programming Toolkit (MRPT) C++ library          |
   |                                                                           |
   |                   http://mrpt.sourceforge.net/                            |
   |                                                                           |
   |   Copyright (C) 2005-2010  University of Malaga                           |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics Lab, University of Malaga (Spain).                          |
   |    Contact: Jose-Luis Blanco  <jlblanco@ctima.uma.es>                     |
   |                                                                           |
   |  This file is part of the MRPT project.                                   |
   |                                                                           |
   |     MRPT is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |   MRPT is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with MRPT.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+ */

#include <mrpt/base.h>  // Precompiled headers 


#include <mrpt/poses/CPoses3DSequence.h>
using namespace mrpt;
using namespace mrpt::poses;
using namespace mrpt::math;
using namespace mrpt::utils;

IMPLEMENTS_SERIALIZABLE( CPoses3DSequence, CSerializable ,mrpt::poses)

/*---------------------------------------------------------------
			Default constructor
  ---------------------------------------------------------------*/
CPoses3DSequence::CPoses3DSequence() :
	m_poses()
{

}

/*---------------------------------------------------------------
			Returns the poses count in the sequence:
  ---------------------------------------------------------------*/
size_t	CPoses3DSequence::posesCount()
{
	return m_poses.size();
}

/*---------------------------------------------------------------
   Implements the writing to a CStream capability of
     CSerializable objects
  ---------------------------------------------------------------*/
void  CPoses3DSequence::writeToStream(CStream &out,int *version) const
{
	if (version)
		*version = 0;
	else
	{
		uint32_t		i,n;

		// The poses:
		n = m_poses.size();
		out << n;
		for (i=0;i<n;i++) out << m_poses[i];
	}
}

/*---------------------------------------------------------------
	Implements the reading from a CStream capability of
		CSerializable objects
  ---------------------------------------------------------------*/
void  CPoses3DSequence::readFromStream(CStream &in, int version)
{
	switch(version)
	{
	case 0:
		{
			uint32_t	i,n;

			// The poses:
			in >> n;
			m_poses.resize(n);
			for (i=0;i<n;i++)	in >> m_poses[i];
		} break;
	default:
		MRPT_THROW_UNKNOWN_SERIALIZATION_VERSION(version)

	};
}

/*---------------------------------------------------------------
Reads the stored pose at index "ind", where the first one is 0, the last "posesCount() - 1"
 ---------------------------------------------------------------*/
void  CPoses3DSequence::getPose(unsigned int ind, CPose3D &outPose)
{
	if (ind>=m_poses.size()) THROW_EXCEPTION("getPose: Index out of range!!");

	outPose = m_poses[ind];
}

/*---------------------------------------------------------------
Changes the stored pose at index "ind", where the first one is 0, the last "posesCount() - 1"
 ---------------------------------------------------------------*/
void  CPoses3DSequence::changePose(unsigned int ind, CPose3D &inPose)
{
	if (ind>=m_poses.size()) THROW_EXCEPTION("getPose: Index out of range!!");
	m_poses[ind] = inPose;
}

/*---------------------------------------------------------------
	Appends a new pose at the end of sequence. Remember that poses are relative, incremental to the last one.
 ---------------------------------------------------------------*/
void  CPoses3DSequence::appendPose(CPose3D &newPose)
{
	m_poses.push_back( newPose );
}

/*---------------------------------------------------------------
			Clears the sequence.
 ---------------------------------------------------------------*/
void  CPoses3DSequence::clear()
{
	m_poses.clear();
}

/*---------------------------------------------------------------
 ---------------------------------------------------------------*/
/** Returns the absolute pose of a robot after moving "n" poses, so for "n=0" the origin pose (0,0,0deg) is returned, for "n=1" the first pose is returned, and for "n=posesCount()", the pose
 *  of robot after moving ALL poses is returned, all of them relative to the starting pose.
 */
CPose3D  CPoses3DSequence::absolutePoseOf(unsigned int n)
{
	CPose3D		ret(0,0,0);
	unsigned int			i;

	if (n>m_poses.size()) THROW_EXCEPTION("absolutePoseOf: Index out of range!!");

	for (i=0;i<n;i++)	ret = ret + CPose3D(m_poses[i]);

	return ret;
}


/*---------------------------------------------------------------
	A shortcut for "absolutePoseOf( posesCount() )".
 ---------------------------------------------------------------*/
CPose3D  CPoses3DSequence::absolutePoseAfterAll()
{
	return absolutePoseOf( posesCount() );
}

/*---------------------------------------------------------------
	Returns the traveled distance after moving "n" poses, so for "n=0" it returns 0, for "n=1" the first traveled distance, and for "n=posesCount()", the total
	distance after ALL movements.
 ---------------------------------------------------------------*/
float  CPoses3DSequence::computeTraveledDistanceAfter(unsigned int n)
{
	unsigned int		i;
	float	dist = 0;

	if (n>m_poses.size()) THROW_EXCEPTION("computeTraveledDistanceAfter: Index out of range!!");

	for (i=0;i<n;i++)	dist+= m_poses[i].norm();
	return dist;
}

/*---------------------------------------------------------------
	Returns the traveled distance after ALL movements.
	A shortcut for "computeTraveledDistanceAfter( posesCount() )".
 ---------------------------------------------------------------*/
float  CPoses3DSequence::computeTraveledDistanceAfterAll()
{
	return computeTraveledDistanceAfter( posesCount() );
}
