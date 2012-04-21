mpstat -P ALL | grep 'all'  | ./column 11 | awk '{dif = 100 - $1; printf("utilization: %f\n", dif);}'

