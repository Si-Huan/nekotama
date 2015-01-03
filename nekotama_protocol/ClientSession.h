#pragma once
#include <deque>

#include <ISocket.h>

#include "ILogger.h"
#include "Package.h"

namespace nekotama
{
	class Server;
	class ClientSession;
	typedef std::shared_ptr<ClientSession> ClientSessionHandle;

	/// @brief 用户会话
	class ClientSession
	{
		friend class Server;
	public:
		typedef std::deque<uint8_t> BufferType;
	private:
		ILogger* m_pLogger;
		SocketHandle m_Socket;
		std::string m_IP;
		uint16_t m_Port;

		// 数据缓冲区
		PackageValidChecker m_Checker;
		BufferType m_RecvBuf;
		BufferType m_SendBuf;
	public:
		const std::string& GetIP()const { return m_IP; }
		uint16_t GetPort()const { return m_Port; }

	protected:  // 底层数据传输控制函数
		/// @brief  接收数据直到组成一个封包
		/// @return 若数据已能组成一个封包则返回true
		bool RecvData();
		/// @brief  发送所有未发送的数据
		void SendData();
		/// @brief  计时器计数
		void Tick(uint32_t TickCount);
		/// @brief  获取读取缓冲区
		const BufferType& GetReadBuffer()const { return m_RecvBuf; }
		/// @brief  获取写入缓冲区
		BufferType& GetWriteBuffer() { return m_SendBuf; }
	public:
		ClientSession(SocketHandle handle, const std::string& ip, uint16_t port, ILogger* pLogger);
	};
}
