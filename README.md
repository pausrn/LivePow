# LivePow
A reader for making heatmap from soapy_power output

## Building
```
mkdir build
qmake ..
make -j4
```

## Usage
### Example scanning the FM radio band
`soapy_power -f 80M:110M -n 10 -e 30 -B 30k -k 30 --pow2 -F rtl_power -R | ./LivePow`

### Example loading the data in the file scan.csv
`./LivePow scan.csv`

### Example reading from scan.csv using the palette from palette.png while skipping the first 22 lines
`./LivePow -p palette.png -s 22 scan.csv`

### Palette
Palette are simple images files, pixels are read from left to right and top to bottom and mapped from the lower value to the higher value  
The default palette is grayscale  
Example palette :  
![Example palette](palette.png)

### Example output : FM band scan
![Example output](out_fm_band.png)
