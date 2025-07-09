# strl-lang

Basic programming language implemented in C.

## Description

I created strl-lang to learn more about programming language creation and design. I tried to look at the least amount of resources possible, since this was strictly made for learning purposes. In the future, I might research how _good_ languages are actually made to improve upon this. For now, this will do.

## Getting Started

### Dependencies

* No external libraries used, just need Make for simple compilation

### Installing

* Clone the repo
* In root, you can compile by running `make all`
* Pre-made scripts can be found in the scripts directory

### Executing program

Simply execute your strl-lang files like so:
```
strllang <strl file> [--verbose]
```
The verbose option (actually, any third arg value will trigger the verbose flag) will print out some info on traversing the script text, as well as print out the final Abstract Syntax Tree.
