#include <cstdlib> // For exit() and EXIT_FAILURE
#include <sys/time.h>
#include <sys/types.h>

#include "../include/Socket.hpp"
#include "../include/Server.hpp"
#include "../include/HttpResponseParser.hpp"
#include "../include/Exceptions.hpp"
#include "../include/ExceptionManager.hpp"
#include "../include/HttpRequest.hpp"
#include "../include/HttpRequestParser.hpp"
#include "../include/HttpRequestHandler.hpp"
#include "../include/FileHandler.hpp"
#include "../include/ConfigParser.hpp"
#include "../include/Client.hpp"

std::vector<pollfd> pollfds;
std::map<int, Client*> _clients;

void	isCallValid(const int fd, const std::string errorMsg, int closeFd)
{
	if (fd < 0)
	{
		if (closeFd != -1)
			close(closeFd);
		throw PollException(errorMsg);
	}
}

void addNewPoll(int fd)
{
	pollfds.push_back({fd, POLLIN, 0});
}

void	handleNewClient(Socket *socket)
{
	int newClientFd = -1;
	while (1)
	{
		newClientFd = socket->acceptConnection();
		if (newClientFd < 0)
			break ;
		addNewPoll(newClientFd);
	}
}

void closeConnections()
{
	for (std::vector<pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); it++)
	{
		if (it->fd > 0)
		{
			close(it->fd);
			it->fd = -1;
		}
		pollfds.erase(it);
	}
}

void closeConnection(int fd)
{
	for (std::vector<pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); it++)
	{
		if (fd == it->fd)
		{
			if (it->fd > 0)
			{
				close(it->fd);
				it->fd = -1;
			}
			pollfds.erase(it);
			break;
		}
	}
}

std::string readRequest(int connection, unsigned int buffer_size)
{
	char buffer[buffer_size];
	std::string input;

	while (1)
	{
		int readBytes = recv(connection, buffer, sizeof(buffer), 0);
		if (readBytes < 0)
			break;
		if (readBytes == 0)
		{
			closeConnection(connection);
			break;
		}
		buffer[readBytes] = '\0';
		input.append(buffer);
	}

	return input;
}

void writeResponse(int connection, const std::string response)
{
	int result = send(connection, response.c_str(), response.size(), 0);
	if (result < 0)
		throw InternalException("Could not send response");
}

bool incomingClient(int fd, std::vector<Server>& servers)
{
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		Socket *socket = it->getSocket();
		if (fd == socket->getFd())
		{
			handleNewClient(socket);
			return true;
		}
	}

	return false;
}

void handleIncomingRequest(pollfd *fd)
{
	Client *client;
	std::map<int, Client*>::iterator it = _clients.find(fd->fd);
	if (it == _clients.end())
	{
		client = new Client();
		std::pair<std::map<int, Client*>::iterator, bool> result;
		result = _clients.insert(std::pair<int, Client*>(fd->fd, client));
		if (!result.second)
			throw BadRequestException("Connection already established with this client");
	}
	else
		client = it->second;

	std::string requestString = readRequest(fd->fd, 1000);
	HttpRequestParser requestParser;
	HttpRequest *request = requestParser.parseHttpRequest(requestString);
	client->setRequest(request);

	// TODO: separate to handler part
	HttpRequestHandler requestHandler;
	requestHandler.handleRequest(client);
	fd->events = POLLOUT;
}

void handleOutgoingResponse(pollfd *fd)
{
	std::map<int, Client*>::iterator it = _clients.find(fd->fd);
	if (it == _clients.end())
		return;
	writeResponse(fd->fd, HttpResponseParser::Parse(*it->second->getResponse()));
	_clients.erase(it);
}

void handleOutgoingError(const Exception& e, pollfd *fd)
{
	HttpResponse *response = new HttpResponse(ExceptionManager::getErrorStatus(e), "");

	std::map<int, Client*>::iterator it = _clients.find(fd->fd);
	if (it != _clients.end())
	{
		it->second->setResponse(response);
		fd->events = POLLOUT;
		return;
	}

	Client *client = new Client();
	client->setResponse(response);
	std::pair<std::map<int, Client*>::iterator, bool> result;
	result = _clients.insert(std::pair<int, Client*>(fd->fd, client));
	if (!result.second)
		throw BadRequestException("Connection already established with this client");
	fd->events = POLLOUT;
}

void handleRevent(pollfd *fd, void (*handlerFunc)(pollfd *))
{
	try
	{
		handlerFunc(fd);
	}
	catch (const Exception& e)
	{
		handleOutgoingError(e, fd);
	}
}

void handlePollEvents(std::vector<Server>& servers)
{
	for (unsigned long i = 0; i < pollfds.size(); i ++)
	{
		if (pollfds[i].revents == 0)
			continue;
		if (incomingClient(pollfds[i].fd, servers))
			continue;
		else if (pollfds[i].revents & POLLIN)
		{
			handleRevent(&pollfds[i], handleIncomingRequest);
			continue;
		}
		else if (pollfds[i].revents & POLLOUT)
			handleRevent(&pollfds[i], handleOutgoingResponse);
	}
}

void runServers(std::vector<Server>& servers)
{
	while (1)
	{
		// Wait max 3 minutes for incoming traffic
		int result = poll(pollfds.data(), pollfds.size(), CONNECTION_TIMEOUT);
		if (result == 0)
			throw TimeOutException("The program excited with timeout");
		else if (result < 0)
			throw PollException("Poll failed");
		handlePollEvents(servers);
	}

	closeConnections();
	pollfds.clear();
}


Server& initServer()
{
	Server *server = new Server();
	server->setHostIp("127.0.0.1");
	server->setListenPort(8000);
	server->setName("localhost");
	server->setClientMaxBodySize(1000);
	server->setSocket();
	addNewPoll(server->getSocket()->getFd());

	return *server;
}

// Candidate for removal after testing
void printVector(const std::vector<std::string>* vecPtr)
{
	if (vecPtr)
	{
		for (std::vector<std::string>::const_iterator it = vecPtr->begin(); it != vecPtr->end(); ++it)
		{
			std::cout << *it;
			if (it + 1 != vecPtr->end())
				std::cout << ", ";
		}
		std::cout << std::endl;
	}
}

// Going to be removed once the config -> server handoff works. Until them
// it only executes when there's no argv[1]
void dansTestFunc() {
	ConfigParser parser;
	parser.parseConfig("config/danTest.conf");

	// Access the servers
	const std::vector<Server>& servers = parser.getServers();
	for (std::vector<Server>::const_iterator it = servers.begin(); it != servers.end(); ++it) {
		const Server& server = *it;

		std::cout << "Methods: ";
		printVector(server.getLocationValue("/", "method"));
		printVector(server.getLocationValue("/tmp", "method"));
		printVector(server.getLocationValue("/cgi-bin", "directory"));
	}
}

int main(int argc, char **argv)
{
	if (argc != 2 || !argv[1])
	{	
		try
		{
			dansTestFunc();
			return 0;
		}
		catch(const std::logic_error& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	else
	{
		try
		{
			Server server = initServer();
			std::vector<Server> servers;
			servers.push_back(server);
			runServers(servers);

			servers.clear();
		}
		catch(const std::logic_error& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return 0;
}
