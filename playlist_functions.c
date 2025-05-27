#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "playlist.h"

void menu_clear(void) {
    #ifdef _WIN32
        system("CLS");
    #else
        system("clear");
    #endif
}

void read_input(const char* prompt, char* buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

int playlist_exists(char* name) {
    PlaylistNode* current = playlist_head;
    while (current != NULL) {
        if (strcmp(current->playlist.name, name) == 0) return 1;
        current = current->next;
    }
    return 0;
}

void free_song_list(SongNode* head) {
    SongNode* current = head;
    while (current != NULL) {
        SongNode* next = current->next;
        free(current);
        current = next;
    }
}

int compare_playlists(const void* a, const void* b) {
    Playlist* p1 = *(Playlist**)a;
    Playlist* p2 = *(Playlist**)b;
    if (p1->rating < p2->rating) return 1;
    if (p1->rating > p2->rating) return -1;
    return 0;
}

void load_playlists_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }

    char line[256];
    PlaylistNode* last_node = NULL;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        if (line[0] == '#') {
            char name[100];
            float rating;
            if (sscanf(line, "#%[^,],%f", name, &rating) != 2) continue;

            PlaylistNode* new_node = malloc(sizeof(PlaylistNode));
            strcpy(new_node->playlist.name, name);
            new_node->playlist.rating = rating;
            new_node->playlist.songs = NULL;
            new_node->next = NULL;

            if (playlist_head == NULL) {
                playlist_head = new_node;
            } else {
                last_node->next = new_node;
            }
            last_node = new_node;
        } else if (last_node != NULL) {
            SongNode* new_song = malloc(sizeof(SongNode));
            if (sscanf(line, "%[^,],%[^\n]", new_song->title, new_song->artist) != 2) {
                free(new_song);
                continue;
            }
            new_song->next = last_node->playlist.songs;
            last_node->playlist.songs = new_song;
        }
    }
    fclose(file);
}

void save_playlists_to_file(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error saving to file.txt\n");
        return;
    }

    PlaylistNode* current = playlist_head;
    while (current != NULL) {
        fprintf(file, "#%s,%.2f\n", current->playlist.name, current->playlist.rating);

        SongNode* song = current->playlist.songs;
        while (song != NULL) {
            fprintf(file, "%s,%s\n", song->title, song->artist);
            song = song->next;
        }
        fprintf(file, "\n");
        current = current->next;
    }
    fclose(file);
}

void view_playlists() {
    int count = 0;
    PlaylistNode* current = playlist_head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    if (count == 0) {
        printf(COLOR_YELLOW "\nNo playlists available." COLOR_RESET "\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    Playlist** playlist_array = malloc(count * sizeof(Playlist*));
    current = playlist_head;
    for (int i = 0; i < count; i++) {
        playlist_array[i] = &current->playlist;
        current = current->next;
    }

    qsort(playlist_array, count, sizeof(Playlist*), compare_playlists);

    printf("\n" COLOR_CYAN "=== Playlists sorted by rating ===" COLOR_RESET "\n");
    for (int i = 0; i < count; i++) {
        printf("%d. " COLOR_GREEN "%s" COLOR_RESET " - Rating: " COLOR_YELLOW "%.2f" COLOR_RESET "\n",
              i+1, playlist_array[i]->name, playlist_array[i]->rating);
    }

    free(playlist_array);
    printf("\nPress Enter to continue...");
    getchar();
}

Playlist* select_playlist() {
    int count = 0;
    PlaylistNode* current = playlist_head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    if (count == 0) {
        printf("No playlists available.\n");
        return NULL;
    }
    Playlist** playlist_array = malloc(count * sizeof(Playlist*));
    current = playlist_head;
    for (int i = 0; i < count; i++) {
        playlist_array[i] = &current->playlist;
        current = current->next;
    }
    qsort(playlist_array, count, sizeof(Playlist*), compare_playlists);
    printf("Select a playlist:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s - Rating: %.2f\n", i+1, playlist_array[i]->name, playlist_array[i]->rating);
    }
    int choice;
    printf("Enter the number of the playlist: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > count) {
        printf("Invalid choice.\n");
        free(playlist_array);
        return NULL;
    }
    Playlist* selected = playlist_array[choice-1];
    free(playlist_array);
    return selected;
}

void create_playlist() {
    char name[100];
    char rating_str[10];
    float rating;

    read_input("Enter playlist name (max 99 characters): ", name, 100);

    if (playlist_exists(name)) {
        printf(COLOR_RED "\nError: Playlist '%s' already exists!" COLOR_RESET "\n", name);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    read_input("Enter rating (0.0 to 5.0): ", rating_str, 10);
    if (sscanf(rating_str, "%f", &rating) != 1 || rating < 0 || rating > 5) {
        printf(COLOR_RED "\nInvalid rating! Please enter a number between 0.0 and 5.0." COLOR_RESET "\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    Playlist new_playlist;
    strcpy(new_playlist.name, name);
    new_playlist.rating = rating;
    new_playlist.songs = NULL;

    PlaylistNode* new_node = malloc(sizeof(PlaylistNode));
    new_node->playlist = new_playlist;
    new_node->next = playlist_head;
    playlist_head = new_node;

    printf(COLOR_GREEN "\nPlaylist '%s' created successfully!" COLOR_RESET "\n", name);
    printf("Press Enter to continue...");
    getchar();
}

void delete_playlist() {
    Playlist* selected = select_playlist();
    if (selected == NULL) return;

    printf(COLOR_RED "\nAre you sure you want to delete '%s'? (y/n): " COLOR_RESET, selected->name);
    char confirm = getchar();
    getchar();

    //if (confirm != 'y' || confirm != 'Y') {
    //    printf("Deletion canceled.\n");
    //    printf("Press Enter to continue...");
    //    getchar();
    //    return;
    //}

    PlaylistNode* current = playlist_head;
    PlaylistNode* prev = NULL;
    while (current != NULL) {
        if (&current->playlist == selected) {
            if (prev == NULL) {
                playlist_head = current->next;
            } else {
                prev->next = current->next;
            }
            free_song_list(current->playlist.songs);
            free(current);
            printf(COLOR_GREEN "\nPlaylist '%s' deleted successfully!" COLOR_RESET "\n", selected->name);
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        prev = current;
        current = current->next;
    }
}

void add_song(Playlist* playlist) {
    char title[100];
    char artist[100];

    read_input("\nEnter song title: ", title, 100);
    if (strlen(title) == 0) {
        printf(COLOR_RED "\nError: Song title cannot be empty!" COLOR_RESET "\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    read_input("Enter artist: ", artist, 100);
    if (strlen(artist) == 0) {
        printf(COLOR_RED "\nError: Artist name cannot be empty!" COLOR_RESET "\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    SongNode* new_song = malloc(sizeof(SongNode));
    strcpy(new_song->title, title);
    strcpy(new_song->artist, artist);
    new_song->next = playlist->songs;
    playlist->songs = new_song;

    printf(COLOR_GREEN "\n'%s' by %s added to '%s'!" COLOR_RESET "\n",
          title, artist, playlist->name);
    printf("Press Enter to continue...");
    getchar();
}

void remove_song(Playlist* playlist) {
    if (playlist->songs == NULL) {
        printf("No songs in this playlist.\n");
        return;
    }
    SongNode* current = playlist->songs;
    int index = 1;
    printf("Songs in %s:\n", playlist->name);
    while (current != NULL) {
        printf("%d. %s by %s\n", index, current->title, current->artist);
        current = current->next;
        index++;
    }
    int choice;
    printf("Enter the number of the song to remove: ");
    scanf("%d", &choice);
    current = playlist->songs;
    SongNode* prev = NULL;
    index = 1;
    while (current != NULL && index < choice) {
        prev = current;
        current = current->next;
        index++;
    }
    if (current == NULL || index != choice) {
        printf("Invalid lol.\n");
        return;
    }
    if (prev == NULL) {
        playlist->songs = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
    printf("Song removed.\n");
    printf("Press Enter to continue...");
    getchar();
}

void view_songs(Playlist* playlist) {
    if (playlist->songs == NULL) {
        printf("No songs in this playlist.\n");
        return;
    }
    printf("Songs in %s:\n", playlist->name);
    SongNode* current = playlist->songs;
    int index = 1;
    while (current != NULL) {
        printf("%d. %s by %s\n", index, current->title, current->artist);
        current = current->next;
        index++;
    }
    printf("Press Enter to continue...");
    getchar();
}

void manage_playlist() {
    Playlist* selected = select_playlist();
    if (selected == NULL) {
        return;
    }
    int choice;
    do {
        menu_clear();
        printf("\nManage %s:\n", selected->name);
        printf("1. Add song\n");
        printf("2. Remove song\n");
        printf("3. View songs\n");
        printf("4. Back to main menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                add_song(selected);
                break;
            case 2:
                remove_song(selected);
                break;
            case 3:
                view_songs(selected);
                break;
            case 4:
                menu_clear();
                break;
            default:
                printf("Invalid lol");
                getchar();
        }
    } while (choice != 4);
}
