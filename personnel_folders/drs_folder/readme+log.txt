Folder for use by Devon to store various code without committing to src

3/5/2022 Notes:
Wrote out NOVA 0920B_ro code. RO stands for re-organized. 
1. Added Vector3D
2. Added a cpp file to indicate the possibility of stablization, added a cpp file to indicate general math functions
3. Reorganized loop() to run via a while loop.
4. Moved all macros and constants/other global variables to macros.h
5. Removed flstate system as well as the some code which set LED's during the launch sequence. 
6. Known Errors: There are a ton of errors on my end, all of them, with the exception of one seem to be the result of not having
the libraries working properly in visual studio. it seems that the string library seems to be causing some issues. 