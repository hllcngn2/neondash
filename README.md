# neondash
ASCII vignette editor for the terminal

### usage
```
make
./neondash [h w]
```
if h and w are omitted, the values 5 and 10 will be used.\
if h and w don't fall into the ranges 1-10 and 1-20, they will be set respectively to 10 and 20, which is the max size of the canvas. Ideally you'd want to use low values unless you're prepared to work on a masterpiece.

### controls
q - quit\
s - save\
\
i - canvas focus\
' ' - modify a canvas cell\
\
f - select fg palette\
b - select bg palette\
a - add and validate color (fg or bg depending on focus)\
s - discard and try another color\
\
hjkl - move cursor\
esc - esc\
\
NB: These controls can be modified in controls.h with a recompile
