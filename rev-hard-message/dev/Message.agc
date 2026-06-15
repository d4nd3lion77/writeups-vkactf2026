        SETLOC  4000
        INHINT
        TCF     START

        RESUME
        NOOP
        NOOP
        NOOP

        RESUME
        NOOP
        NOOP
        NOOP

        DXCH    ARUPT
        EXTEND
        QXCH    QRUPT
        TCF     T3RUPT

        RESUME
        NOOP
        NOOP
        NOOP

        RESUME
        NOOP
        NOOP
        NOOP

        RESUME
        NOOP
        NOOP
        NOOP

        RESUME
        NOOP
        NOOP
        NOOP

        RESUME
        NOOP
        NOOP
        NOOP

        RESUME
        NOOP
        NOOP
        NOOP

        RESUME
        NOOP
        NOOP
        NOOP

DEST1   NOOP

O07     OCT     07
O77     OCT     77
SEED    OCT     27
STEP    OCT     13

START   RELINT
        CA      ZEROES
        TS      IDX
        CA      SEED
        TS      STATE

LOOP    CA      STATE
        AD      STEP
        MASK    O77
        TS      STATE

        CA      IDX
        INDEX   A
        CA      TABP
        AD      STATE
        MASK    O77
        TS      ERRNUM

        CA      IDX
        INDEX   A
        CA      TABN
        AD      STATE
        AD      O07
        MASK    O77
        TS      ERRSUB

        CA      IDX
        AD      ONE
        MASK    O77
        TS      IDX

SNAP    TCF     LOOP

T3RUPT  RESUME

TABP    OCT     66
        OCT     13
        OCT     57
        OCT     54
        OCT     60
        OCT     35
        OCT     67
        OCT     72
        OCT     77
        OCT     33
        OCT     24
        OCT     54
        OCT     46
        OCT     47
        OCT     25
        OCT     25
        OCT     67
        OCT     55
        OCT     41
        OCT     13
        OCT     03
        OCT     40
        OCT     46
        OCT     25
        OCT     50
        OCT     64
        OCT     47
        OCT     40
        OCT     44
        OCT     46
        OCT     70
        OCT     32
        OCT     14
        OCT     16
        OCT     40
        OCT     02
        OCT     64
        OCT     66
        OCT     10
        OCT     52
        OCT     34
        OCT     36
        OCT     60
        OCT     22
        OCT     04
        OCT     06
        OCT     30
        OCT     72
        OCT     54
        OCT     56
        OCT     00
        OCT     42
        OCT     24
        OCT     26
        OCT     50
        OCT     12
        OCT     74
        OCT     76
        OCT     20
        OCT     62
        OCT     44
        OCT     46
        OCT     70
        OCT     32

TABN    OCT     25
        OCT     36
        OCT     04
        OCT     31
        OCT     44
        OCT     06
        OCT     34
        OCT     71
        OCT     06
        OCT     24
        OCT     76
        OCT     55
        OCT     23
        OCT     24
        OCT     21
        OCT     15
        OCT     61
        OCT     14
        OCT     64
        OCT     00
        OCT     23
        OCT     70
        OCT     50
        OCT     02
        OCT     72
        OCT     17
        OCT     52
        OCT     60
        OCT     33
        OCT     65
        OCT     37
        OCT     31
        OCT     43
        OCT     75
        OCT     47
        OCT     41
        OCT     53
        OCT     05
        OCT     57
        OCT     51
        OCT     63
        OCT     15
        OCT     67
        OCT     61
        OCT     73
        OCT     25
        OCT     77
        OCT     71
        OCT     03
        OCT     35
        OCT     07
        OCT     01
        OCT     13
        OCT     45
        OCT     17
        OCT     11
        OCT     23
        OCT     55
        OCT     27
        OCT     21
        OCT     33
        OCT     65
        OCT     37
        OCT     31

$VariablesAndConstants.agc

        SETLOC  200
IDX     ERASE
STATE   ERASE
