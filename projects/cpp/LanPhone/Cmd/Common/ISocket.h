// ISocket.h: interface for the IClient class.
//
//////////////////////////////////////////////////////////////////////

class ISocket
{
public:
	ISocket();
	virtual ~ISocket();
	virtual void Init(int iSrcPort, int iDestPort, char pszIP[]) {}

private:
	int m_iSourcePort;
	int m_iDestPort;
	char m_szIPAddress[15];
};

class IClient : private ISocket
{
public:
	IClient();
	virtual ~IClient();
	virtual int SendData(const char *buf, int size) {}
};

class IServer : private ISocket
{
public:
	IServer();
	virtual ~IServer();
	virtual BOOL Start() {}
	virtual void SetOnReceiveData(void (*func)(const char*, int)) {}

private:
	void (*m_OnReceiveData)(const char *data, int size);
};
