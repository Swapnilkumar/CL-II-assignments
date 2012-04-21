/*
 * The average procedure receives an array of real
 * numbers and returns the average of their
 * values. This program handles a maximum of
 * 200 numbers.
 */
const MAXAVGSIZE  = 200;

struct input_data {
  int input_data<200>;
};

typedef struct input_data input_data;

program AVERAGEPROG {
    version AVERAGEVERS {
        int AVERAGE(input_data) = 1;
    } = 1;
} = 22855;

