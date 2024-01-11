#pragma once
#include "Header.h"
#include <SFML/Network.hpp>
#include <SFML/System/Mutex.hpp>
#include <string>  
#include <thread>
#include <queue>

using namespace std;
using namespace sf;


class Client
{
private:

	TcpSocket* socket = new TcpSocket;
	Socket::Status clientStatus;
	const std::string IP_ADRESS = "127.0.0.1";
	const unsigned int PORT = 2100;
	int pid;
	thread threadBasicConnexion;
	thread threadSendMessage;
	thread threadReceiveMessage;
	queue<string> messagesToSend;
	Mutex mutex;


	// Functions
	void initConnection();
	void initVariables();



public:


	// Accessor and destructor
	Client();
	virtual ~Client();

	void sendMessage();
	void receiveMessage();

	void launchThreadConnexion();
};

