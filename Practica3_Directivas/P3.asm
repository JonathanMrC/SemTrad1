E3  EQU     50
    ORG     $4000
E2  EQU     15
    ADCA    #15
    ADCA    15
    START
    ADCA    150
    ADCA    1500
    ADDD    #10
    ADDD    10
    DC.B
    DC.B    10
    DC.B    10,11
    BSZ     10
    FILL    2,3
    DC.W
    DC.W    1,2
    FCC     /HOLA/
    FCB     10
E1  END
