const PORTSLIMIT  = 50;

struct inputstruct {
        char host1[40];
        int start;
        int end;
};


struct outputstruct {

        int tot;
        int port_no[PORTSLIMIT];
};


typedef struct inputstruct in;

typedef struct outputstruct out;

program PORTPROG {
    version PORTS {
        out getopenports(in) = 1;
    } = 1;
} = 22855;

