//
////adresse_socket_serveur = ("127.0.0.1", 8088)
////socket_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
////socket_client.connect(adresse_socket_serveur)
////print("Client connect�")
////socket_client.send(("Client � serveur : j'utilise le PID " + str(os.getpid())).encode("utf-8"))
////donnees_recues_du_serveur = socket_client.recv(4096)
////print(donnees_recues_du_serveur.decode("utf-8"))
////print("Connexion ferm�e")
////socket_client.close()
//
#include <stdio.h>
#include <string>  
#include <sstream>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <iterator>
#include <SFML/Network.hpp>
#include <Windows.h>


using namespace std;
using namespace sf;

int main() {

	sf::TcpSocket socket;
	socket.connect("127.0.0.1", 8088);
	std::cout << "Client connect�"  << endl;
	/*std::cout << "Port local:" + socket.getLocalPort() << endl;*/
	

	unsigned int pid = GetCurrentProcessId();
	
	string messageToSend;
	messageToSend += "Client a serveur. J'utilise le pid : " + to_string(pid);
	
	unsigned char data[100] = { 0 };

	std::copy(messageToSend.begin(), messageToSend.end(), data);

	socket.send(data, 100);

	char dataReceived[100] = { 0 };
	std::size_t received;

	if (socket.receive(dataReceived, 100, received) != sf::Socket::Done)
	{
		std::cout << "Erreur r�ception de donn�es" << endl;
	}
	std::cout << "Received:" << received << " bytes" << std::endl;
	std::cout << "Received:" << dataReceived << " bytes" << std::endl;


	return 0;
}

//#include <SFML/Graphics.hpp>
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }
//
//    return 0;
//}