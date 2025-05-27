#include <stdio.h>
#include <stdlib.h>
#include "playlist.h"

int playlist_exists(char* name);
void free_song_list(SongNode* head);
int compare_playlists(const void* a, const void* b);
void view_playlists();
Playlist* select_playlist();
void create_playlist();
void delete_playlist();
void add_song(Playlist* playlist);
void remove_song(Playlist* playlist);
void view_songs(Playlist* playlist);
void manage_playlist();

PlaylistNode* playlist_head = NULL;

int main() {
    const char* filename = "file.txt";


    FILE* check_file = fopen(filename, "a");
    if (check_file == NULL) {
        printf("Error creating file.txt\n");
        return 1;
    }
    fclose(check_file);

    load_playlists_from_file(filename);

    int choice;
    do {
        printf("\nPlaylist Management System\n");
        printf("1. Create playlist\n");
        printf("2. View all playlists\n");
        printf("3. Delete playlist\n");
        printf("4. Manage playlist\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                menu_clear();
                create_playlist();
                save_playlists_to_file(filename);
                break;
            case 2:
                menu_clear();
                view_playlists();
                break;
            case 3:
                menu_clear();
                delete_playlist();
                save_playlists_to_file(filename);
                break;
            case 4:
                menu_clear();
                manage_playlist();
                save_playlists_to_file(filename);
                break;
            case 5:
                save_playlists_to_file(filename);
                break;
            default:
                printf("Invalid lol");
                getchar();
        }
    } while (choice != 5);


    PlaylistNode* current = playlist_head;
    while (current != NULL) {
        PlaylistNode* next = current->next;
        free_song_list(current->playlist.songs);
        free(current);
        current = next;
    }
    return 0;
}