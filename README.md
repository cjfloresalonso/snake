# snake
The classic game snake in the terminal.

## TODO
- use branches!
- <s>make fullscreen</s>
- deal with screen resizing<s> (possibly pause the game)</s>
- <s>clean up initilisation/end code</s>
- <s>make snake move on it's own</s>
- <s>think about if threading is needed </s>(it's not)
- <s>comment and </s>document code
- write a nice README
- add main loop
    - <s>add food</s>
    - <s>check snake doesn't leave board</s>
    - <s>collision detection</s>
    - <s>make snake grow</s>
    - increase snake's speed
- fix death at the borders (too short)
- <s>score keeping</s>
- nice death message
- nice pause message
- <s>fix collosions of size three</s>
- what if food spawns of snake?


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

