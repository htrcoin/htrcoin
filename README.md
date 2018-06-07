HighTemperature Release Tree

HighTemperature is a POW/PoS/MN based cryptocurrency.

HighTemperature is a PoW (Scrypt) / PoS / Master Node hybrid cryptocurrency, with a 80 million coin maximum supply.

Block Spacing: 60 Seconds
Diff Retarget: every 10 min.
Maturity: 100 Blocks
Stake Minimum Age: 1 Hours

Default Network Ports:
Port: 11368
RPC Port: 11369



For compiling on different architectures, see the docs/build-*os* documents. Otherwise, view releases page for windows qt-wallets.

****
HTRC includes an Address Index feature, based on the address index API (searchrawtransactions RPC command) implemented in Bitcoin Core but modified the implementation to work with the HTRC codebase (PoS coins maintain a txindex by default for instance).

Initialize the Address Index By Running with -reindexaddr Command Line Argument.  It may take 10-15 minutes to build the initial index.

HTRC is dependent upon libsecp256k1 by sipa, the sources for which can be found here:
https://github.com/bitcoin/secp256k1
****
