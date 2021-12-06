# cache_simulator
Cache Simulator for CDA3101

## Usage
You can start the program by running `bin/simulator` from the project root.
### Help
Running `bin/simulator --help` will list the allows option and a quick description of each.
#### Allows options
  - `--help` : produce help message
  - `--trace-file arg` : trace file to run (required)
  - `--cache-type arg` : cache type. Supported: fully associative = 0, direct map = 1, set associative = 2
  - `--rep-policy arg` : replacement policy. Supported: FIFO = 0, LRU = 1
  - `--set-count arg`  : number of lines in the cache
  - `--lines-per-set arg` : number of lines in each set
  - `--line-size arg` : number of bytes per line
  - `--benchmark` : run default benchmark

## Building
### Important commands:
- `make all`: will build all libraries and executables.
- `make simulator`: will build the simulator executable.
- `make tests`: will build the tests executable.

### Dependencies:
#### Google Tests:
  - You can run `./scripts/setup_gtest.sh` to install and setup Google Tests.
  - [How to install Google Tests on Ubuntu](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/). Google Tests libraries should end up at `/usr/local/lib/` to be compatible with the existing makefile.

#### Boost:
  - You can run `sudo apt-get install libboost-all-dev` to install Boost.