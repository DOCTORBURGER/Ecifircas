This is a chess engine that I'm currently developing. Once complete, it will be a UCI protocol based engine. 

Here is a list of some sources I have taken inspiration from:
- [Stockfish (repository and documentation)](https://github.com/official-stockfish/Stockfish)
- [Sebastian Lague - Coding Adventure: Chess (video)](https://youtu.be/U4ogK0MIzqk?si=B_AzoIEiNSfqXBnx)
- [Sebastian Lague - Coding Adventure: Making a Better Chess Bot (video)](https://youtu.be/_vqlIPDR2TU?si=7MfmTSUdMzOQnqTo)
- [Chess Programming Youtube Channel (CodeMonkeyKing)](https://www.youtube.com/@chessprogramming591)
- [Chess Programming Wiki](https://www.chessprogramming.org/Main_Page)

Helpful tools:
- [Bitboard Viewer](https://tearth.dev/bitboard-viewer/)
- [Arena Chess GUI (best free GUI I found for debugging)](http://www.playwitharena.de/)

**HOW TO COMPILE:**

Create a build folder, `cmake ..` and then run `make`.

**HOW TO USE:**

This project follows uci protocol, so to use it you just need to point a chess UI that expects an engine to the .exe file. For testing this project I used Arena chess UI.
