#include "DeviceManage.h"

#define ReceiveFrameLen	1024*32


void CALLBACK FnDataCallBack(unsigned long lLoginID, const char *pBuffer, unsigned long dwBufSize, void* dwUser)
{
	if (dwUser)
	{
		CDeviceManage* pThis = (CDeviceManage*)dwUser;
		pThis->GroupFrame(lLoginID, pBuffer, dwBufSize);
	}

}

void CALLBACK FnStateCallback(unsigned long lLoginID, char *szIP, unsigned long state, void* dwUser)
{
	if (dwUser)
	{
		CDeviceManage* pThis = (CDeviceManage*)dwUser;
		if (pThis->m_pfnGetConnectStateCB)
		{
			pThis->m_pfnGetConnectStateCB(lLoginID, szIP, (DEV_CONNECT_STATUS)state, pThis->m_pStateUser);
		}


	}
}

CDeviceManage::CDeviceManage()
{
	Init();
}

CDeviceManage::~CDeviceManage()
{
	UnInit();
}


int CDeviceManage::Init()
{
	m_vctDevLogin.clear();

	m_pfnGetConnectStateCB = NULL;
	m_pStateUser = NULL;
	m_pfnGetFrameCB = NULL;
	m_pFrameUser = NULL;
	CNet_SetNewFrameListener(FnDataCallBack, this);
	CNet_SetDeviceStateListener(FnStateCallback, this);
	m_nReceiveDataLength = 0;
	m_pRecvDepthBuf = new unsigned char[ReceiveFrameLen * 2];
	memset(m_pRecvDepthBuf, 0 , ReceiveFrameLen * 2);
	return 0;
}

void CDeviceManage::UnInit()
{
	DEV_LOGIN_INFO stuLoginInfo;

	std::vector<DEV_LOGIN_INFO>::iterator it = m_vctDevLogin.begin();
	while (it != m_vctDevLogin.end())
	{
		stuLoginInfo = *it;
		if (stuLoginInfo.nLoginID > 0)
		{
			CNet_DisconnectDevice(stuLoginInfo.nLoginID);
		}

		m_vctDevLogin.erase(it++);

		if (m_vctDevLogin.size() == 0)
		{
			return;
		}

	}
}


INT32 CDeviceManage::DevLogin(UINT8* byDevIpAddr, UINT32 nDevPort)
{
	
	int nLoginID = CNet_ConnectDevice((const char*)byDevIpAddr, nDevPort, "admin", "123456", ConnectMode::UDP);

	if (nLoginID > 0)
	{
		DEV_LOGIN_INFO stLoginInfo = { 0 };
		strcpy_s((char*)stLoginInfo.iDevIpAddr, 16, (char*)byDevIpAddr);
		stLoginInfo.nLoginID = nLoginID;
		m_vctDevLogin.push_back(stLoginInfo);
		return nLoginID;
	}

	return -1;
}

INT32 CDeviceManage::DevLogout(UINT32 nLoginID)
{
	DEV_LOGIN_INFO stuLoginInfo;

	std::vector<DEV_LOGIN_INFO>::iterator it = m_vctDevLogin.begin();
	while (it != m_vctDevLogin.end())
	{
		stuLoginInfo = *it;
		if (stuLoginInfo.nLoginID == nLoginID)
		{
			CNet_DisconnectDevice(nLoginID);
			m_vctDevLogin.erase(it++);
			break;
		}
		else
		{
			it++;
		}
	}
	return 0;
}

int CDeviceManage::SetConnectState(PFN_GET_CONNECT_STATE_CB pFnGetConnectStateCB, void* pUser)
{
	m_pfnGetConnectStateCB = pFnGetConnectStateCB;
	m_pStateUser = pUser;
	return 0;
}

void CDeviceManage::GroupFrame(UINT32 nLoginID, const char* pData, unsigned long nLen)
{
	if (pData)
	{
		char nKeyValue = pData[0];
		switch (nKeyValue)
		{
		case 0x43:
		{
			ParseStreamDataPacket(nLoginID,pData, nLen);
		}
		break;
		default:
			break;
		}
	}
}

void CDeviceManage::ParseData(const char* pData, unsigned long nLen)
{
	if (pData)
	{
		int nKeyValue = pData[0];
		switch (nKeyValue)
		{
		case 0x43: 
		{
			//ParseStreamDataPacket(pData, nLen);
		}
		break;
		default:
			break;
		}
	}
}

void CDeviceManage::ParseStreamDataPacket(int nLoginID, const char* data, int len)
{
	if (data)
	{
		char Value = data[1];
		switch (Value)
		{
		case 0x01:
		{
			char Flag = data[4];
			if (Flag == 1){
				m_nReceiveDataLength = 0;
			}
			int nEffectiveLen = len - 4;
			int nCopyLen = nEffectiveLen > (ReceiveFrameLen - m_nReceiveDataLength) ? (ReceiveFrameLen - m_nReceiveDataLength) : nEffectiveLen;
			memcpy((char*)m_pRecvDepthBuf + m_nReceiveDataLength, data + 4, nCopyLen);
			m_nReceiveDataLength += nCopyLen;
			char EndFlag = data[4];
			if (m_nReceiveDataLength >= ReceiveFrameLen && Flag == 2) {
				GP_DEV_FRAME_INFO stDevFrameInfo = { 0 };
				stDevFrameInfo.nFrameID = 0;
				stDevFrameInfo.nFrameType = 0;
				stDevFrameInfo.pFrameData = NULL;
				stDevFrameInfo.nDataLen = m_nReceiveDataLength;
				stDevFrameInfo.pFrameData = new (std::nothrow)char[ReceiveFrameLen];
				if (!stDevFrameInfo.pFrameData)
				{
					return;
				}
				memcpy(stDevFrameInfo.pFrameData, m_pRecvDepthBuf, stDevFrameInfo.nDataLen);
				if (m_pfnGetFrameCB) {
					m_pfnGetFrameCB(nLoginID, &stDevFrameInfo, m_pFrameUser);
				}
			}
		}
		break;
		default:
			break;
		}
	}
}

int	CDeviceManage::GetFrame(UINT32 nLoginID, PFN_GET_FRAME_CB pfnGetFrameCB, void* pUser) {
	m_pfnGetFrameCB = pfnGetFrameCB;
	m_pFrameUser = pUser;
	COMMAND_PACKET stCommand;
	stCommand.byKey = 0x43;
	stCommand.byKey = 0x02;
	int nPacketLen = sizeof(stCommand);
	bool bRet = CNet_SetDevConfig(nLoginID, (const char*)&stCommand, nPacketLen);

	if (!bRet)
	{
		return -1;
	}
	return true;
}



