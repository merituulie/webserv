#include "Socket.hpp"

Socket::Socket(void)
{
}

void	Socket::isCallValid(const int fd, const std::string errorMsg, int closeFd)
{
	if (fd < 0)
	{
		std::cerr << errorMsg << std::endl;
		if (closeFd != -1)
			close(closeFd);
	}
}

Socket::Socket(const int portNumber) : fd(-1)
{
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	isCallValid(this->fd, "Failed to create the socket", -1);

	int opt = 1;
	setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->address.sin_port = htons(portNumber);

	int result = bind(this->fd, (struct sockaddr*)&this->address, sizeof(this->address));
	isCallValid(result, "Failed to bind to port", this->fd);

	result = listen(this->fd, 10);
	isCallValid(result, "Failed to listen on socket", this->fd);
}

Socket::~Socket(void)
{
	close(this->fd);
}

Socket::Socket(const Socket &rhs) : fd(rhs.fd)
{
}

int Socket::acceptConnection()
{
	size_t socketSize = sizeof(this->address);
	int connection = accept(this->fd, (struct sockaddr*)&this->address, (socklen_t*)&socketSize);
	isCallValid(connection, "Failed to accept connection", this->fd);

	return connection;
}

const std::string Socket::readRequest(int connection, unsigned int buffer_size) const
{
	char buffer[buffer_size];

	int result = read(connection, buffer, buffer_size);
	isCallValid(result, "Failed to read request", -1);
	buffer[result] = '\0';
	std::string input(buffer);

	return input;
}

void Socket::writeResponse(int connection, const std::string response) const
{
	int result = write(connection, response.c_str(), response.size());
	isCallValid(result, "Failed to send response", -1);
}

int Socket::getFd() const
{
	return this->fd;
}

Socket &Socket::operator=(const Socket &rhs)
{
	if (this != &rhs)
		this->fd = rhs.fd;

	return *this;
}
