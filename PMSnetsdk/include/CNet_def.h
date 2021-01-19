/**
* Copyright (c) 2018 Genius Pros, Inc. All rights reserved.
*
* This file defines the data structure definition that the network data send and receive the sdk needs
*
* @author Xi Wenyong
* @version 1.0 init implement(20170801)
*/

#ifndef VIXHZ_DEF_H
#define VIXHZ_DEF_H

#if defined(_WIN32) || defined(WIN32)
// For FILETIME in FromFileTime, until it moves to a new converter class.
// See TODO(iyengar) below.
//#include <windows.h>
#include <string>
#define CALLBACK __stdcall

//#ifdef CNET_SDK
//# define CNET_EXPORT __declspec(dllexport)
//#else
//# define CNET_EXPORT __declspec(dllimport)
//#endif

#else
#include <stddef.h>
#include <netdb.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>
#include <ifaddrs.h>
#define CALLBACK
#define HWND void*
#define SOCKET int

//#ifdef CNET_SDK
//# define CNET_EXPORT __attribute__((visibility("default")))
//#else
//# define CNET_EXPORT
//#endif
#endif

#define XN_STATUS_OK 0

/**
* Network connection mode define
*/
typedef enum _ConnectMode
{
	TCP,
	UDP
}ConnectMode;

/**
* device state define
*/
typedef enum _DeviceState
{
	DEV_STATE_DISCONNECTED,
	DEV_STATE_CONNECTED
}DeviceState;

/**
* maximum number of serach devices
*/
#define MAX_SERACH_DEVICE_NUMBER	100

/**
* Network protocal command define
*/
#define GVCP_KET_VALUE			0x42
#define GVCP_REQ_FLAG			0x00
#define GVCP_ACK_FLAG			0x01
#define GVCP_DEVICE_REQ			0x0010
#define GVCP_DEVICE_ACK			0x0011
#define GVCP_CONNECT_REQ		0x0012
#define GVCP_CONNECT_ACK		0x0013
#define GVCP_READ_REQ			0x0020
#define GVCP_READ_ACK			0x0021
#define GVCP_WRITE_REQ			0x0030
#define GVCP_WRITE_ACK			0x0031
#define GVCP_TRANS_IRQ_REQ		0x0040
#define GVCP_TRANS_IRQ_ACK		0x0041

#define GVCP_UPDATAE_DEVICE_CONFIG_REQ  0x0050  
#define GVCP_UPDATAE_DEVICE_CONFIG_ACK  0x0051

#define GVCP_UPGRADE_DEVICE_REQ  0x0060  
#define GVCP_UPGRADE_DEVICE_ACK  0x0061

#define GVCP_TRANS_CTRL_REQ       0x0070
#define GVCP_TRANS_CTRL_ACK       0x0071

#define GVCP_TIME_REQ			   0x0080
#define GVCP_TIME_ACK			   0x0081
/**
* device infomation define
*/
struct GVCP_DEVICE_INFO 
{
	char ip[64];
	char mask[64];
	char gateway[64];	
	int nCameraId;
	int nCameraType;
	int nFpgaVision;	//camera fpga version
	int nAndroidOpenniVersion;	//Android openni version
	int nAndroidSystemVersion;  //Android system version
	unsigned int nPort;
};


#endif
