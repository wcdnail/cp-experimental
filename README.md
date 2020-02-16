# CP experimental
## Windows DevEnv setup (VCPKG)
```
git clone https://github.com/Microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
```
## Windows DevEnv setup (GTK & QT:VCPKG)
```
vcpkg install ^
gtk:x64-windows ^
qt5:x64-windows ^
qscintilla:x64-windows ^
cgal:x64-windows ^
mpfr:x64-windows ^
boost:x64-windows ^
opencsg:x64-windows ^
glew:x64-windows ^
fontconfig:x64-windows ^
harfbuzz:x64-windows ^
libzip:x64-windows ^
double-conversion:x64-windows ^
eigen3:x64-windows ^
libxml2:x64-windows
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
