#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    active_deck = 0;
    auto_sync = false;
    bpm_tolerance = 0;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks...." << std::endl;
    for(int i = 0; i < 2; ++i){
        if(decks[i]){
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;
    PointerWrapper<AudioTrack> cloned_wrapper = track.clone();
    if (!cloned_wrapper) {
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return -1;
    }
    bool is_first_track = (decks[0] == nullptr && decks[1] == nullptr);
    size_t target;
    if (is_first_track) {
        target = 0;
    } else {
        target = 1 - active_deck;
    }
    std::cout << "[Deck Switch] Target deck: " << target << std::endl;
    if (decks[target] != nullptr) {
        delete decks[target];
        decks[target] = nullptr;
    }
    cloned_wrapper->load();
    cloned_wrapper->analyze_beatgrid();
    if (!is_first_track && decks[active_deck] != nullptr && auto_sync) { 
        if (can_mix_tracks(cloned_wrapper) == false) { 
            sync_bpm(cloned_wrapper);
        }
    }
    decks[target] = cloned_wrapper.release();
    std::cout << "[Load Complete] '" << decks[target]->get_title() << "' is now loaded on deck " << target << std::endl;
    if (!is_first_track && decks[active_deck] != nullptr) {
        std::cout << "[Unload] Unloading previous deck " << active_deck 
                  << " (" << decks[active_deck]->get_title() << ")" << std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }
    active_deck = target;
    std::cout << "[Active Deck] Switched to deck " << target << std::endl;
    return target;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] == nullptr){
        return false;
    }
    if(!track){
        return false;
    }
    int current_bpm = decks[active_deck]->get_bpm();
    int new_bpm = track->get_bpm();
    int diff = current_bpm - new_bpm;
    if (diff < 0) {
        diff = -diff;
    }
    return diff <= bpm_tolerance;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if (!decks[active_deck] || !track) return;
    int current_bpm = decks[active_deck]->get_bpm();
    int original_bpm = track->get_bpm();
    int avg_bpm = (current_bpm + original_bpm) / 2;
    std::cout << "[Sync BPM] Syncing BPM from " << original_bpm << " to " << avg_bpm << std::endl;
}
