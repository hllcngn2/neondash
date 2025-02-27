# neondash
ASCII vignette editor for the terminal\
CURRENTLY FUNCTIONAL IN BLACK & WHITE

### description
This program is designed to enable crafting of small, character-perfect, coloured pieces of ASCII art.

### usage
```
make
./neondash [h w]
```
where optionally 1 <= h <= 10 and 1 <= w <= 20\
if h and w are omitted, the default values 5 and 10 will be used.\
if h and w don't fall into the ranges 1-10 and 1-20, they will be set respectively to 10 and 20, which is the max size of the canvas.\
Ideally you'd want to use low values unless you're prepared to work on a masterpiece.

### controls
q - quit\
s - save\
\
i - canvas focus\
space - modify a canvas cell\
type your ASCII character after pressing space\
\
(not currently fully functional)\
(f - fg palette focus)\
(b - bg palette focus)\
(a - add then validate a new color)\
(s - discard new color and try another one)\
NB: the ASCII char palette is not interactive and for indicative purposes only\
\
hjkl - move cursor\
esc - exit focus\
\
NB: These controls can be modified in controls.h with a recompile

### author
by d0pe
