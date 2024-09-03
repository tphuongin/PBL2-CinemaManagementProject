#include <iostream>
#include <string>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Them thu vien Winsock

void sendRequest(SOCKET connectSocket, const std::string& request) {
    if (send(connectSocket, request.c_str(), (int)request.length(), 0) == SOCKET_ERROR) {
        std::cerr << "Goi du lieu that bai!\n";
    }
}

std::string receiveResponse(SOCKET connectSocket) {
    char buffer[512];
    int recvbuflen = 512;
    int bytesReceived = recv(connectSocket, buffer, recvbuflen, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        return std::string(buffer);
    }
    return "";
}

int main() {
    WSADATA wsaData;
    SOCKET connectSocket = INVALID_SOCKET;
    sockaddr_in clientService;

    // Khoi tao Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Khoi tao Winsock that bai!\n";
        return 1;
    }

    // Tao socket
    connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET) {
        std::cerr << "Khong the tao socket!\n";
        WSACleanup();
        return 1;
    }

    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("192.168.219.163"); // Dia chi IP cua server
    clientService.sin_port = htons(8080); // Cong cua server

    // Ket noi den server
    if (connect(connectSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        std::cerr << "Ket noi den server that bai!\n";
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Da ket noi den server!\n";

    while (true) {
        std::cout << "Chuong trinh Client - Ban ve:\n";
        std::cout << "1. Xem danh sach su kien\n";
        std::cout << "2. Dat ve\n";
        std::cout << "3. Huy ve\n";
        std::cout << "4. Thoat\n";
        std::cout << "Chon mot tuy chon: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Bo qua ky tu newline con lai trong buffer

        if (choice == 1) {
            sendRequest(connectSocket, "LIST_EVENTS\n");
            std::cout << receiveResponse(connectSocket);
        } else if (choice == 2) {
            std::cout << "Nhap so su kien muon dat ve: ";
            int eventNumber;
            std::cin >> eventNumber;
            std::cin.ignore(); // Bo qua ky tu newline con lai trong buffer
            std::cout << "Nhap ten khach hang: ";
            std::string customerName;
            std::getline(std::cin, customerName);

            std::string request = "BOOK_TICKET " + std::to_string(eventNumber) + " " + customerName + "\n";
            sendRequest(connectSocket, request);
            std::cout << receiveResponse(connectSocket);
        } else if (choice == 3) {
            std::cout << "Nhap ma ve can huy: ";
            int ticketID;
            std::cin >> ticketID;
            std::cin.ignore(); // Bo qua ky tu newline con lai trong buffer

            std::string request = "CANCEL_TICKET " + std::to_string(ticketID) + "\n";
            sendRequest(connectSocket, request);
            std::cout << receiveResponse(connectSocket);
        } else if (choice == 4) {
            break;
        } else {
            std::cout << "Tuy chon khong hop le. Vui long chon lai.\n";
        }
    }

    closesocket(connectSocket);
    WSACleanup();
    return 0;
}
