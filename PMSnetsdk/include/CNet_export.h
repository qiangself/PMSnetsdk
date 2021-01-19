/**
* Copyright (c) 2018 Genius Pros, Inc. All rights reserved.
*
* This file defines newwork data send and receive external interface definition
*
* @author Xi Wenyong
* @version 1.0 init implement(20170801)
*/

#ifndef VIXHZ_EXPORT_H
#define VIXHZ_EXPORT_H

#include "CNet_def.h"

/**
* define video stream data callback function
* @param lLoginID[unsigned long] the login id which each client only voucher
* @param pBuffer[const char *] video stream data
* @param dwBufSize[unsigned long] the size of video stream
* @param dwUser[void*] user param
*/
typedef void (CALLBACK* FrameDataCallback)(unsigned long lLoginID, const char *pBuffer, unsigned long dwBufSize, void* dwUser);

/**
* define device state callback function
* @param lLoginID[unsigned long] the login id which each client only voucher
* @param pDevIp[char *] device ip
* @param lParam1[unsigned long] the state of device
* @param dwUser[void*] user param
*/
typedef void (CALLBACK * DeviceStateCallback)(unsigned long lLoginID, char*pDevIp, unsigned long lParam1, void* dwUser);


extern "C"
{
	/**
	* init sdk
	* @return true
	*/
	bool CNet_InitSDK();

	/**
	* unInit sdk
	*/
	void CNet_UnInitSDK();

	/**
	* connect device
	* @param szIp[const char*] device ip
	* @param nPort[int] device port
	* @param szUserName[const char*] the user name of the login device,default to empty
	* @param szPassword[const char*] the password of the login device,default to empty
	* @param model[ConnectMode] login model,TCP and UDP,default UDP
	* @return login id  when connect successful,otherwise return 0
	*/
	 long CNet_ConnectDevice(const char* szIp, int nPort, const char* szUserName = "", const char* szPassword = "", ConnectMode model = ConnectMode::UDP);

	/**
	* disconnect device
	* @param lLoginID[unsigned long] user login id
	*/
	void CNet_DisconnectDevice(unsigned long lLoginID);
	 
	/**
	* set data callback function
	* @param func[FrameDataCallback] user define callback
	* @param pUser[void*] user param
	* @return true
	*/
	long CNet_SetNewFrameListener(FrameDataCallback func, void* pUser);
	/**
	* reset data callback function
	* @param lLoginId[long] user login id
	* @return true
	*/
	long CNet_RemoveNewFrameListener(long lLoginId);

	/**
	* set device state callback function
	* @param func[DeviceStateCallback] user define callback
	* @param pUser[void*] user param
	* @return true
	*/
	long CNet_SetDeviceStateListener(DeviceStateCallback func, void* pUser);
	
	/**
	* set device configuration
	* @param lLonginID[unsigned long] user login id
	* @param config[const char*] config data
	* @param nSize[int] the size of config data
	* @return true when set successful otherwise false;
	*/
	bool CNet_SetDevConfig(unsigned long lLonginID, const char*  config, int nSize);
	/**
	* Unimplement
	* get device configuration 
	* @param lLonginID[unsigned long] user login id
	* @param config[const char*] config data
	* @param nSize[int] the size of return config data
	* @return false;
	*/
	bool CNet_GetDevConfig(unsigned long lLonginID, const char*  config, int& nSize);
	
	/**
	* serach device
	* @param pBuf[GVCP_DEVICE_INFO] the cache of device info
	* @param nNum[int] the number of device to be serached
	* @param ulWaitTime[unsigned long] timeout(ms)
	* @return false;
	*/
	bool CNet_SearchDevice(GVCP_DEVICE_INFO* pBuf, int& nNum, unsigned long ulWaitTime);
};

#endif