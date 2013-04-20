#!/bin/bash
set -e

#debuild -us -uc
#sudo debuild -i -us -uc -rsudo
#sudo debuild -I -us -uc -rsudo
debuild -i -us -uc -rfakeroot
#su -c pbuilder build ../qmentat_0.8.dsc

