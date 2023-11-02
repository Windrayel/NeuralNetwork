#include <vector>
#include <string>
#include <fstream>
#include <sstream>
//#include <gdiplus.h>
#include "EasyBmp/EasyBMP.h"

using namespace std;

class Another_network
{
    vector<vector<double>> neuron;			//neuron values, layer 0 - input, layer last - output
    vector<vector<vector<double>>> weights;
public:
    explicit Another_network(vector<int> layers) {
        neuron.resize(layers.size());
        weights.resize(layers.size() - 1);

        for (size_t k = 0; k < layers.size(); k++) {
            neuron[k].resize(layers[k]);
        }

        for (size_t k = 0; k < layers.size() - 1; k++) {
            weights[k].resize(layers[k]);
            for (size_t i = 0; i < layers[k]; i++) {
                weights[k][i].resize(layers[k+1]);
            }
        }

        for (size_t k = 0; k < layers.size() - 1; k++) {
            for (size_t i = 0; i < layers[k]; i++) {
                for (size_t j = 0; j < layers[k+1]; j++) {
                    if (rand() % 2)
                        weights[k][i][j] = (double)rand() / RAND_MAX;
                    else
                        weights[k][i][j] = -(double)rand() / RAND_MAX;
                }
            }
        }
    }
    vector<double> evaluate(int picture_number) {
//        string fileName = R"(C:\Users\kofta\source\HDL\HDL\Images\)" + to_string(picture_number) + ".bmp";
//        wstring temporary = wstring(fileName.begin(), fileName.end());
//        const wchar_t* widecstr = temporary.c_str();
//        Gdiplus::Bitmap bitmap(widecstr);
//
//        for (size_t i = 0; i < 49; i++) {
//            auto* temp = new Gdiplus::Color();
//            bitmap.GetPixel(i % 7, i / 7, temp);
//            temp->GetB() == 0 ? neuron[0][i] = 1 : neuron[0][i] = 0;
//        }

        BMP inputImage;
        string fileName = R"(C:\Users\kofta\CLionProjects\NeuronNetwork\Images\)" + to_string(picture_number) + ".bmp";
        inputImage.ReadFromFile(fileName.c_str());

        for (size_t i = 0; i < 49; i++) {
            int temp = inputImage(i % 7, i / 7)->Blue;
            temp == 0 ? neuron[0][i] = 1 : neuron[0][i] = 0;
        }

        for (size_t k = 1; k < neuron.size(); k++) {
            for (size_t i = 0; i < neuron[k].size(); i++) {
                double sum = 0;
                for (size_t j = 0; j < neuron[k - 1].size(); j++) {
                    sum += neuron[k-1][j] * weights[k-1][j][i];
                }
                neuron[k][i] = 1 / (1 + exp(-sum));
            }
        }
        return neuron.back();
    }

    void learn(int epoch_max, double alpha) {
        ifstream inputFile(R"(C:\Users\kofta\CLionProjects\NeuronNetwork\target.csv)");
        vector<vector<double>> targetArray;
        targetArray.resize(150);
        for (size_t i = 0; i < targetArray.size(); i++) {
            targetArray[i].resize(3);
        }

        string line;
        int lineNumber = 0;
        while (getline(inputFile, line)) {
            string tempStr;

            stringstream inputString(line);
            for (size_t i = 0; i < 3; i++) {
                getline(inputString, tempStr, ',');
                targetArray[lineNumber][i] = stod(tempStr);
            }
            lineNumber++;
        }

        for (int epoch = 0; epoch < epoch_max; epoch++) {
//            if (epoch % 10 > 3)
            evaluate(epoch % 150);
//            else
//                continue;

            double difSum = 0;
            for (size_t i1 = 0; i1 < 3; i1++) {
                difSum += abs(targetArray[epoch % 150][i1] - neuron.back()[i1]);
            }

            double errorFun = difSum / 2;
            vector<vector<double>> delta(neuron);

            for (size_t i1 = 0; i1 < neuron.back().size(); i1++) {
                delta.back()[i1] = neuron.back()[i1] * (1 - neuron.back()[i1])
                        * (targetArray[epoch % 150][i1] - neuron.back()[i1]);
            }

            for (size_t k = neuron.size() - 1; k > 0; k--) {
                for (size_t i = 0; i < neuron[k-1].size(); i++) {
                    double sum = 0;
                    for (size_t j = 0; j < neuron[k].size(); j++) {
                        sum += delta[k][j] * weights[k - 1][i][j];
                    }
                    delta[k - 1][i] = neuron[k-1][i] * (1 - neuron[k-1][i]) * sum;
                }
            }

            for (size_t k = neuron.size() - 1; k > 0; k--) {
                for (size_t i = 0; i < neuron[k].size(); i++) {
                    for (size_t j = 0; j < neuron[k-1].size(); j++) {
                        weights[k - 1][j][i] += alpha * delta[k][i] * neuron[k-1][j];
                    }
                }
            }

//        cout << epoch << endl;
//        cout << output[0] << " " << output[1] << " " << output[2] << " " << endl;
//        cout << errorFun << endl;
        }
    }

    void print_network() {
        cout << "neuron" << " {";
        for (size_t i = 1; i < neuron.size() - 1; i++) {
            cout << "{";
            for (size_t j = 0; j < neuron[i].size(); j++) {
                cout << neuron[i][j];
                if (j < neuron[i].size() - 1) cout << ", ";
            }
            cout << "}";
            if (i< neuron.size() - 2) cout << "," << endl;
        }
        cout << "}" << endl << endl;


        cout << "weights" << " {";
        for (int k = 0; k < weights.size(); ++k) {
            cout << "{";
            for (size_t i = 0; i < weights[k].size(); i++) {
                cout << "{";
                for (size_t j = 0; j < weights[k][i].size(); j++) {
                    cout << weights[k][i][j];
                    if (j < weights[k][i].size() - 1) cout << ", ";
                }
                cout << "}";
                if (i < weights[k].size() - 1) cout << "," << endl;
            }
            cout << "}, " << endl;
        }
        cout << "}" << endl;
    }

    static void print_picture(int picture_number) {
        BMP inputImage;
        string fileName = R"(C:\Users\kofta\CLionProjects\NeuronNetwork\Images\)" + to_string(picture_number) + ".bmp";
        inputImage.ReadFromFile(fileName.c_str());
        cout << "{";
        for (size_t i = 0; i < 49; i++) {
            int temp = inputImage(i % 7, i / 7)->Blue;
            temp == 0 ? cout << 1 : cout << 0;
            if (i < 48) cout << ", ";
            if (i % 7 == 6) cout << endl;
        }
        cout << "}";
    }
};