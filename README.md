# sdl2-pathfinder

A fun little project that demonstrates different path-finding algorithms with pretty visuals, made with SDL2 and C++.

https://github.com/hosua/sdl2-grid/assets/22788738/6a682d61-5d16-410e-a766-e598570c0d8b

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
- [ ] Modular UI Widgets (not sure how many I want to actually do yet)
- [X] Robust scene management (still needs work)
- [X] A* search 
- [ ] Loading/Saving Maps
- [ ] Add compilation instructions/binary for Windows
- [ ] Multithreading support so that input isn't blocked whilst pathfinding
- [ ] World randomizer
