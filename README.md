# weberver 🌐

This project implements an **HTTP server** based on the *Hypertext Transfer Protocol (HTTP)* specifications, focusing on serving static websites and handling dynamic content through *CGI* (Common Gateway Interface), and supporting various **HTTP methods** like **GET**, **POST**, and **DELETE**.

## Introduction 🚀

The *Hypertext Transfer Protocol (HTTP)* is fundamental for distributed, collaborative, hypermedia information systems, forming the backbone of communication for the World Wide Web. HTTP facilitates the transfer of hypertext documents, allowing users to access resources through hyperlinks via web browsers.

## Features 📋

- **Configuration File Handling**: The program accepts a configuration file .config as an argument or uses a default configuration file.
- **Compatibility**: The server is compatible with Google Chrome and adheres to HTTP version 1.1 standards.
- **Error Handling**: Proper *HTTP response status codes* and default error pages are provided.
- **HTTP Methods**: Supports **GET**, **POST**, and **DELETE** methods.
- **File Upload**: Clients are able to upload files.
- **Configuration Flexibility**: The configuration file allows for setting up servers, defining routes, limiting client body size, directory listing, setting up where to upload files, etc.
- **CGI Execution**: Server allows executing python based *CGI scripts*.
- **Resilience**: The server remains available under stress and does not hang forever on requests.

## Configuration File ⚙️

The configuration file resembles **NGINX's** configuration format, allowing users to specify server settings, routes, error pages, etc. Some key features of the configuration file include:

- Choosing port and host for each server.
- Defining server names and default servers.
- Setting up custom error pages (default error pages provided).
- Limiting client body size.
- Configuring routes with accepted HTTP methods, redirections, file directories, CGI execution, etc.

## Conclusion 🎉

This **HTTP server implementation** aims to provide a resilient, feature-rich solution for serving web content while adhering to HTTP standards and ensuring compatibility across different platforms and browsers.

# Setting Up the Project with Makefile

## 1. Clone the Repository 📁

- git clone <repository_url>

## 2. Set Up the Project 🚀

- Make sure you have a C++ compiler installed (preferably \`g++\`).
- Navigate to the project directory.

## 3. Makefile Usage 🛠️

- **\`make all\`**: Compiles the project.
- **\`make clean\`**: Removes all object files.
- **\`make fclean\`**: Removes all object files and the executable.
- **\`make re\`**: Performs a clean build (removes all object files and recompiles).

## 4. Compilation 🖥️

- Run \`make all\` to compile the project.
- The compiled executable will be named \`webserv\`.

## 5. Customization 🛠️

- Adjust \`CFLAGS\` in the Makefile for any specific compilation flags you require.
- Modify source files (\`*.cpp\`) and header files (\`*.hpp\`) in the \`src/\` and \`include/\` directories respectively to customize the project.

## 6. Compilation Output 💻

- The compiled executable \`webserv\` will be created in the project directory upon successful compilation.
