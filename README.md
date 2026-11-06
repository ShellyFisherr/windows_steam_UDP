```
cd ~/orca_project/src/orca_motionTracker

mkdir -p build
cd build

cmake ..
cmake --build . -j$(nproc)
```
