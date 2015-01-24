#pragma once
#include <Client.h>

#include "ClientRenderer.h"

namespace nekotama
{
	/// @brief �ͻ���ʵ��
	class ClientImplement :
		public Client
	{
	private:
		std::shared_ptr<ClientRenderer> m_ClientRenderer;
	public:
		void OnConnectFailed()NKNOEXCEPT;
		void OnNotSupportedServerVersion()NKNOEXCEPT;
		void OnKicked(KickReason why)NKNOEXCEPT;
		void OnLostConnection()NKNOEXCEPT;
		void OnLoginSucceed(const std::string& server, const std::string& nickname, const std::string& addr, uint16_t gameport)NKNOEXCEPT;
		void OnDataArrival(const std::string& source_ip, uint16_t source_port, uint16_t target_port, const std::string& data)NKNOEXCEPT;
		void OnRefreshGameInfo(const std::vector<GameInfo>& info)NKNOEXCEPT;
		void OnDelayRefreshed()NKNOEXCEPT;
	public:
		ClientImplement(const std::shared_ptr<ClientRenderer>& renderer, const std::string& serverip, const std::string& nickname, uint16_t port = 12801);
	};
}
