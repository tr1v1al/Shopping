#!/bin/bash
/usr/bin//valgrind --leak-check=full ./dist/Debug/GNU-Linux/shopping2 < ./tests/validation/ECommerce49.keyboard 1> /dev/null 2>> ./tests/output/ECommerce49.mleaks.full
