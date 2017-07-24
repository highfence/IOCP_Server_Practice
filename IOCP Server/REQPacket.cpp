
#include "ServiceManager.h"
#include "../CommonLib/Protocol.h"

using namespace NetworkLib;

const BOOL ServiceManager::recvCS_AUTH_LOGIN_REQ(SessionData* pSession)
{
	WCHAR	Buffer[MAX_BUFFER];
	static UINT snCount = 0;	// �� static ������ �׽�Ʈ ������ ����Ŷ� �����ص� �˴ϴ�

	body_CS_AUTH_LOGIN_REQ* body = (body_CS_AUTH_LOGIN_REQ*)(Buffer + SIZE_HEADER);

	memcpy(&Buffer, &pSession->m_SocketCtx.recvContext->Buffer, sizeof(body_CS_AUTH_LOGIN_REQ) + SIZE_HEADER);

	int id = body->id;
	int pw = body->pw;

	printf("Ŭ���̾�Ʈ�� ���۹��� �� id : %d, pw : %d - Count : %d\n", id, pw, ++snCount);

	this->recvCS_AUTH_LOGIN_ACK(pSession);

	return 1;
}