// playlist.h
#ifndef PLAYLIST_H
#define PLAYLIST_H
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_CYAN "\033[1;36m"
#define COLOR_RESET "\033[0m"

typedef struct SongNode
{
    char title[100];
    char artist[100];
    struct SongNode *next;
} SongNode;

typedef struct Playlist
{
    char name[100];
    float rating;
    SongNode* songs;
} Playlist;

typedef struct PlaylistNode
{
    Playlist playlist;
    struct PlaylistNode *next;
} PlaylistNode;

extern PlaylistNode* playlist_head;

void menu_clear(void);
void load_playlists_from_file(const char* filename);
void save_playlists_to_file(const char* filename);

#endif //PLAYLIST_H