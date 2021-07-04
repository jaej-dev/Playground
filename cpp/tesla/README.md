# The tesla project
The tesla project is a skeleton of software composed of event-driven architecture's FSM, logger, and diagnosis interfaces.
This software is made to be simple as an example of manual and autopilot driving of Tesla vehicles.


# Philosophy
- Separatation of concern such as FSMs, states, components and behaviors
- Scalable and flexible developments
- Reduce complexit
- Increase the productivity


# Build procedure
**1. Requirements**
- cmake 3.x or later
- python 3.x or later
- gcc 4.2.1 or later

**2. Coding rules**
- Misra c++ 2008+
- Google c++ style guide
- 80 character column limit
- 2 spaces for tab character or do not use tab character

**3. Clone project**

    $ git clone --recursive git@address
    $ cd tesla
    $ mkdir build
    $ cd build

**4. Build**

    $ cmake ..
    $ make

**5. Store relevant build output files to `tesla/stage/bin`**

    $ make install

