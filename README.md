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
`soapy_power -f 80M:110M -n 10 -e 30 -B 30k -k 30 --pow2 -F rtl_power -R | ./Livepow`

### Example loadind the data in the file scan.csv
`./Livepow scan.csv`
