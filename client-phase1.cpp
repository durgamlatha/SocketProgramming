#include <string>
#include <iostream>
#include <sys/socket.h>
#include <filesystem>
#include <fstream>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <sstream>
#include <dirent.h>
#include <queue>
#include <bits/stdc++.h>

using namespace std;

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int main(int argc, char const *argv[])
{
    int ID, port, unique_ID;
    vector<string> V;
    DIR *dir;
    struct dirent *diread;
    vector<char *> files;

    if ((dir = opendir(argv[2])) != nullptr)
    {
        while ((diread = readdir(dir)) != nullptr)
        {
            files.push_back(diread->d_name);
        }
        closedir(dir);
    }

    for (int i = 2; i < files.size(); i++)
    {
        if (strcmp(files[i], "Downloads") != 0)
        {
            cout << files[i] << "\n";
            //filesItCon.push_back(files[i]);
        }
    }
    std::ifstream myfile(argv[1]);
    string myline;
    int neighbours;
    vector<int> neigh_ids, neigh_ports;

    if (myfile.is_open())
    {
        while (myfile)
        { // equivalent to myfile.good()
            std::getline(myfile, myline);
            V.push_back(myline);
        }
        stringstream ss1(V[0]);
        int number, count = 0;
        while (ss1 >> number)
        {
            if (count == 0)
            {
                ID = number;
            }
            else if (count == 1)
            {
                port = number;
            }
            else if (count == 2)
            {
                unique_ID = number;
            }
            count++;
        }
        int i = 0, j = 0;
        count = 0;
        int y = stoi(V[1]);
        neighbours = y;
        int adj_ids[y], adj_ports[y];
        vector<int> line3;
        stringstream ss3(V[2]);
        while (ss3 >> number)
        {
            line3.push_back(number);
        }
        for (int i = 0; i < 2 * y; i++)
        {
            if (i % 2 == 0)
            {
                adj_ids[i / 2] = line3[i];
                neigh_ids.push_back(adj_ids[i / 2]);
            }
            else
            {
                adj_ports[i / 2] = line3[i];
                neigh_ports.push_back(adj_ports[i / 2]);
            }
        }
    }
    int buffer_IDs[neighbours]; //--------------------------------------
    int buffer_UIDs[neighbours];
    int buffer_Ports[neighbours];
    int sockfd[neighbours], o_sockfd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    vector<int> n_skts;
    if ((o_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < neighbours; i++)
    {
        if ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        n_skts.push_back(neigh_ids[i]);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(o_sockfd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(o_sockfd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int new_socket, valread;
    for (int i = 0; i < neighbours; i++)
    {
        if (ID < neigh_ids[i])
        {
            struct sockaddr_in neigh_address;
            int valread;
            int neigh_addrlen = sizeof(neigh_address);
            neigh_address.sin_family = AF_INET;
            neigh_address.sin_addr.s_addr = INADDR_ANY;
            neigh_address.sin_port = htons(neigh_ports[i]);
            if (connect(sockfd[i], (struct sockaddr *)&neigh_address,
                        sizeof(neigh_address)) < 0)
            {
                i--;
                continue;
            }
            if ((valread = recv(sockfd[i], buffer, 1024, 0)) <= 0)
            {
                // got error or connection closed by client
                if (valread == 0)
                {
                    // connection closed
                }
                else
                {
                }
                close(i); // bye!
            }
            std::istringstream ss10(buffer);
            std::string var;
            int token;
            vector<int> dest;
            if (strlen(buffer) != 0)
            {
                while (std::getline(ss10, var, '_'))
                {
                    stringstream obj;
                    obj << var; // insert data into obj
                    obj >> token;

                    dest.push_back(token);
                }
                buffer_IDs[i] = dest[0];
                buffer_UIDs[i] = dest[1];
                buffer_Ports[i] = dest[2];
            }

            string msg = to_string(ID).append("_").append(to_string(unique_ID)).append("_").append(to_string(port));
            int n = msg.length();

            // declaring character array
            char data[n + 1];

            // copying the contents of the
            // string to char array
            strcpy(data, msg.c_str());
            if (send(sockfd[i], data, strlen(data), 0) == -1)
            {
            }
        }
        else
        {
            if ((new_socket = accept(o_sockfd, (struct sockaddr *)&address,
                                     (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                i--;
                continue;

                exit(EXIT_FAILURE);
            }
            sockfd[i] = new_socket;
            string msg = to_string(ID).append("_").append(to_string(unique_ID)).append("_").append(to_string(port));
            int n = msg.length();

            // declaring character array
            char data[n + 1];

            // copying the contents of the
            // string to char array
            strcpy(data, msg.c_str());
            if (send(new_socket, data, strlen(data), 0) == -1)
            {
            }
            if ((valread = recv(new_socket, buffer, 1024, 0)) <= 0)
            {
                // got error or connection closed by client
                if (valread == 0)
                {
                    // connection closed
                    printf("selectserver: socket %d hung up\n", i);
                }
                else
                {
                }
                close(i); // bye!
            }
            std::istringstream ss10(buffer);
            std::string var;
            int token;
            vector<int> dest;
            if (strlen(buffer) != 0)
            {
                while (std::getline(ss10, var, '_'))
                {
                    stringstream obj;
                    obj << var; // insert data into obj
                    obj >> token;

                    dest.push_back(token);
                }
                buffer_IDs[i] = dest[0];
                buffer_UIDs[i] = dest[1];
                buffer_Ports[i] = dest[2];
            }
        }
        close(new_socket);
    }
    //---------------------------------------------------
    int p, q;

    for (p = 0; p < neighbours - 1; p++)
    {

        for (q = 0; q < neighbours - p - 1; q++)
        {
            if (buffer_IDs[q] > buffer_IDs[q + 1])
            {
                swap(&buffer_IDs[q], &buffer_IDs[q + 1]);
                swap(&buffer_UIDs[q], &buffer_UIDs[q + 1]);
                swap(&buffer_Ports[q], &buffer_Ports[q + 1]);
            }
        }

        // IF no two elements were swapped by inner loop, then break
    }

    for (int x = 0; x < neighbours; x++)
    {
        cout << "Connected to " << buffer_IDs[x] << " with unique-ID " << buffer_UIDs[x] << " on port " << buffer_Ports[x] << endl;
    }
    //---------------------------------------------------
}