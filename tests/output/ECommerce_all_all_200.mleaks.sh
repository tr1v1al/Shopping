#!/bin/bash
/usr/bin//valgrind --leak-check=full ./dist/Debug/GNU-Linux/shopping2 < ./tests/validation/ECommerce_all_all_200.keyboard 1> /dev/null 2>> ./tests/output/ECommerce_all_all_200.mleaks.full
