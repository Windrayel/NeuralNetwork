#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include "EasyBmp/EasyBMP.h"

class Network {
    double targetArray[150][3] = {0};

    static const int k = 5; // inside layer quantity
    static const int i = 10; // neuron quantity
    static const int j = 49; // input quantity

    double input[49] = {0};
    double neuron[k][i]; // hidden neurons value
    double output[3] = {0};

    double wInput[i][j]; // input weights
    double wInside[k-1][i][i]; // inside weights
    double wOutput[3][i]; // output weights

    double alpha = 0.2;
    double errorFun = 1;
    int epoch = 0;

    public:
    void init_target() {
        std::ifstream inputFile(R"(C:\Users\kofta\CLionProjects\NeuronNetwork\target.csv)");

        std::string line;
        int lineNumber = 0;
        while (std::getline(inputFile, line)) {
            std::string tempStr;

            std::stringstream inputString(line);
            for (int i1 = 0; i1 < 3; i1++) {
                std::getline(inputString, tempStr, ',');
                targetArray[lineNumber][i1] = std::stod(tempStr);
            }
            lineNumber++;
        }
    }

    void init_weights(){
        for (auto & i1 : wInput) {
            for (double & j1 : i1) {
                if (rand() % 2)
                    j1 = (double) rand() / RAND_MAX;
                else
                    j1 = -(double) rand() / RAND_MAX;
//            wInput[i1][j1] = 0.3;
            }
        }

        for (auto & k1 : wInside) {
            for (auto & i1 : k1) {
                for (double & i2 : i1) {
                    if (rand() % 2)
                        i2 = (double) rand() / RAND_MAX;
                    else
                        i2 = -(double) rand() / RAND_MAX;
//                wInside[k1][i1][i2] = 0.3;
                }
            }
        }

        for (auto & i1 : wOutput) {
            for (double & j1 : i1) {
                if (rand() % 2)
                    j1 = (double) rand() / RAND_MAX;
                else
                    j1 = -(double) rand() / RAND_MAX;
//            wOutput[i1][j1] = 0.3;
            }
        }
    }

    void evaluate(int picture_number) {
        BMP inputImage;
        std::string fileName = R"(C:\Users\kofta\CLionProjects\NeuronNetwork\Images\)" + std::to_string(picture_number) + ".bmp";
        inputImage.ReadFromFile(fileName.c_str());

        for (int i1 = 0; i1 < 49; i1++) {
            int temp = inputImage(i1 % 7, i1 / 7)->Blue;
            temp == 0 ? input[i1] = 1 : input[i1] = 0;
//            std::cout << input[i1] << " ";
//            if (i1 % 7 == 6)
//                std::cout << std::endl;
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
                    sum += neuron[k1 - 1][i2] * wInside[k1 - 1][i1][i2];
                }
                neuron[k1][i1] = 1 / (1 + exp(-sum));
            }
        }

        for (int i1 = 0; i1 < 3; i1++) {
            double sum = 0;
            for (int i2 = 0; i2 < i; i2++) {
                sum += neuron[k - 1][i2] * wOutput[i1][i2];
            }
            output[i1] = 1 / (1 + exp(-sum));
        }
    }

    void learn(int epoch_max) {
        epoch = 0;
        for (epoch = 0; epoch < epoch_max; epoch++) {
//            if (epoch % 10 > 3)
                evaluate(epoch % 150);
//            else
//                continue;

            double difSum = 0;
            for (int i1 = 0; i1 < 3; i1++) {
                difSum += std::abs(targetArray[epoch % 150][i1] - output[i1]);
            }

            errorFun = difSum / 2;

            double deltaOut[3] = {0};

            for (int i1 = 0; i1 < 3; i1++) {
                deltaOut[i1] = output[i1] * (1 - output[i1]) * (targetArray[epoch % 150][i1] - output[i1]);
            }

            double deltaIns[k][i];

            for (int i1 = 0; i1 < i; i1++) {
                double sum = 0;
                for (int j1 = 0; j1 < 3; j1++) {
                    sum += deltaOut[j1] * wOutput[j1][i1];
                }
                deltaIns[k - 1][i1] = neuron[k - 1][i1] * (1 - neuron[k - 1][i1]) * sum;
            }

            for (int k1 = k - 1; k1 > 0; k1--) {
                for (int i1 = 0; i1 < i; i1++) {
                    double sum = 0;
                    for (int j1 = 0; j1 < i; j1++) {
                        sum += deltaIns[k1][j1] * wInside[k1 - 1][j1][i1];
                    }
                    deltaIns[k1 - 1][i1] = neuron[k1-1][i1] * (1 - neuron[k1-1][i1]) * sum;
                }
            }

            for (int i1 = 0; i1 < i; i1++) {
                for (int j1 = 0; j1 < 3; j1++) {
                    wOutput[j1][i1] += alpha * deltaOut[j1] * neuron[k-1][i1];
                }
            }

            for (int k1 = k - 1; k1 > 0; k1--) {
                for (int i1 = 0; i1 < i; i1++) {
                    for (int j1 = 0; j1 < i; j1++) {
                        wInside[k1 - 1][i1][j1] += alpha * deltaIns[k1][i1] * neuron[k1-1][j1];
                    }
                }
            }

            for (int i1 = 0; i1 < i; i1++) {
                for (int j1 = 0; j1 < j; j1++) {
                    wInput[i1][j1] += alpha * deltaIns[0][i1] * input[j1];
                }
            }
//        std::cout << epoch << std::endl;
//        std::cout << output[0] << " " << output[1] << " " << output[2] << " " << std::endl;
//        std::cout << errorFun << std::endl;
        }
    }

    void print_output() {
        for (double i1 : output) {
            std::cout << i1 << " ";
        }
        std::cout << std::endl << errorFun;
    }
};

int main() {
    Network network;
    network.init_target();
    network.init_weights();
    network.learn(25000);
    network.evaluate(140);
    network.print_output();

    return 0;
}