#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist.get_name()), library() {} //Check later if this was indeed alowed to be altered.
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for (const auto& track_info : library_tracks) {
        AudioTrack* new_track = nullptr;
        if (track_info.type == "MP3") {
            bool has_tags = (track_info.extra_param2 != 0);
            new_track = new MP3Track(track_info.title, track_info.artists, track_info.duration_seconds, track_info.bpm, track_info.extra_param1, has_tags);
        }
        else if (track_info.type == "WAV") {
            new_track = new WAVTrack(track_info.title, track_info.artists, track_info.duration_seconds, track_info.bpm, track_info.extra_param1, track_info.extra_param2);
        }
        if (new_track) {
            library.push_back(new_track);
        }
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist.~Playlist();
    new (&playlist) Playlist(playlist_name);
    int count = 0;
    for (int index : track_indices) {
        if (index < 1 || index > (int)library.size()) {
            std::cout << "[WARNING] Invalid track index: " << index << std::endl;
            continue; 
        }
        AudioTrack* original_track = library[index - 1];
        PointerWrapper<AudioTrack> cloned_wrapper = original_track->clone();
        if (!cloned_wrapper) {
            std::cout << "[ERROR] Failed to clone track at index: " << index << std::endl;
            continue;
        }
        cloned_wrapper->load();
        cloned_wrapper->analyze_beatgrid();
        AudioTrack* raw_track = cloned_wrapper.release();
        playlist.add_track(raw_track);
        count++;
        std::cout << "Added '" << raw_track->get_title() << "' to playlist '" << playlist_name << "'" << std::endl;
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << count << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;
    for (AudioTrack* track : playlist.getTracks()) {
        if (track) {
            titles.push_back(track->get_title());
        }
    }
    return titles;
}
