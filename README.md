# sdl2-pathfinder

A fun little project that demonstrates different path-finding algorithms with pretty visuals, made with SDL2 and C++.

https://github.com/hosua/sdl2-pathfinder/assets/22788738/1f211b33-0196-4d86-b14e-c87fdaa18a4f

# Linux Compilation Instructions
```
mkdir build
cd build
cmake ..
make
```
then run with `./grid-game`.

If you want, you can also just run the `./run.sh` script to compile and run everything.


# Features to be Implemented
- [ ] Modular UI Widgets (not sure how many I want to do yet)
- [X] Robust scene management (still needs work)
- [X] A* search 
- [ ] Loading/Saving Maps
- [ ] Add compilation instructions/binary for Windows
- [ ] Multithreading support so that input isn't blocked whilst pathfinding
- [X] World randomizer (need to add more/better randomization techniques)
