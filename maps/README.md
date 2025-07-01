# Map Files

This directory contains text-based map files for the Space Station game.

## Map File Format

Each map file is a simple text file where:
- `#` represents walls
- `.` represents empty space
- `P` represents the player starting position
- `F` represents the finish/goal
- `A`, `B`, `C`, `D` represent different types of atmosphere tanks
- Any other character is treated as empty space

## File Structure

- `easy.txt` - Easy level (10x10)
- `medium.txt` - Medium level (10x20) 
- `hard.txt` - Hard level (20x20)

## Creating Custom Maps

1. Create a new `.txt` file in this directory
2. Use the symbols above to design your level
3. Make sure all lines have the same length
4. The map must contain exactly one `P` (player start) and one `F` (finish)
5. The game will automatically load the appropriate file based on difficulty level

## Example Map

```
##########
#P......F#
####.#.##
#...#A..#
#.#.#.##
#.#.#...#
#.#.####
#.#.....#
#...#.##
##########
```

## Notes

- The game will fall back to hardcoded maps if a file is missing or invalid
- Map files are copied to the build directory automatically during compilation
- Make sure to rebuild the project after adding new map files 