# GTK+ 3 experimental
## Windows DevEnv setup (GTK:VCPKG)
```
git clone https://github.com/Microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
vcpkg install gtk:x64-windows
```
## Windows DevEnv setup (QT:VCPKG)
```
vcpkg install qt5:x64-windows
vcpkg install qscintilla:x64-windows
vcpkg install cgal:x64-windows
vcpkg install mpfr:x64-windows
vcpkg install boost:x64-windows
vcpkg install opencsg:x64-windows
vcpkg install glew:x64-windows
vcpkg install fontconfig:x64-windows
vcpkg install harfbuzz:x64-windows
vcpkg install libzip:x64-windows
vcpkg install bison:x64-windows
vcpkg install flex:x64-windows
vcpkg install pkg-config:x64-windows
vcpkg install double-conversion:x64-windows
vcpkg install eigen3:x64-windows
vcpkg install libxml2:x64-windows
```
## Windows build (MSYS)
 - git cmake <a target="_blank" href="http://repo.msys2.org/distrib/">msys</a>
 - required packages
```
pacman -S mingw-w64-x86_64-gcc \
mingw-w64-x86_64-gdb \
mingw-w64-x86_64-make \
mingw-w64-x86_64-ninja \
mingw-w64-x86_64-pkg-config \
mingw-w64-x86_64-glib2 \
mingw-w64-x86_64-gtk3 \
mingw-w64-x86_64-glade

```
 - optional packages
``` 
pacman -S mingw-w64-x86_64-crt-git \
mingw-w64-x86_64-headers-git \
mingw-w64-x86_64-python3-gobject \
mingw-w64-x86_64-python-pyopengl \
mingw-w64-x86_64-python-numpy \
mingw-w64-x86_64-vala

```
## Special thanks
 - Icons - <a target="_blank" href="https://icons8.com/icons/set/3d-select">3D Object icon</a> icon by <a target="_blank" href="https://icons8.com">Icons8</a>
