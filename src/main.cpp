#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "../include/Socket.hpp"
#include "../include/Server.hpp"
#include "../include/Exceptions.hpp"
#include "../include/ExceptionHandler.hpp"
#include "../include/HttpRequest.hpp"
#include "../include/HttpRequestParser.hpp"

#define TIMEOUT 180000

// Server* initServer()
// {
// 	Server *server = new Server();
// 	server->setHostIp("127.0.0.1");
// 	server->setListenPort(8000);
// 	server->setName("localhost");
// 	server->setRoot("/");
// 	server->setClientMaxBodySize(1000);

// 	return server;
// }

// void	isCallValid(const int fd, const std::string errorMsg, int closeFd)
// {
// 	if (fd < 0)
// 	{
// 		std::cerr << errorMsg << std::endl;
// 		if (closeFd != -1)
// 			close(closeFd);
// 	}
// }

// pollfd	*addNewPoll(pollfd *fds, int size, int fd, short events)
// {
// 	struct pollfd *newFds = new pollfd[size + 1];

// 	if (fds != NULL)
// 	{
// 		for (int i = 0; i < size; i++)
// 			newFds[i] = fds[i];
// 		delete [] fds;
// 	}
// 	newFds[size].fd = fd;
// 	newFds[size].events = events;

// 	return newFds;
// }

// void	handleNewClient(int *numberOfFds, Socket *socket, pollfd **fds)
// {
// 	int newClientFd = -1;
// 	while (1)
// 	{
// 		newClientFd = socket->acceptConnection();
// 		if (newClientFd < 0)
// 			break;
// 		*fds = addNewPoll(*fds, *numberOfFds, newClientFd, POLLIN);
// 		*numberOfFds += 1;
// 	}
// }

// void runServer(Server *server)
// {
// 	Socket *socket = new Socket(server->getListenPort());

// 	// Initialize poll struct for sockets and clients
// 	int numberOfFds = 1, currentFdsSize = 0, socketFd = socket->getFd();
// 	struct pollfd *fds = NULL;
// 	fds = addNewPoll(fds, currentFdsSize, socketFd, POLLIN);
// 	while (1)
// 	{
// 		// Wait max 3 minutes for incoming traffic
// 		int result = poll(fds, numberOfFds, TIMEOUT);
// 		if (result <= 0)
// 			break;

// 		currentFdsSize = numberOfFds;
// 		std::string request;
// 		for (int i = 0; i < currentFdsSize; i ++)
// 		{
// 			if (fds[i].revents == 0)
// 				continue;
// 			if (fds[i].revents != POLLIN)
// 				break; // TODO this is an error?
// 			if (fds[i].fd == socketFd)
// 				handleNewClient(&numberOfFds, socket, &fds);
// 			else
// 			{
// 				try
// 				{
// 					request = socket->readRequest(fds[i].fd, server->getClientMaxBodySize());
// 					if (request.compare("Q\r\n") == 0)
// 						break;
// 					// TODO: handle request
// 					std::cout << "Request from '" << i << "' was: " << request;
// 					socket->writeResponse(fds[i].fd, "HTTP/1.1 200 OK\r\n");
// 				}
// 				catch (const Exception& e)
// 				{
// 					HttpResponse response("something", 30, "txt/html");
// 					response.setStatus(ExceptionHandler::getErrorStatus(e));
// 					std::cout << response._status.first << ", " << response._status.second << std::endl;
// 					break ;
// 				}
// 			}
// 		}

// 		if (request.compare("Q\r\n") == 0)
// 			break;
// 	}


// 	socket->closeConnections(fds, currentFdsSize);
// 	delete [] fds;
// 	delete socket;
// }

// int main()
// {
// 	try
// 	{
// 		Server *server = initServer();
// 		runServer(server);

// 		delete server;
// 	}
// 	catch(const std::exception& e)
// 	{
// 		// TODO clean up
// 		std::cout << e.what() << '\n';
// 	}

// 	return 0;
// }

int main ()
{
	std::string input = "GET /user/moi HTTP/1.1\nbodybodybody\nblablablalbaa\n\r\n";
	HttpRequestParser requestParser;
	HttpRequest theRequest = requestParser.parseHttpRequest(input);

	std::cout << theRequest.getMethod() << std::endl;
	std::cout << theRequest.getUri() << std::endl;
	std::cout << theRequest.getVersion() << std::endl;

}
