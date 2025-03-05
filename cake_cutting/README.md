# CppExercises

## Cake cutting

On a rectangle table there are pieces of cake. Cut the table alongside (give Y coordinate) so that the amount of cake
in both parts of the table is the same.

Input: `[Cake(x=1.0, y=0.0, side=2.0), Cake(x=4.0, y=1.0, side=2.0)]`  
Output: 1.5

```
0   1   2   3   4   5   6
0 +---+---+---+---+---+---+---+
|   | X | X |   |   |   |   |
1 +---+---+---+---+---+---+---+
|   | X | X |   | X | X |   |
2 +---+---+---+---+---+---+---+
|   |   |   |   | X | X |   |
3 +---+---+---+---+---+---+---+
```