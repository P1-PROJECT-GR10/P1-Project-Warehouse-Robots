# P1-Project-Warehouse-Robots

![BUILD](https://github.com/P1-PROJECT-GR10/P1-Project-Warehouse-Robots/actions/workflows/cmake-unit-tests.yml/badge.svg)

![BUILD](https://github.com/P1-PROJECT-GR10/P1-Project-Warehouse-Robots/actions/workflows/simulations.yml/badge.svg)

## Overview
This project is a C program, developed as part of cs-25-sw-1-p1-10's semester project at the Department of Computer Science at AAU. <br />

This project simulates movement of a singular robot in a dynamically generated warehouse environment, given a pathfinding algorithm. <br />
Both A-star and a simple greedy step pathfinding algorithm, is implemented for simulation comparisons. <br />
Generated shelves are populated with items from ```items.txt``` in consistent order from left to right, top to bottom, excess shelves being empty. <br />
The robot uses a picking list, to simulate picking order(s), and ends the run when reaching closest drop-off zone.

The simulation outputs algorithm runtime and amount of steps moved.

## Usage
Seeded pseudo-random number generation (PRNG) is used to generate reproducible scenarios for simulation comparison. <br />
To generate randomized scenarios, use ```time(NULL)``` as seed.

## Credits
This project is developed by members of cs-25-sw-1-p1-10 at Department of Computer Science. <br />
During the development, the group was supported by supervisor Fengchun Zhang, and research assistant Ivan Bonev Bonev.

For unit testing, [mtest](https://github.com/MortenSchou/mtest) is used, a lightweight C unit testing framework, developed by [Morten Schou](https://github.com/MortenSchou). <br />

## License
MIT © 2025 cs-25-sw-1-p1-10