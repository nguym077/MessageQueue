# MessageQueue

Designed and implemented an “event” monitoring & reporting system.
An event is a random number that is divisible bysome pre-assigned "marker"
value and the random number is generated internally by the software
component. When an event is observed by a component, the phenomenon is
reported to other components.

This project consists of 5 separate components/programs that execute
concurrently on the same machine. Three of the programs are sending
data to 2 receiving programs.
