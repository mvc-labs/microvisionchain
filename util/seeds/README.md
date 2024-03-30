## Dependencies

Ubuntu:

    sudo apt-get install python3-dnspython


## Modify 'nodes_main.txt' and 'nodes_test.txt'

    123.123.123.123:5555
    111.222.111.222:6666
    ......

## Create chainparamsseeds.h

     python3 generate-seeds.py . > ../../src/chainparamsseeds.h