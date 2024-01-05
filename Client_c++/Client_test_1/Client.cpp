#include "Client.h"

#define CONDITIONS (this->clientStatus != Socket::Disconnected) && (this->clientStatus != Socket::Error)


Client::Client()
{
	initVariables();
	launchThreadConnexion();
}

Client::~Client()
{
	// ~Socket(socket);
}

void Client::initConnection()
{
	float time = 10;
	Time timeout = seconds(time);
	
	while (socket->connect("127.0.0.1", 2100, timeout) != Socket::Done) {
		cout << "En attente d'une connexion avec le serveur" << endl;
	}
	
	cout << "Client connecté" << endl;
	

	string messageToSend;
	messageToSend += "Client a serveur. J'utilise le pid : " + to_string(pid);

	messagesToSend.push(messageToSend);

	threadSendMessage = thread(&Client::sendMessage, this);

	threadReceiveMessage = thread(&Client::receiveMessage, this);

}

void Client::initVariables()
{
	this->pid = GetCurrentProcessId();
}

void Client::sendMessage()
{
	while (CONDITIONS) {

		while (!messagesToSend.empty()) {
			unsigned char data[100] = { 0 };
			string messageToSend = this->messagesToSend.front();

			copy(messageToSend.begin(), messageToSend.end(), data);

			this->mutex.lock();
			socket->send(data, 100);
			messagesToSend.pop();
			this->mutex.unlock();
		}

	}

}

void Client::receiveMessage()
{
	
	char dataReceived[100] = { 0 };
	size_t received;
	string messageReceived;
	
	while (CONDITIONS) {
		this->clientStatus = socket->receive(dataReceived, 100, received);
		if (CONDITIONS)
		{
			this->mutex.lock();
			messageReceived.assign(dataReceived);
			cout << "Message recu : " << messageReceived << std::endl;
			this->mutex.unlock();
		}
		else {
			cout << "En attente d'une réception de données" << endl;
		}

	}

}

void Client::launchThreadConnexion()
{
	this->threadBasicConnexion = thread(&Client::initConnection, this);
}
