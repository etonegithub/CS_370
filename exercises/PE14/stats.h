#ifndef STATS_H
#define STATS_H

void stats(const std::vector<double>& data, double& min, double& max, double& avg, double& std_dev) {

    // confirm inputs are clean
    min = data[0];
    max = data[0];
    double avg_total = 0;
    double dev_total = 0;
    avg = 0;
    std_dev = 0;

    // min, max, and avg
    for(const double& value : data) {
        if(value < min) {
            min = value;
        }
        if (value > max) {
            max = value;
        }
        avg_total += value;
    }
    avg = avg_total / data.size();

    // standard deviation
    for(const double& value : data) {
        dev_total += pow(value - avg, 2);
    }
    std_dev = sqrt(dev_total / data.size());
}

#endif