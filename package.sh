#!/bin/bash
set -e

#debuild -us -uc
sudo debuild -i -us -uc -rsudo
#debuild -S -us -uc
#su -c pbuilder build ../qmentat_0.8.dsc

