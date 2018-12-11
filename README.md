# snake
The classic game snake in the terminal.

## TODO
- use branches!
- <s>make fullscreen</s>
- deal with screen resizing (possibly pause the game)
- <s>clean up initilisation/end code</s>
- make snake move on it's own
- think about if threading is needed
- <s>comment and </s>document code
- write a nice README
- add main loop
    - add food 
    - check snake doesn't leave board
    - collision detection
    - make snake grow
    - increase snake's speed


## Dependecies
`ncurses` library:
```
sudo apt install ncurses-*
```

## Usage
Build using make:
```
cd snake/
make
```

Run game:
```
./bin/snake
```

Move with
```
 W
ASD
```
Quit with `q`.
