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

#ifndef CInterfaceFTDI_H
#define CInterfaceFTDI_H

#include <mrpt/config.h>
#include <mrpt/utils/CStream.h>
#include <mrpt/utils/stl_extensions.h>

#include <mrpt/hwdrivers/link_pragmas.h>

namespace mrpt
{
	namespace hwdrivers
	{

		/** A list of FTDI devices and their descriptors.
		  * \sa CInterfaceFTDI::ListAllDevices
		  */
		struct HWDRIVERS_IMPEXP TFTDIDevice
		{
			std::string 	ftdi_manufacturer;
			std::string 	ftdi_description;
			std::string 	ftdi_serial;

			uint16_t		usb_idVendor;
			uint16_t		usb_idProduct;
			uint8_t			usb_serialNumber;

	#if defined(MRPT_OS_LINUX) || defined(MRPT_OS_APPLE)
			/** Only for Linux: the corresponding libusb's  "usb_device*" */
			void 			*usb_device_struct;
	#endif
		};

		/** Print out all the information of a FTDI device in textual form. */
		std::ostream HWDRIVERS_IMPEXP  &operator << ( std::ostream &o, const TFTDIDevice &d);

		/** Used in  CInterfaceFTDI::ListAllDevices */
		typedef  std::deque<TFTDIDevice> TFTDIDeviceList;

		/** A definition of a CStream actually representing a USB connection to a FTDI chip.
		 *
		 *  This class implements the communication with FT245BM / FT245RL chips.
		 *   Using this class makes a program to depend on:
		 *		- Windows: "FT2XX.DLL" and the device drivers (see FTDI website).
		 *		- Linux: "libusb.so" (quite standard!), and "libftdi.so" only if linking against the dynamic library.
		 *
		 *  If there is any error during the communications (or loading the Windows DLL), a std::exception will be raised.
		 *
		 *  To write bulk data, use CStream::ReadBuffer and CStream::WriteBuffer. See also the derived classes for
		 *   higher level communication: CInterfaceFTDIMessages
		 *
		 * Warning: Avoid defining an object of this class in a global scope if you want to catch all potential
		 *      exceptions during the constructors (like DLL not found, etc...)
		 *
		 * VERSIONS:
		 *		- 11/APR/2005: Initial development. JLBC
		 *		- 16/FEB/2007: Integration into the MRPT framework. Support for device serial numbers. JLBC
		 *		- 15/APR/2008: Implemented for Linux using libftdi. JLBC
		 *
		 * \sa CInterfaceFTDIMessages, CStream
		 */
		class HWDRIVERS_IMPEXP CInterfaceFTDI : public utils::CStream
		{
		public:
			/** Constructor, which loads driver interface (the DLL under Windows).
			  */
			CInterfaceFTDI();

			/** Destructor, which closes the connection with the chip and unloads the driver interface.
			  */
			virtual ~CInterfaceFTDI();

			/** This object cannot be copied */
			CInterfaceFTDI(const CInterfaceFTDI &o);

			/** This object cannot be copied */
			CInterfaceFTDI& operator =(const CInterfaceFTDI &o);

			/** Checks whether the chip has been successfully open.
			  * \sa OpenBySerialNumber, OpenByDescription
			  */
			bool  isOpen();

			/** Open by device serial number
			  */
			void  OpenBySerialNumber( const std::string &serialNumber );

			/** Open by device description
			  */
			void  OpenByDescription( const std::string &description );

			/** Close the USB device */
			void  Close();

			/** Reset the USB device */
			void  ResetDevice();

			/** Purge the I/O buffers */
			void  Purge();

			/** Change the latency timer (in milliseconds) implemented on the FTDI chip: for a few ms, data is not sent to the PC waiting for possible more data, to save USB trafic. */
			void  SetLatencyTimer (unsigned char latency_ms);

			/** Change read & write timeouts, in milliseconds. */
			void  SetTimeouts(unsigned long dwReadTimeout_ms, unsigned long dwWriteTimeout_ms);


			/** Generates a list with all FTDI devices connected right now.
			  */
			void ListAllDevices( TFTDIDeviceList &outList );

			/** Tries to read, raising no exception if not all the bytes are available, but raising one if there is some communication error.
			 */
			size_t  ReadSync(void *Buffer, size_t Count)
			{
				return Read(Buffer,Count);
			}

			/** Tries to write, raising no exception if not all the bytes are available, but raising one if there is some communication error.
			 */
			size_t  WriteSync(const void *Buffer, size_t Count)
			{
				return Write(Buffer,Count);
			}

		protected:
			/** Introduces a pure virtual method responsible for reading from the stream.
			  *  It integrates a cache buffer to speed-up sequences of many, small readings.
			  */
			size_t  Read(void *Buffer, size_t Count);

			mrpt::utils::circular_buffer<uint8_t>    m_readBuffer;  //!< Used in Read

			/** Introduces a pure virtual method responsible for writing to the stream.
			 *  Write attempts to write up to Count bytes to Buffer, and returns the number of bytes actually written.
			 */
			size_t  Write(const void *Buffer, size_t Count);

			/** This virtual method does nothing in this class.
			 */
			size_t Seek(long Offset, CStream::TSeekOrigin Origin = sFromBeginning);

			/** This virtual method does nothing in this class.
			 */
			size_t getTotalBytesCount();

			/** This virtual method does nothing in this class.
			 */
			size_t getPosition();


			void  ftdi_read(void  *lpvBuffer, unsigned long dwBuffSize, unsigned long  *lpdwBytesRead);
			void  ftdi_write(const void  *lpvBuffer, unsigned long dwBuffSize, unsigned long  *lpdwBytes);

	#if defined(MRPT_OS_WINDOWS)
		private:
			void  checkErrorAndRaise(int errorCode);

			void  ftdi_open(void* pvDevice);
			void  ftdi_openEx(void* pArg1, unsigned long dwFlags);
			void  ftdi_listDevices(void *pArg1, void *pArg2, unsigned long dwFlags);
			void  ftdi_getQueueStatus(unsigned long  *lpdwAmountInRxQueue);

			void				*m_hmodule;
			unsigned long		m_ftHandle;

			void 		loadDriver();

			enum FT_STATUS
			{
				dummy
			};

			typedef FT_STATUS (__stdcall *PtrToOpen)(void*, unsigned long  *);
			PtrToOpen m_pOpen;

			typedef FT_STATUS (__stdcall *PtrToOpenEx)(void*, unsigned long, unsigned long  *);
			PtrToOpenEx m_pOpenEx;

			typedef FT_STATUS (__stdcall *PtrToListDevices)(void*, void*, unsigned long);
			PtrToListDevices m_pListDevices;

			typedef FT_STATUS (__stdcall *PtrToClose)(unsigned long );
			PtrToClose m_pClose;

			typedef FT_STATUS (__stdcall *PtrToRead)(unsigned long , void  *, unsigned long, unsigned long  *);
			PtrToRead m_pRead;

			typedef FT_STATUS (__stdcall *PtrToWrite)(unsigned long , const void  *, unsigned long, unsigned long  *);
			PtrToWrite m_pWrite;

			typedef FT_STATUS (__stdcall *PtrToResetDevice)(unsigned long );
			PtrToResetDevice m_pResetDevice;

			typedef FT_STATUS (__stdcall *PtrToPurge)(unsigned long , unsigned long);
			PtrToPurge m_pPurge;

			typedef FT_STATUS (__stdcall *PtrToSetTimeouts)(unsigned long , unsigned long, unsigned long);
			PtrToSetTimeouts m_pSetTimeouts;

			typedef FT_STATUS (__stdcall *PtrToGetQueueStatus)(unsigned long , unsigned long  *);
			PtrToGetQueueStatus m_pGetQueueStatus;

			typedef FT_STATUS (__stdcall *PtrToSetLatencyTimer )(unsigned long , unsigned char);
			PtrToSetLatencyTimer m_pSetLatencyTimer;

	#else
		// Declarations for Linux:
			void		*m_ftdi_context;

			/** Process recursively a USB device and its children: */
			void recursive_fill_list_devices( void *usb_device_structure , TFTDIDeviceList &outList );


	#endif


		}; // end of class

	} // end of namespace
} // end of namespace

#endif
