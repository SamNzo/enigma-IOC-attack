# Enigma Index of Coincidence (IoC) attack
C++ implementation of ciphertext-only attack (IoC) on Enigma 

```
 ______            ______          ________  __    __  ______   ______   __       __   ______  
|      \          /      \        |        \|  \  |  \|      \ /      \ |  \     /  \ /      \ 
 \$$$$$$ ______  |  $$$$$$\       | $$$$$$$$| $$\ | $$ \$$$$$$|  $$$$$$\| $$\   /  $$|  $$$$$$\
  | $$  /      \ | $$   \$$______ | $$__    | $$$\| $$  | $$  | $$ __\$$| $$$\ /  $$$| $$__| $$
  | $$ |  $$$$$$\| $$     |      \| $$  \   | $$$$\ $$  | $$  | $$|    \| $$$$\  $$$$| $$    $$
  | $$ | $$  | $$| $$   __ \$$$$$$| $$$$$   | $$\$$ $$  | $$  | $$ \$$$$| $$\$$ $$ $$| $$$$$$$$
 _| $$_| $$__/ $$| $$__/  \       | $$_____ | $$ \$$$$ _| $$_ | $$__| $$| $$ \$$$| $$| $$  | $$
|   $$ \\$$    $$ \$$    $$       | $$     \| $$  \$$$|   $$ \ \$$    $$| $$  \$ | $$| $$  | $$
 \$$$$$$ \$$$$$$   \$$$$$$         \$$$$$$$$ \$$   \$$ \$$$$$$  \$$$$$$  \$$      \$$ \$$   \$$
                                                                            
```

# Installation

```
git clone https://github.com/SamNzo/enigma-IoC-attack.git
```

```
cd enigma-IoC-attack
chmod +x make.sh
./make.sh
```

```
cd build
./ioc --rotors 5 --plugboard 10 <path to ciphertext file>
```

Paper: **Ciphertext-only cryptanalysis of Enigma** *by James J. Gillogly*

https://www.youtube.com/watch?v=V4V2bpZlqx8

https://en.wikipedia.org/wiki/Bombe

**If you are interested in the history of the enigma cryptanalysis, read the following book:**

*Solving the Enigma - History of the Cryptanalytic Bombe by Jennifer Wilcox*
