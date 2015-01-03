#pragma once
#include <atomic>
#include <thread>
#include <stdexcept>

#include <ISocket.h>

#include "ILogger.h"
#include "ConcurrentQueue.h"
#include "ClientSession.h"

namespace nekotama
{
	/// @brief ������
	struct IServerListener
	{
		virtual void OnCreateSession(ClientSession* pSession) = 0;
		virtual void OnCloseSession(ClientSession* pSession) = 0;

	};

	/// @brief ����
	class Server
	{
		/// @brief ��Ϣ����
		enum class MessageType
		{
			None = 0,
			WorkStopped,
			ClientArrival,
			ClientRemoved,
			ClientPackageArrival
		};
		/// @brief ��Ϣ
		struct Message
		{
			MessageType Type;
			uint32_t SessionCount;
			union
			{
				ClientSession* Session;
			};

			Message& WithSessionCount(uint32_t c)
			{
				SessionCount = c;
				return *this;
			}
			Message()
				: Type(MessageType::None) {}
			Message(MessageType type)
				: Type(type) {}
			Message(MessageType type, ClientSession* session)
				: Type(type), Session(session) {}
		};
	private:
		ISocketFactory* m_pFactory;
		ILogger* m_pLogger;
		IServerListener* m_pListener;
		uint16_t m_Port;
		uint16_t m_MaxClients;

		SocketHandle m_Socket;

		// �߳�״̬
		std::atomic<bool> m_stopFlag;  // ֪ͨ�߳�ֹͣ�����ı��
		std::atomic<bool> m_bRunning;  // ָʾ������Ƿ�������
		std::shared_ptr<std::thread> m_tSocket;  // Socket���ʱ�������߳�
		std::shared_ptr<std::thread> m_tMsgWork;  // ��Ϣ�����߳�

		// ��Ϣ����
		ConcurrentQueue<Message> m_MsgQueue;
	protected:
		void socketThreadLoop()NKNOEXCEPT;
		void messageThreadLoop()NKNOEXCEPT;
	public:
		/// @brief ��ȡ������
		IServerListener* GetListener() { return m_pListener; }
		/// @brief ���ü�����
		void SetListener(IServerListener* pListener) { m_pListener = pListener; }
		/// @brief ��������
		/// @note  �첽����
		void Start();
		/// @brief ��������
		/// @note  �첽����
		void End();
		/// @brief �ȴ������߳���ֹ
		void Wait();
		/// @brief �Ƿ�������
		bool IsRunning()const throw();
	private:
		Server& operator=(const Server&);
		Server(const Server&);
	public:
		/// @brief ��ʼ������
		Server(ISocketFactory* pFactory, ILogger* pLogger, uint16_t port = 12801, uint16_t maxClient = 32);
	};
}