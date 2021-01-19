#include "PMSnetsdk.h"
#include "DeviceManage.h"
#include "singleton_instance.hpp"

CLIENT_NET_API int	CALL_METHOD DEV_Init()
{
	base::singleton_instance<CDeviceManage>::instance()->Init();
	return 0;
}
CLIENT_NET_API void	CALL_METHOD DEV_DeInit()
{
	// 必须先uninitCDeviceManage 会将cnetsdk反初始化掉
	base::singleton_instance<CDeviceManage>::instance()->UnInit();
	base::singleton_instance<CDeviceManage>::release();
	return;
}

CLIENT_NET_API int CALL_METHOD DEV_SetConnectState(PFN_GET_CONNECT_STATE_CB pFnGetConnectStateCB, void* pUser)
{
	int nRet = base::singleton_instance<CDeviceManage>::instance()->SetConnectState(pFnGetConnectStateCB, pUser);
	return nRet;
}

// Connect/Control
CLIENT_NET_API int	CALL_METHOD DEV_Login(UINT8* byDevIpAddr, UINT32 nDevPort, UINT8* byUserName, UINT8* byPassword)
{
	int nRet = base::singleton_instance<CDeviceManage>::instance()->DevLogin(byDevIpAddr, nDevPort);
	return nRet;
}
CLIENT_NET_API int	CALL_METHOD DEV_Logout(INT32 nLoginID)
{
	int nRet = base::singleton_instance<CDeviceManage>::instance()->DevLogout(nLoginID);
	return nRet;
}

CLIENT_NET_API int	CALL_METHOD DEV_StartStream(INT32 nLoginID, PFN_GET_FRAME_CB pFnGetFrameCB, void* pUser) {
	int nRet = base::singleton_instance<CDeviceManage>::instance()->GetFrame(nLoginID, pFnGetFrameCB, pUser);
	return nRet;
}