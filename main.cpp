#include <iostream>
#include <fstream>
#include "playlist.h"

int main() {
    const std::string filename = "file.txt";

    std::ofstream check_file(filename, std::ios::app);
    if (!check_file.is_open()) {
        std::cout << "Error creating file.txt\n";
        return 1;
    }
    check_file.close();

    PlaylistManager manager;
    manager.load_from_file(filename);

    int choice;
    do {
        std::cout << "\nPlaylist Management System\n";
        std::cout << "1. Create playlist\n";
        std::cout << "2. View all playlists\n";
        std::cout << "3. Delete playlist\n";
        std::cout << "4. Manage playlist\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                menu_clear();
            manager.create_playlist();
            manager.save_to_file(filename);
            break;
            case 2:
                menu_clear();
            manager.view_playlists();
            break;
            case 3:
                menu_clear();
            manager.delete_playlist();
            manager.save_to_file(filename);
            break;
            case 4:
                menu_clear();
            manager.manage_playlist();
            manager.save_to_file(filename);
            break;
            case 5:
                manager.save_to_file(filename);
            break;
            default:
                std::cout << "Invalid choice\n";
            std::string dummy;
            std::getline(std::cin, dummy);
        }
    } while (choice != 5);

    return 0;
}