#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include "EasyBmp/EasyBMP.h"
#include "Another.cpp"

int main() {
    vector<int> config {49, 7, 4, 9, 3};

    Another_network a_network(config);
    a_network.learn(25000, 0.2);
    a_network.print_network();
    Another_network::print_picture(9);
//    vector<double> result = a_network.evaluate(58);
//    for (double i : result) {
//        cout << i << " ";
//    }
//    cout << endl;
    return 0;
}