# C++ DJ Session Engine

A command-line DJ session simulator built in C++11, focused on resource ownership, polymorphism, and cache management.

The system models a music library, playlists, a controller-side LRU cache, and a two-deck mixer. It simulates track loading and transitions using track metadata; it does not decode or play audio.

## Features

- Polymorphic MP3 and WAV track models with format-specific metadata and quality scoring.
- Copy and move semantics for dynamically managed track data.
- A custom move-only pointer wrapper with ownership transfer, reset, and release operations.
- Playlist management with track insertion and removal.
- An LRU cache for controller-side track storage.
- A two-deck mixing simulation with BPM compatibility checks and optional synchronization.
- Configuration-driven libraries, playlists, and session settings.
- Interactive playlist selection and automatic processing of all configured playlists.

## Design

The application separates library management, caching, mixing, and session orchestration.

| Component | Responsibility |
| --- | --- |
| `AudioTrack`, `MP3Track`, `WAVTrack` | Track metadata, polymorphic behavior, and cloning |
| `PointerWrapper` | Exclusive ownership and move-based resource transfer |
| `Playlist` | Track collections and playlist operations |
| `LRUCache`, `CacheSlot` | Cached tracks and least-recently-used replacement |
| `DJLibraryService` | Music library construction and track lookup |
| `DJControllerService` | Track loading and controller cache access |
| `MixingEngineService` | Deck management and simulated BPM synchronization |
| `DJSession` | Session orchestration and playlist processing |
| `SessionFileParser` | Configuration parsing |

Tracks are cloned when independent ownership is needed across components. The custom pointer wrapper supports transferring ownership without copying the underlying object.

## Requirements

- A C++11-compatible compiler, such as GCC
- GNU Make
- Linux or WSL
- Optional: Valgrind for memory diagnostics

On Ubuntu or Debian:

```bash
sudo apt-get update
sudo apt-get install build-essential
```

For memory diagnostics:

```bash
sudo apt-get install valgrind
```

## Build

```bash
git clone https://github.com/alonmoneli/cpp-dj-session-engine.git
cd cpp-dj-session-engine
make
```

The executable is created at `bin/dj_manager`.

To rebuild with the project's debug configuration:

```bash
make clean
make debug
```

## Run

Run commands from the repository root so the application can locate `bin/dj_config.txt`.

### Process all playlists

```bash
./bin/dj_manager -I -A
```

Loads the configured library and processes all playlists automatically, printing track, cache, deck, and session information.

### Select playlists interactively

```bash
./bin/dj_manager -I
```

### Run the built-in demonstrations

```bash
./bin/dj_manager
```

Exercises playlist operations, copy/move behavior, pointer ownership, and polymorphism.

These demonstrations print diagnostic output; they are not a comprehensive assertion-based test suite.

## Configuration

The session reads `bin/dj_config.txt`.

The configuration defines:

- Library tracks and their metadata.
- Playlists and their track references.
- Controller cache capacity.
- BPM tolerance.
- Whether automatic BPM synchronization is enabled.

Edit the supplied configuration to experiment with different playlists and cache settings.

## Memory Diagnostics

Build with debug information, then inspect a complete session with Valgrind:

```bash
make clean
make debug
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/dj_manager -I -A
```

To inspect the built-in demonstrations separately:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/dj_manager
```

Memory-diagnostic results depend on the execution path and input. A successful build or completed session alone does not establish memory safety.

## Repository Layout

- `src/` — C++ implementations and command-line entry point.
- `include/` — Class declarations and pointer-wrapper implementation.
- `bin/dj_config.txt` — Default session configuration.
- `input_2/` — Additional configuration and sample output.
- `Makefile` — Build and diagnostic targets.

## Scope

This project focuses on C++ object lifetimes, ownership, and application structure through a DJ simulation.

MP3/WAV loading, beat analysis, and mixing are simulated using metadata. Real audio playback, decoding, and digital signal processing are outside its current scope.
