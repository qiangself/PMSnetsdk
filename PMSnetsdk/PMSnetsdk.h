#ifndef _NET_SDK_DEFINE_H_
#define _NET_SDK_DEFINE_H_

#if defined _WIN32 || defined _WIN64
#define CALLBACK	__stdcall
#define CALL_METHOD __stdcall

#elif defined __linux__
#define CALLBACK  
#define CALL_METHOD
#endif


#if defined(WIN32) || defined(WIN64)		//windows include
//#include <windows.h>
#ifndef INT8
typedef signed char INT8;
#endif
#ifndef UINT8
typedef unsigned char UINT8;
#endif
#ifndef UINT16
typedef unsigned short UINT16;
#endif
#ifndef INT16
typedef short INT16;
#endif
#ifndef UINT32
typedef unsigned int UINT32;
#endif
#ifndef INT32
typedef signed int INT32;
#endif
#ifndef UINT64
typedef unsigned __int64 UINT64;
#endif
#ifndef INT64
typedef signed __int64 INT64;
#endif
#ifndef FLOAT32
typedef float FLOAT32;
#endif
#ifndef FLOAT64
typedef double FLOAT64;
#endif


#ifdef PMSNETSDK_EXPORTS
# define CLIENT_NET_API __declspec(dllexport)
#else
# define CLIENT_NET_API __declspec(dllimport)
#endif

#elif defined __linux__

#ifndef UINT8
typedef unsigned char UINT8;
#endif
#ifndef UINT16
typedef unsigned short UINT16;
#endif
#ifndef INT16
typedef short INT16;
#endif
#ifndef UINT32
typedef unsigned int UINT32;
#endif
#ifndef INT32
typedef signed int INT32;
#endif
#ifndef UINT64
typedef unsigned __int64 UINT64;
#endif
#ifndef INT64
typedef signed __int64 INT64;
#endif
#ifndef FLOAT32
typedef float FLOAT32;
#endif
#ifndef FLOAT64
typedef double FLOAT64;
#endif

#define CLIENT_NET_API  extern "C"

#endif
///< 设备帧数据
typedef struct _GP_DEV_FRAME_INFO_
{
	unsigned int nFrameID;
	unsigned int nFrameType;
	char*		 pFrameData;
	unsigned int nDataLen;
	unsigned int nReserved[10];   
}GP_DEV_FRAME_INFO, *PGP_DEV_FRAME_INFO;

///< 连接状态
typedef enum _DEV_CONNECT_STATUS_
{
	STATUS_DISCONNECTED = 0,
	STATUS_CONNECTED = 1,
}DEV_CONNECT_STATUS;

///< 回调函数
typedef void(CALLBACK *PFN_GET_FRAME_CB)(const int nLoginID, const PGP_DEV_FRAME_INFO pstFrameInfo, void* pUserData);
typedef void(CALLBACK *PFN_GET_CONNECT_STATE_CB)(const int nLoginID, const char* szIP, const  DEV_CONNECT_STATUS enStatus, void* pUserData);

//---------------------------------------------------------------
/**
\brief	 SDK初始化,使用时必须先调用
\param   void

\return  状态参数 0表示成功， <0表示失败
*/
//---------------------------------------------------------------
CLIENT_NET_API int	CALL_METHOD DEV_Init();


//---------------------------------------------------------------
/**
\brief	 SDK资源释放,结束时必须调用
\param   void

\return  void
*/
//---------------------------------------------------------------
CLIENT_NET_API void	CALL_METHOD DEV_DeInit();

//---------------------------------------------------------------
/**
\brief	初始化后先调用DEV_SetConnectState，监控设备的连接、断开状态
\param   pFnGetConnectStateCB     [in]设备连接断开状态监控回调函数
\param   pUser                    [in]用户

\return  状态参数 0表示成功， <0表示失败
*/
//---------------------------------------------------------------
CLIENT_NET_API int  CALL_METHOD DEV_SetConnectState(PFN_GET_CONNECT_STATE_CB pFnGetConnectStateCB, void* pUser);

//---------------------------------------------------------------
/**
\brief	 设备登录
\param   byDevIpAddr	          [in]设备IP
\param   nDevPort                 [in]端口号
\param   byUserName               [in]用户名，无写NULL
\param   byPassword               [in]密码，无写NULL

\return  返回值nLoginID > 0表示登录ID, <= 0 表示登录失败
*/
//---------------------------------------------------------------
CLIENT_NET_API int	CALL_METHOD DEV_Login(UINT8* byDevIpAddr, UINT32 nDevPort, UINT8* byUserName, UINT8* byPassword);


//---------------------------------------------------------------
/**
\brief	 设备退出
\param   nLoginID				  [in]登录ID

\return  状态参数 0表示成功， <0表示失败
*/
//---------------------------------------------------------------
CLIENT_NET_API int	CALL_METHOD DEV_Logout(INT32 nLoginID);


//---------------------------------------------------------------
/**
\brief	 设置设备取流回调
\param   nLoginID				  [in]登录ID
\param   pFnGetFrameCB			  [in]获取Frame数据回调
\param   pUser                    [in]用户

\return  状态参数 0表示成功， <0表示失败
*/
//---------------------------------------------------------------
CLIENT_NET_API int	CALL_METHOD DEV_StartStream(INT32 nLoginID, PFN_GET_FRAME_CB pFnGetFrameCB, void* pUser);

#endif _GP_NET_SDK_DEFINE_H_
