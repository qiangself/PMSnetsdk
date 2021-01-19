#ifndef BU_DEVICE_MANAGE_H
#define BU_DEVICE_MANAGE_H




#include "CNet_def.h"
#include "CNet_export.h"
#include <vector>
#include <process.h>
#include <map>
#include "singleton_instance.hpp"
#include "PMSnetsdk.h"

//#include <windows.h>
static void CALLBACK FnDataCallBack(unsigned long lLoginID, const char *pBuffer, unsigned long dwBufSize, void* dwUser);
static void CALLBACK FnStateCallback(unsigned long lLoginID, char *szIP, unsigned long state, void* dwUser);



typedef struct _DEV_LOGIN_INFO_
{
	UINT32    nLoginID;
	UINT8     byMac[6];
	UINT8     iDevIpAddr[16];
}DEV_LOGIN_INFO;

typedef struct _COMMAND_PACKET_
{
	UINT8	byKey;
	UINT8	byFlag;
	UINT8	byCommand;
	UINT8	byLength;
}COMMAND_PACKET, *PCOMMAND_PACKET;

class CDeviceManage
{
public:
	CDeviceManage();
	~CDeviceManage();
	friend class base::singleton_instance<CDeviceManage>;
	int			Init();
	void		UnInit();
	INT32		DevLogin(UINT8* byDevIpAddr, UINT32 nDevPort);
	INT32		DevLogout(UINT32 nLoginID);
	int         SetConnectState(PFN_GET_CONNECT_STATE_CB pFnGetConnectStateCB, void* pUser);
	void        GroupFrame(UINT32 nLoginID, const char* pData, unsigned long nLen);
	int			GetFrame(UINT32 nLoginID, PFN_GET_FRAME_CB pfnGetFrameCB, void* pUser);
private:
	std::vector<DEV_LOGIN_INFO> m_vctDevLogin;
	void		ParseData(const char* pData, unsigned long nLen);
	void		ParseStreamDataPacket(int nLoginID, const char* data, int len);
	int			m_nReceiveDataLength;
	unsigned char* m_pRecvDepthBuf;
public:
	PFN_GET_CONNECT_STATE_CB m_pfnGetConnectStateCB;
	PFN_GET_FRAME_CB m_pfnGetFrameCB;
	void*			 m_pFrameUser;
	void*            m_pStateUser;

	


};

#endif

