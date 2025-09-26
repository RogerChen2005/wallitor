# Wallitor

A dynamic wallpaper software build with tauri.

## Setup

This project has two parts, core and GUI.

### Core

Build solution with *MSVC*, you'll get a dynamic library.

### GUI

GUI is build with *tauri* and *vite+vue*, using *pnpm* as package manager.

1. Install node packages.

   ```bash
   pnpm install 
   ```
2. Build project

   ```bash
   pnpm tauri build --no-bundle
   ```

### Combination

Copy libracy and place in the same directory with executable file.

## Releases

See [Release](https://github.com/RogerChen2005/wallitor/releases).

## Contributions

Welcome Issues and PRs!!!

## Acknowledgement

Wallitor was based on these projects:

* [tauri-apps/tauri](https://github.com/tauri-apps/tauri): Build smaller, faster, and more secure desktop applications with a web frontend.
* [vitejs/vite](https://github.com/vitejs/vite): Next generation frontend tooling. It's fast!
* [vuejs/vue](https://github.com/vuejs/vue): Vue is a **progressive framework** for building user interfaces.
