not finished
# Enigma Index of Coincidence (IoC) attack
C++ implementation of ciphertext-only attack (IoC) on Enigma 

The attack is presented in the following paper: [**Ciphertext-only cryptanalysis of Enigma** *by James J. Gillogly*](https://web.archive.org/web/20060720040135/http://members.fortunecity.com/jpeschel/gillog1.htm)

# Installation

Clone repository
```
git clone https://github.com/SamNzo/enigma-IoC-attack.git
```

Init & update enigma submodule
```
cd enigma-IoC-attack/enigma
git submodule init
git submodule update
```

Build executable
```
cd enigma-IoC-attack
chmod +x make.sh
./make.sh
```

The executable is then in the `build` directory

# Usage

The attack has three phases:
- the first one determines the most likely rotor order
- the second determines the most likely ring settings & starting positions
- the last one determines the plugboard connections

## 1. Rotor order
The simplest way to use it is:
```
./ioc-attack --ciphertext <path to ciphertext file>
```

You can choose a custom number of rotors to test between 3 and 8.
```
./ioc-attack --rotors 3 --ciphertext <path to ciphertext file>
```

For more informations: `./ioc -h rotors`

You can also choose a specific reflector
```
./ioc-attack --reflector C --ciphertext <path to ciphertext file>
```

For more informations: `./ioc -h reflector`

The default configuration tests 5 rotors (namely rotors I, II, III, IV and V) and reflector B

### Example
To test 8 rotors with reflector C:

```
./ioc-attack --rotors 8 --reflector C --message <path to ciphertext file>
```

The top 10 rotor configurations are displayed as output

## 2. Ring setting

## 3. Plugboard

https://www.youtube.com/watch?v=V4V2bpZlqx8

https://en.wikipedia.org/wiki/Bombe

# Attack description

**If you are interested in the history of the enigma cryptanalysis, read the following book:**

*Solving the Enigma - History of the Cryptanalytic Bombe by Jennifer Wilcox*

# Credits

The progress bar was made by **https://github.com/gipert/progressbar**
