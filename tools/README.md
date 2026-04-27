# Tools

This folder contains additional utilities for the macro tool.


# Coordinate Picker

A simple command-line tool to capture mouse coordinates and generate ready-to-use commands for **macro.ini**.

## Features

- Capture mouse position with hotkeys
- Outputs commands directly compatible with the macro tool
- Supports:
  - __Left click__
  - __Right click__
  - __Double click__


## Build

Compile with:

```bash
gcc coordinate_picker.c -o picker.exe -luser32
```


## Usage

Run the program:

```bash
picker.exe
```


## Controls

- F8  → left click (`click=x,y`)
- F9  → right click (`rclick=x,y`)
- F10 → double click (`dbclick=x,y`)
- ESC → exit


## Example Output

```ini
click=100,200
rclick=300,400
dbclick=500,600
```

You can copy these lines directly into your **macro.ini**.


## Notes

- Coordinates depend on your screen resolution
- For best results, use consistent DPI scaling (e.g. 100%)
