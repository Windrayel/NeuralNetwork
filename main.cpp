#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

int main() {
    double targetArray[150][3] = {0};

    std::ifstream inputFile;
    inputFile.open(R"(C:\Users\kofta\CLionProjects\NeuronNetwork\target.csv)");

    std::string line;
    int lineNumber = 0;
    while (std::getline(inputFile, line)) {
        std::string tempStr;

        std::stringstream inputString(line);
        for (int i = 0; i < 3; i++) {
            std::getline(inputString, tempStr, ',');
            targetArray[lineNumber][i] = std::stoi(tempStr);
        }
        lineNumber++;
    }
    int k = 5; // inside layer quantity
    int i = 10; // neuron quantity
    int j = 49; // input quantity

    double input[49] = {0};
    double neuron[k][i]; // hidden neurons value
    double output[3] = {0};

    double wInput[i][j]; // input weights
    double wInside[k-1][i][i]; // inside weights
    double wOutput[3][i]; // output weights

    // assign start values of weights
    for (int i1 = 0; i1 < i; i1++) {
        for (int j1 = 0; j1 < j; j1++) {
            wInput[i1][j1] = (double) rand() / RAND_MAX;
        }
    }

    for (int k1 = 0; k1 < k; k1++) {
        for (int i1 = 0; i1 < i; i1++) {
            for (int i2 = 0; i2 < i; i2++) {
                wInside[k1][i1][i2] = (double) rand() / RAND_MAX;
            }
        }
    }

    for (int i1 = 0; i1 < 3; i1++) {
        for (int j1 = 0; j1 < i; j1++) {
            wOutput[i1][j1] = (double) rand() / RAND_MAX;
        }
    }

    for (int i1 = 0; i1 < i; i1++) {
        double sum = 0;
        for (int j1 = 0; j1 < j; j1++) {
            sum += input[j1] * wInput[i1][j1];
        }
        neuron[0][i1] = 1 / (1 + exp(-sum));
    }

    for (int k1 = 1; k1 < k; k1++) {
        for (int i1 = 0; i1 < i; i1++) {
            double sum = 0;
            for (int i2 = 0; i2 < i; i2++) {
                sum += neuron[k1-1][i2] * wInside[k1-1][i1][i2];
            }
            neuron[k1][i1] = 1 / (1 + exp(-sum));
        }
    }

    for (int i1 = 0; i1 < 3; i1++) {
        double sum = 0;
        for (int i2 = 0; i2 < i; i2++) {
            sum += neuron[k-1][i2] * wOutput[i1][i2];
        }
        output[i1] = 1 / (1 + exp(-sum));
    }

    double difSum = 0;
    for (int i1 = 0; i1 < 3; i1++) {
        difSum += targetArray[0][i1] - output[i1];
    }
    double errorFun = difSum / 2;

    double deltaOut[3] = {0};
    double alpha = 0.1;

    for (int i1 = 0; i1 < 3; i1++) {
        deltaOut[i1] = output[i1] * (1 - output[i1]) * (targetArray[0][i1]);
    }

    double deltaIns[k][i];

    for (int i1 = 0; i1 < i; i1++) {
        double sum = 0;
        for (int j1 = 0; j1 < 3; j1++) {
            sum += deltaOut[j1] * wOutput[j1][i1];
        }
        deltaIns[k-1][i1] = output[i1] * (1 - output[i1]) * sum;
    }

    for (int k1 = k-1; k1 > 0; k1--) {
        for (int i1 = 0; i1 < i; i1++) {
            double sum = 0;
            for (int j1 = 0; j1 < i; j1++) {
                sum += deltaIns[k1][j1] * wInside[k1-1][j1][i1];
            }
            deltaIns[k1-1][i1] = neuron[k1][i1] * (1 - neuron[k1][i1]) * sum;
        }
    }

    for (int i1 = 0; i1 < i; i1++) {
        for (int j1 = 0; j1 < 3; j1++) {
            wOutput[j1][i1] += alpha * deltaOut[j1] * neuron[k-1][i1];
        }
    }

    for (int k1 = k-1; k1 > 0; k1--) {
        for (int i1 = 0; i1 < i; i1++) {
            for (int j1 = 0; j1 < i; j1++) {
                wInside[k1-1][i1][j1] += alpha * deltaIns[k1][i1] * neuron[k1-1][j1];
            }
        }
    }

    for (int i1 = 0; i1 < i; i1++) {
        for (int j1 = 0; j1 < j; j1++) {
            wInput[i1][j1] += alpha * deltaIns[0][i1] * input[j1];
        }
    }


//    input[0] = 1;
    std::cout << targetArray[0][0] << std::endl;
    return 0;
}