# Snake game in C

## Requirements

- Linux
- gcc
- ncurses

## Running

Compile and run:
```sh
cd src
gcc *.c -lncurses -o main.out
./main.out
```

## Playing

For simplicity I used regular ASCII characters:

```
_: Empty cell
+: Snake head
=: Snake body
@: Apple, eat it to make your snake bigger
```

Controls:

```
[UP, RIGHT, DOWN, LEFT]: move
q: quit
```
