#! /bin/bash

find /usr/bin/FBookshelf -exec rm -rf {} \;
find /usr/share/FBookshelf -exec rm -rf {} \;
find /usr/share/pixmaps/FBookshelf* -exec rm -rf {} \;
find /usr/lib/zlibrary -exec rm -rf {} \;
find /usr/share/zlibrary -exec rm -rf {} \;

