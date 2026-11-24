# Windows Steam UDP bridge

This is a short program the read the trackerdata using steamVR and the OpenVR SDK.

The file must be dropped alongside the other files in the OpenVR SDK, so you get:
```
- bin
- docs
- headers
- samples
- src
- tracker_reader
- ...
```
from there, in a terminal, go to the corresponding `tracker_reader` file and run:
```
mkdir -p build
cd build

cmake ..
cmake --build . --config Release
```
This will create a new `build` folder and move to it. From there it will build the MakeFile parameters and create a executable and puts it in a `Release` folder.

From there the program can be run by either opening the `trackerdata.exe`. Or it can be run from the command line by going to the folder it is located in and using
```
./trackerdata.exe
```
Make sure that steamVR is running and the tracker is turned on, otherwise the program will close itself again since no tracker was found.


# Config
If you want to change the address the data is send to, change the IP address in trackerdata.cpp on line 111.
The corresponding host address for the Linux side can be found by running the following command in Linux:
```
hostname -I
```
The correct IP-address is the long (usually first) sequence.
For example:
```
123.456.789.00
```
