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

#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::find;
using std::string;
using std::vector;

using namespace std;

bool DoesExists(vector<string> vec, string str)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (str == vec[i])
        {
            return true;
        }
    }
    return false;
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swapString(string *xp, string *yp)
{
    string temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int string_to_int(string str)
{
    stringstream ss;
    int num;
    ss << str;
    ss >> num;
    return num;
}

int main(int argc, char const *argv[])
{
    int ID, port, unique_ID;
    vector<string> V;
    DIR *dir;
    struct dirent *diread;
    vector<char *> files;
    vector<string> filesItCon;
    //-----------------

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
            filesItCon.push_back(files[i]);
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
    //-----------------
    int buffer_IDs[neighbours];
    int buffer_UIDs[neighbours];
    int buffer_Ports[neighbours];
    int neighbours2 = 0;
    vector<int> neigh2_ids;
    vector<int> neigh2_uids;
    vector<int> neigh2_ports;

    vector<string> FilesTBS;
    vector<string> FilesTBS2;
    string FilesString;
    vector<string> neighboursFiles[neighbours];
    int n_files = string_to_int(V[3]);
    for (int kajal = 0; kajal < n_files; kajal++)
    {
        FilesTBS.push_back(V[4 + kajal]);
    }

    string prefinal_string = "";
    string plus = "=";
    for (int suman = 0; suman < filesItCon.size(); suman++)
    {
        if (suman == 0)
        {
            prefinal_string = prefinal_string.append(filesItCon[suman]);
        }
        else
        {
            prefinal_string = prefinal_string.append(plus).append(filesItCon[suman]);
        }
    }

    //-----------------

    int sockfd[neighbours], o_sockfd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char buffer2[1024] = {0};
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
        // if ((sockfd2[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        // {
        //     perror("socket failed");
        //     exit(EXIT_FAILURE);
        // }
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
    int i = 0;
    int new_socket, valread;
    while (!n_skts.empty())
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
            //cout << "received : " << buffer << " from " << neigh_ids[i]  << " at depth 1"<< endl;
            std::istringstream ss10(buffer);
            string token;
            vector<string> dest;
            if (strlen(buffer) != 0)
            {
                while (std::getline(ss10, token, '^'))
                {

                    dest.push_back(token);
                }
                buffer_IDs[i] = string_to_int(dest[0]);
                buffer_UIDs[i] = string_to_int(dest[1]);
                buffer_Ports[i] = string_to_int(dest[2]);

                std::istringstream ss11(dest[3]);
                string var2;
                while (std::getline(ss11, var2, '='))
                {
                    neighboursFiles[i].push_back(var2);
                }
            }

            string msg = to_string(ID).append("^").append(to_string(unique_ID)).append("^").append(to_string(port)).append("^").append(prefinal_string);
            int n = msg.length();
            // declaring character array
            char data[n + 1];

            // copying the contents of the
            // string to char array
            strcpy(data, msg.c_str());
            if (send(sockfd[i], data, strlen(data), 0) == -1)
            {
            }
            //cout << "sent : " << data << " from " << ID << " to : " << neigh_ids[i]  << " at depth 1"<< endl;
            std::vector<int>::iterator it;
            it = std::find(n_skts.begin(), n_skts.end(), neigh_ids[i]);
            if (it != n_skts.end())
            {
                n_skts.erase(it);
            }
        }
        else
        {
            if ((new_socket = accept(o_sockfd, (struct sockaddr *)&address,
                                     (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                continue;
                i--;
                exit(EXIT_FAILURE);
            }
            sockfd[i] = new_socket;
            string msg = to_string(ID).append("^").append(to_string(unique_ID)).append("^").append(to_string(port)).append("^").append(prefinal_string);
            int n = msg.length();

            // declaring character array
            char data[n + 1];

            // copying the contents of the
            // string to char array
            strcpy(data, msg.c_str());

            // std::find function call
            std::vector<int>::iterator it;
            it = std::find(n_skts.begin(), n_skts.end(), neigh_ids[i]);
            if (it != n_skts.end())
            {
                n_skts.erase(it);
            }
            if (send(sockfd[i], data, strlen(data), 0) == -1)
            {
            }
            if ((valread = recv(sockfd[i], buffer, 1024, 0)) <= 0)
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
            //cout << "received : " << buffer << " from " << neigh_ids[i]  << " at depth 1"<< endl;
            std::istringstream ss10(buffer);
            std::string var;
            string token;
            vector<string> dest;
            if (strlen(buffer) != 0)
            {
                while (std::getline(ss10, token, '^'))
                {

                    dest.push_back(token);
                }
                buffer_IDs[i] = string_to_int(dest[0]);
                buffer_UIDs[i] = string_to_int(dest[1]);
                buffer_Ports[i] = string_to_int(dest[2]);

                std::istringstream ss11(dest[3]);
                string var2;
                while (std::getline(ss11, var2, '='))
                {
                    neighboursFiles[i].push_back(var2);
                }
            }
            // cout << "before closing" << endl;
            // close(new_socket);
            // cout << "after closing" << endl;
        }
        i++;
    }
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
                neighboursFiles[q].swap(neighboursFiles[q + 1]);
            }
        }
    }

    for (p = 0; p < FilesTBS.size() - 1; p++)
    {

        for (q = 0; q < FilesTBS.size() - p - 1; q++)
        {
            int n = FilesTBS[q].length();
            int m = FilesTBS[q + 1].length();
            // declaring character array
            char data1[n + 1], data2[m + 1];

            // copying the contents of the
            // string to char array
            strcpy(data1, FilesTBS[q].c_str());
            strcpy(data2, FilesTBS[q + 1].c_str());
            if (strcmp(data1, data2) < 0)
            {
                swapString(&FilesTBS[q], &FilesTBS[q + 1]);
            }
        }
    }

    for (int x = 0; x < neighbours; x++)
    {
        cout << "Connected to " << buffer_IDs[x] << " with unique-ID " << buffer_UIDs[x] << " on port " << buffer_Ports[x] << endl;
        // close(sockfd[x]);
    }
    bool once = true;
    // cout << "FILES TO BE SEARCHED : " << endl;
    // for (int j = 0; j < FilesTBS.size(); j++)
    // {
    //     cout << FilesTBS[j] << endl;
    // }
    int no_of_filesTBS = FilesTBS.size();
    string p_filename[FilesTBS.size()];
    int p_uid[FilesTBS.size()];
    int p_depth[FilesTBS.size()];
    int count = 0;
    while (!FilesTBS.empty())
    {
        string filename = FilesTBS[FilesTBS.size() - 1];
        int uniq = 1000000;
        int freq = 0;
        for (int m = 0; m < neighbours; m++)
        {
            bool isPresent = DoesExists(neighboursFiles[m], filename);
            // for(int p = 0; p < neighboursFiles[m].size(); p++){
            //     cout << "neighbour : " << neigh_ids[m] << " filename : " << neighboursFiles[m][p] << endl;
            // }
            if (isPresent)
            {
                uniq = min(uniq, buffer_UIDs[m]);
                //cout << filename << " present in depth 1 at : " << neigh_ids[m] << endl;
            }
            else
            {
                //cout << filename << " NOT present in depth 1 at : " << neigh_ids[m] << endl;
                freq++;
                //cout << "max : " << neighbours << " freq : " << freq << endl;
                if (freq == neighbours)
                {
                    FilesTBS2.push_back(filename);
                }
                continue;
            }
        }
        if (uniq != 1000000)
        {
            p_filename[count] = filename;
            p_depth[count] = 1;
            p_uid[count] = uniq;
            count++;
            //cout << "Found " << filename << " at " << uniq << " with MD5 0 at depth 1" << endl;
        }
        FilesTBS.pop_back();
    }
    for (int i = 0; i < neighbours; i++)
    {
        if (ID < buffer_IDs[i])
        {
            if ((valread = recv(sockfd[i], buffer2, 1024, 0)) <= 0)
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
            std::istringstream ss10(buffer2);
            string token;
            vector<string> dest;
            if (strlen(buffer2) != 0)
            {
                int pqr = 0;
                while (std::getline(ss10, token, '_'))
                {
                    if (token == "")
                    {
                        pqr = 1;
                    }
                    else
                    {
                        dest.push_back(token);
                    }
                }
                if (pqr == 1)
                {
                    i--;
                    continue;
                }
                neighbours2 += string_to_int(dest[0]);

                int q = 1;
                while (q < 3 * (string_to_int(dest[0])) + 1)
                {
                    bool neigh2_is_neigh = false;
                    bool neigh2_is_twice = false;
                    if (string_to_int(dest[q]) != ID)
                    {
                        for (int r = 0; r < neighbours; r++)
                        {
                            if (string_to_int(dest[q]) == neigh_ids[r])
                            {
                                neigh2_is_neigh = true;
                            }
                        }
                        for (int r = 0; r < neigh2_ids.size(); r++)
                        {
                            if (string_to_int(dest[q]) == neigh2_ids[r])
                            {
                                neigh2_is_twice = true;
                            }
                        }

                        if (!neigh2_is_neigh && !neigh2_is_twice)
                        {
                            neigh2_ids.push_back(string_to_int(dest[q]));
                            neigh2_uids.push_back(string_to_int(dest[q + 1]));
                            neigh2_ports.push_back(string_to_int(dest[q + 2]));
                        }
                        else{
                            neighbours2--;
                        }
                    }
                    else
                    {
                        neighbours2--;
                    }
                    q = q + 3;
                }
            }
            string msg = to_string(neighbours);
            for (int q = 0; q < neighbours; q++)
            {
                msg = msg.append("_").append(to_string(buffer_IDs[q])).append("_").append(to_string(buffer_UIDs[q])).append("_").append(to_string(buffer_Ports[q]));
            }
            // string msg = to_string(ID).append("^").append(to_string(unique_ID)).append("^").append(to_string(port)).append("^").append(prefinal_string);
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
            string msg = to_string(neighbours);
            for (int q = 0; q < neighbours; q++)
            {
                msg = msg.append("_").append(to_string(buffer_IDs[q])).append("_").append(to_string(buffer_UIDs[q])).append("_").append(to_string(buffer_Ports[q]));
            }
            int n = msg.length();

            // declaring character array
            char data[n + 1];

            // copying the contents of the
            // string to char array
            strcpy(data, msg.c_str());

            // std::find function call
            if (send(sockfd[i], data, strlen(data), 0) == -1)
            {
            }
            if ((valread = recv(sockfd[i], buffer2, 1024, 0)) <= 0)
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
            std::istringstream ss10(buffer2);
            std::string var;
            string token;
            vector<string> dest;
            if (strlen(buffer2) != 0)
            {
                int pqr = 0;
                while (std::getline(ss10, token, '_'))
                {
                    if (token == "")
                    {
                        pqr = 1;
                    }
                    else
                    {
                        dest.push_back(token);
                    }
                }
                if (pqr == 1)
                {
                    i--;
                    continue;
                }
                neighbours2 += string_to_int(dest[0]);
                int q = 1;
                while (q < 3 * (string_to_int(dest[0])) + 1)
                {
                    bool neigh2_is_neigh = false;
                    bool neigh2_is_twice = false;
                    if (string_to_int(dest[q]) != ID)
                    {
                        for (int r = 0; r < neighbours; r++)
                        {
                            if (string_to_int(dest[q]) == neigh_ids[r])
                            {
                                neigh2_is_neigh = true;
                            }
                        }
                        for (int r = 0; r < neigh2_ids.size(); r++)
                        {
                            if (string_to_int(dest[q]) == neigh2_ids[r])
                            {
                                neigh2_is_twice = true;
                            }
                        }

                        if (!neigh2_is_neigh && !neigh2_is_twice)
                        {
                            neigh2_ids.push_back(string_to_int(dest[q]));
                            neigh2_uids.push_back(string_to_int(dest[q + 1]));
                            neigh2_ports.push_back(string_to_int(dest[q + 2]));
                        }
                        else{
                            neighbours2--;
                        }
                    }
                    else
                    {
                        neighbours2--;
                    }
                    q = q + 3;
                }
            }
        }
    }
    for (int j = 0; j < neighbours; j++)
    {
        close(sockfd[i]);
    }
    int sockfd2[neighbours2], new_socket2;
    char buffer3[1024] = {0};
    vector<string> neighbours2Files[neighbours2];
    for (int i = 0; i < neighbours2; i++)
    {
        if ((sockfd2[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < neighbours2; i++)
    {

        if (ID < neigh2_ids[i])
        {
            // cout << "Acting as a client, ID : " << neigh2_ids[i] << " Unique ID : " << neigh2_uids[i] << " port : " << neigh2_ports[i] << endl;
            struct sockaddr_in neigh2_address;
            int valread;
            int neigh2_addrlen = sizeof(neigh2_address);
            neigh2_address.sin_family = AF_INET;
            neigh2_address.sin_addr.s_addr = INADDR_ANY;
            neigh2_address.sin_port = htons(neigh2_ports[i]);
            if (connect(sockfd2[i], (struct sockaddr *)&neigh2_address,
                        sizeof(neigh2_address)) < 0)
            {
                i--;
                continue;
            }
            // cout << "i : " << i << endl;
            if ((valread = recv(sockfd2[i], buffer3, 1024, 0)) <= 0)
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
            //cout << "received : " << buffer3 << " from " << neigh2_ids[i]  << " at depth 2"<< endl;
            std::istringstream ss10(buffer3);
            string token;
            vector<string> dest;
            if (strlen(buffer3) != 0)
            {
                while (std::getline(ss10, token, '='))
                {
                    neighbours2Files[i].push_back(token);
                }
            }

            string msg = prefinal_string;
            int n = msg.length();
            // declaring character array
            char data[n + 1];

            // copying the contents of the
            // string to char array
            strcpy(data, msg.c_str());
            if (send(sockfd2[i], data, strlen(data), 0) == -1)
            {
            }
        }
        else
        {
            // cout << "Acting as a server, ID : " << neigh2_ids[i] << " Unique ID : " << neigh2_uids[i] << " port : " << neigh2_ports[i] << endl;
            if ((new_socket2 = accept(o_sockfd, (struct sockaddr *)&address,
                                      (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                i--;
                continue;
                exit(EXIT_FAILURE);
            }
            // cout << "i : " << i << endl;
            sockfd2[i] = new_socket2;
            string msg = prefinal_string;
            int n = msg.length();

            // declaring character array
            char data[n + 1];

            // copying the contents of the
            // string to char array
            strcpy(data, msg.c_str());

            if (send(new_socket2, data, strlen(data), 0) == -1)
            {
            }
            if ((valread = recv(new_socket2, buffer3, 1024, 0)) <= 0)
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
            //cout << "received : " << buffer3 << " from " << neigh2_ids[i] << " at depth 2" << endl;
            std::istringstream ss10(buffer3);
            std::string var;
            string token;
            vector<string> dest;
            if (strlen(buffer3) != 0)
            {
                while (std::getline(ss10, token, '='))
                {
                    neighbours2Files[i].push_back(token);
                }
            }
        }
        close(new_socket2);
    }
    while (!FilesTBS2.empty())
    {
        string filename = FilesTBS2[FilesTBS2.size() - 1];
        int uniq = 1000000;
        for (int m = 0; m < neighbours2; m++)
        {
            bool isPresent = DoesExists(neighbours2Files[m], filename);
            if (isPresent)
            {
                uniq = min(uniq, neigh2_uids[m]);
            }
            else if (!isPresent)
            {
                continue;
            }
        }
        if (uniq != 1000000)
        {
            p_filename[count] = filename;
            p_depth[count] = 2;
            p_uid[count] = uniq;
            count++;

            //cout << "Found " << filename << " at " << uniq << " with MD5 0 at depth 2" << endl;
        }
        else
        {
            p_filename[count] = filename;
            p_depth[count] = 0;
            p_uid[count] = 0;
            count++;
            //cout << "Found " << filename << " at " << 0 << " with MD5 0 at depth 0" << endl;
        }
        FilesTBS2.pop_back();
    }

    for (p = 0; p < no_of_filesTBS - 1; p++)
    {

        for (q = 0; q < no_of_filesTBS - p - 1; q++)
        {
            int n = p_filename[q].length();
            int m = p_filename[q + 1].length();
            // declaring character array
            char data1[n + 1], data2[m + 1];

            // copying the contents of the
            // string to char array
            strcpy(data1,p_filename[q].c_str());
            strcpy(data2, p_filename[q + 1].c_str());
            if (strcmp(data1, data2) > 0)
            {
                swapString(&p_filename[q], &p_filename[q + 1]);
                swap(&p_depth[q], &p_depth[q+1]);
                swap(&p_uid[q], &p_uid[q+1]);
            }
        }
    }
    for(int k = 0; k < no_of_filesTBS; k++){
        cout << "Found " << p_filename[k] << " at " << p_uid[k] << " with MD5 0 at depth " << p_depth[k] << endl;
    }




}