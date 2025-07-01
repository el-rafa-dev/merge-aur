<p align="center">
<img src=".github/merge_logo.png" width="30%" height="30%"/>

<h2 align="center">Merge, another <a href="https://aur.archlinux.org/">AUR</a> helper</h2>

<p align="center"> 
Yes, another aur helper made by me but now in C++ (the old one was in shell script)... Make good use of it :)
</p>
</p>


## Note
This is not a package manager... The program only helps you to install a package from the AUR Server

## Dependencies
MakePkg, GIT and Curl must be installed on your Arch Linux (or derivatives)

<!-- ## Installation - Manually
Clone the repo with:
```git clone https://github.com/el-rafa-dev/merge.git ```

Run the command:

 ```cd merge```

and then:

```make```
to compile and generate the executable
 or 
 ```make && make install``` to compile, generate the executable and install on the ```/usr/bin``` directory.

For uninstall, use ```sudo rm -r -f /usr/bin/merge``` -->


## Installation with MakePkg
For installation with MakePkg 
Clone the repo with:
```git clone https://github.com/el-rafa-dev/merge-aur ```

Run on terminal: ```cd merge-aur``` and then ```makepkg -si```

For uninstall, use ```sudo pacman -R merge```

## Libs used in this program
I have use the LibCurl and [nlohmann](https://github.com/nlohmann) [JSON](https://github.com/nlohmann/json)

## How use
```
[Help list]: () for optional and <> for required arguments

-install <package name> : install a package from the AUR by typing the name

-search <package name> (limit of output, e.g.: 1 for 1 output) : search in AUR the 
package with the specified name

-version : display the version info of the merge

-help : display this help list
```

## Licensed under MIT license

```
MIT License

Copyright (c) [2025] [El-Rafa-Dev (Rafael Sousa)]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
