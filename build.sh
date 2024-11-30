#!/bin/sh
mkdir build && cd build && cmake .. && make -j$(nproc) || cd build && cmake .. && make -j$(nproc)
