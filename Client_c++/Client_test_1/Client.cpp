#include "Client.h"



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
	
	while (socket->connect("127.0.0.1", 2100, timeout) != Socket::Done) {//127.0.0.1
		cout << "En attente d'une connexion avec le serveur" << endl;
	}

	cout << "Client connecté" << endl;
	
	string messageToSend;

	cout << "Entrez votre pseudo" << endl;

	cin >> messageToSend;

	// Envoyez le pseudo au serveur

	addMessageToQueue("0:10:0:"+messageToSend+":Unknown");

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
			this->mutexSending.lock();
			
			unsigned char data[100] = { 0 };
			string messageToSend = this->messagesToSend.front();
			copy(messageToSend.begin(), messageToSend.end(), data);

			socket->send(data, 100);
			messagesToSend.pop();

			this->mutexSending.unlock();
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
			this->mutexReceiving.lock();
			messageReceived.assign(dataReceived);
			this->messagesReceived.push(messageReceived);
			cout << "Message recu : " << messageReceived << std::endl;
			this->mutexReceiving.unlock();
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

void Client::addMessageToQueue(string newMessage)
{
	this->mutexSending.lock();
	this->messagesToSend.push(newMessage);
	this->mutexSending.unlock();
}
