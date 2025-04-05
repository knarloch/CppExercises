# CppExercises

## Jumping Stones

A path consists of a sequence of stones, each marked with non-negative integer.
You start at the beginning and always progress towards the end. When you step or jump onto a stone, you earn points
equal to the value on that stone times the length of the step or jump.

Length is an integer calculated as one plus the number of stones skipped.

Given that you can jump as far as you want, what's the maximum score you can achieve for a given path?

Example:

Given sequence:
3 7 2 10 5 12 8 10 1
The score would be 81 for the following sequence:
3 -> (5 stones) -> 12 -> (2 stones) -> 10 -> (1 stone) -> 1

Result will fit into 64 bit integer.