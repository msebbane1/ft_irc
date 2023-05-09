#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

int main() {
    // create a TCP/IP socket for the server
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8000);
    
    bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    // list of connected clients
    vector<int> client_list;
    client_list.push_back(server_socket);

    while (true) {
        // use select() to monitor the sockets for any incoming data
        fd_set read_sockets;
        FD_ZERO(&read_sockets);
        int max_fd = server_socket;
        for (int i = 0; i < client_list.size(); i++) {
            int fd = client_list[i];
            FD_SET(fd, &read_sockets);
            max_fd = max(max_fd, fd);
        }

        int ready = select(max_fd + 1, &read_sockets, NULL, NULL, NULL);
        if (ready < 0) {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < client_list.size(); i++) {
            int sock = client_list[i];

            // new connection
            if (sock == server_socket && FD_ISSET(sock, &read_sockets)) {
                int client_socket = accept(server_socket, NULL, NULL);
                client_list.push_back(client_socket);
                cout << "New client connected: " << client_socket << endl;
            }

            // incoming data from a client
            else if (FD_ISSET(sock, &read_sockets)) {
                char buffer[1024] = {0};
                int bytes_recv = recv(sock, buffer, 1024, 0);
                if (bytes_recv <= 0) {
                    close(sock);
                    client_list.erase(client_list.begin() + i);
                    continue;
                }

                // process the incoming data and send it back to all clients
                string message = to_string(sock) + ": " + buffer;
                for (int j = 0; j < client_list.size(); j++) {
                    int client = client_list[j];
                    if (client != server_socket && client != sock) {
                        int bytes_sent = send(client, message.c_str(), message.size(), 0);
                        if (bytes_sent == -1) {
                            close(client);
                            client_list.erase(client_list.begin() + j);
                            continue;
                        }
                    }
                }
            }
        }
    }

    return 0;
}