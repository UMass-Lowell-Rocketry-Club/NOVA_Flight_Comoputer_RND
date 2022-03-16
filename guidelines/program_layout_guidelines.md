Program Layout Guidelines
Written by Tyler Fanuele
03/04/2022

This fill will outline the acceptable programming style for code written for
the UMass Lowell Rocketry club. As strict as some of these rules may be they
are important for keeping our code as streamline and readable as possible.
There is a good chance that our code will be in this club in some way or 
form down the line and it is important that our successors can read and
understand our code. These guidelines are subject to change.

Programming Guidelines For C++:
1.  Your program should pass all cpplint standards but only need to pass these
    tests:
    -   each line should not exceed 80 characters in length
    -   every comparison or loop operator (if, for, while, switch...) should 
        have a space after the operator and before the parenthesis.\
        Ex: if ( x == 2 )
    -   Each comparison or loop operator parenthesis should be spaced in a 
        readable manner.\
        Ex: if ( x <= 2 )
    -   Each comparison or loop operator block's first curly brace should be
        both on the same line and have a space before it.\
        Ex: if ( x >= y ) {}
    -   Common variable names should be in snake code format. Underscore to 
        separate words and all lowercase characters.\
        Ex: int example_var;
    -   Member variables should be the same snake format as common variables
        but with a trailing underscore.\
        Ex: int example_var_;
    -   Class names should be written all in one word and capitals should be 
        used at the start of the variable name as well as to separate words.\
        Ex: class ExampleClass {};
    -   Function names should be mixed case and mutators should be written like
        variables.\
        Ex: SampleFunction();\
        Ex: mutator_function();\
    -   Avoid excessive amounts of whitespaces and blank lines. 
    -   All names should clearly explain what the thing does.
2.  Each file should have a comment block at the top in the following 
    format:\
    /*\
        [File Name]\
        [Author, First Last]\
        [Date, mm/dd/yyyy]\
        [Program description]\
    */
3.  Each function should have a comment block above it in the following
    format:\
    /*\
        [Program description]\
        [Argument descriptions, if applicable\
        [Return value description, if applicable]\
    */
4.  Properly indent your code lines.

