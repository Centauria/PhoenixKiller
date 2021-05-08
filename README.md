# PhoenixKiller

Find out hidden mining virus on the computer.

## Installation

```shell
git clone https://github.com/Centauria/PhoenixKiller.git
cd PhoenixKiller
mkdir build && cd build
cmake ..
make
sudo make install
```

There are 2 executives: ```PhoenixKiller``` and ```gpupid```.

When running ```sudo make install```, a systemd service and a timer will be installed on the machine and will start
automatically.

## Usage

### PhoenixKiller

```
Optional arguments:
-h --help        	shows help message and exits
-v --version     	prints version information and exits
--with-md5-check 	Use md5 check when searching.
More accuracy, but slower [default: false]
--cleanup        	Cleanup mining programs automatically. [default: false]
```

### gpupid

Run directly.