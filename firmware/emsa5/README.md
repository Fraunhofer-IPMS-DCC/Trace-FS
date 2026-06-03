# Firmware for EMSA5

The demonstrator firmware is written as a bare-metal application in C and consists of a hardware abstraction layer (HAL), a simple static scheduler, and the user code that is executed as periodic tasks by the scheduler. The HAL allows for an easy driver interface to the peripherals used in the demonstrator (LEDs and buttons attached to GPIO pins, timer, interrupt controller, C-Trace unit).

The scheduler is implemented using a timer interrupt. Every tick, the scheduler checks whether there is a task which is ready for being dispatched according to its assigned period. If so, it is added to the dispatch queue. The main scheduler loop executes one task after another in the dispatch queue. Before a task is executed, the scheduler writes an ownership message to the trace stream using the HSI interface, making the schedule visible in the trace.

The user code implemented as periodic tasks is used to showcase some of the features of the various analysis tools, for example WCET analysis and code coverage.

## License

The firmware is licensed under the **[MIT License](https://mit-license.org/)**, except for the files `asw/scenarios_a3/dhrystone.c` and `asw/scenarios_a3/dhrystone.h`, which are believed to be in public domain.

## Contact

Simon Wegener (wegener@absint.com)
