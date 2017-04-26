#!/bin/bash

cd ./kbuild
ninja clean
ninja
cd ..

# Run the profiling test app, gather the stats (which are re-inserted into the documentation)
./scripts/profile.sh

# Building the documentation will generate the html/pdf documentation, which includes the 
# sanity/profiling data from the above tests.
./scripts/build_docs.sh


