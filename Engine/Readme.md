# Quick Start
## 1. Install SFML Dependencies:
    - Windows: Nothing to do.
    - Fedora :
        ```sudo dnf5 install libXcursor-devel libX11-devel.x86_64 libxcb-devel.x86_64 xcb-util-devel.x86_64 libXrandr-devel.x86_64 xcb-util-image-devel.x86_64 libgudev-devel.x86_64 glm-devel.noarch mesa-libGL-devel mesa-libEGL-devel freetype-devel.x86_64 libjpeg-turbo-devel.x86_64 openal-soft-devel.x86_64 libogg-devel.x86_64 libvorbis-devel.x86_64 flac-devel.x86_64 systemd-devel```
    - Ubuntu :
        ```sudo apt update
           sudo apt install libxrandr-dev libxcursor-dev libudev-dev libfreetype-dev libopenal-dev libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev```
## 2. Build server & client
    - ```mkdir build
         cd build
         cmake .. -DCMAKE_BUILD_TYPE=[Release|Debug] -DMGE_BUILD_TYPE=[Server|Client]
         cmake --build . --config=[Release|Debug]```