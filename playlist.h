#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>

const std::string COLOR_RED = "\033[1;31m";
const std::string COLOR_GREEN = "\033[1;32m";
const std::string COLOR_YELLOW = "\033[1;33m";
const std::string COLOR_CYAN = "\033[1;36m";
const std::string COLOR_RESET = "\033[0m";

class Song {
public:
    std::string title;
    std::string artist;

    Song(const std::string& t, const std::string& a) : title(t), artist(a) {}
};

class Playlist {
public:
    std::string name;
    float rating;
    std::vector<Song> songs;

    Playlist(const std::string& n, float r) : name(n), rating(r) {}

    void add_song();
    void remove_song();
    void view_songs() const;
};

class PlaylistManager {
private:
    std::vector<Playlist> playlists;

    bool playlist_exists(const std::string& name) const;
    Playlist* select_playlist();

public:
    void load_from_file(const std::string& filename);
    void save_to_file(const std::string& filename);
    void create_playlist();
    void view_playlists() const;
    void delete_playlist();
    void manage_playlist();
};

void menu_clear();

#endif // PLAYLIST_H