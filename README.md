# Macro Tool

Simple Windows macro tool written in C.
Supports mouse automation with clicks, waits, and nested loops.


## Features

- Left click, right click, double click
- Custom wait (milliseconds)
- Loop support
- Nested loops
- Simple '.ini' configuration file


## Usage

1. Create or edit 'macro.ini'
2. Write your macro using the supported commands
3. Run the executable

The program will execute the actions sequentially.

## Syntax

Each line in `macro.ini` represents an action.

### Click
click=X,Y  
Left mouse click at screen coordinates (X, Y)

### Right Click
rclick=X,Y  
Right mouse click at screen coordinates (X, Y)

### Double Click
dbclick=X,Y  
Double left click at screen coordinates (X, Y)

### Wait
wait=TIME  
Pause execution for TIME milliseconds

### Loop Start
loop=N  
Repeat the following block N times

### Loop End
endloop  
Marks the end of a loop block


## Example 1: Simple loop
```ini
click=100,200
wait=500
loop=3
click=200,300
wait=200
endloop
```

## Example 2: Nested loops
```
loop=2
click=100,100
wait=200

loop=3
click=200,200
wait=100
endloop

endloop
```

## Notes

- Coordinates depend on screen resolution and DPI settings
- Recommended display scaling: 100%
- The program does not support games using DirectX input
- Indentation or leading spaces are not allowed in the configuration file


## Error Handling

The program will stop execution if:
- Unknown commands are found
- Coordinates are out of bounds
- Invalid loop values are used
