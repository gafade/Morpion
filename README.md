# General description
This is the implementation of a multiplayer network tic-tac-toe game. 
There could be more than 2 players at the same time playing multiple games.


# The client
It is implemented in C++. 
For the graphical user interface and the network protocol, the SFML library is used.

# The server
It is implemented in Python.
The server is controlling the clients (the players). 
It can create and host multiple tic-tac-toe games simultaneously using different threads.


