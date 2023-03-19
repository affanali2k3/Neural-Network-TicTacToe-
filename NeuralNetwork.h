#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <cmath>
using namespace std;

#define e 2.71828

struct Edges
{
    double weight;
    double changeOfWeight;
};



class Neuron
{
    private:
        float pace;
        double outputValue;
        vector<Edges> edgeData;
        int neuronIndex;
        double outputValueDerivative;
    public:
        Neuron(int numberOfOutputs)
        {
            pace = 0.5;
            for(int i = 0; i < numberOfOutputs; i++)
            {
                edgeData.push_back(Edges());
                edgeData.back().weight = rand()/double(RAND_MAX);
            }
        }

        void feedForward(vector <Neuron> &previousLayer, int neuronIndex)
        {
            this->neuronIndex = neuronIndex;
            double sum = 0;
            for(int j = 0; j < previousLayer.size(); j++)
            {
                sum += previousLayer[j].outputValue * previousLayer[j].edgeData[neuronIndex].weight;
            }
            outputValue = sigmoidFunction(sum);
             
        }


        double sigmoidFunction(double sum)
        {
            return 1/(1+exp(-1*sum));
        }

        double sigmoidFunctionDerivative(double sum)
        {
            return sigmoidFunction(sum) * (1 - sigmoidFunction(sum));
        }

        void updateNeuronWeight(vector<Neuron> &previousLayer, double finalOutputValue, double targetValue)
        {
            double test = 0;
            double newChangeOfWeight;
            for(int neuronNum = 0; neuronNum < previousLayer.size(); neuronNum++)
            {
                    if(previousLayer.size() == 5)
                    {
                        newChangeOfWeight = (outputValue - targetValue) * (outputValue * (1-outputValue)) * (previousLayer[neuronNum].outputValue);
                            previousLayer[neuronNum].edgeData[neuronIndex].weight -= (pace * newChangeOfWeight);
                    }

                    if(previousLayer.size() == 9)
                    {
                        double nextWeight = edgeData[0].weight;
                        newChangeOfWeight = (finalOutputValue - targetValue) * (finalOutputValue * (1-finalOutputValue)) * (nextWeight) * (outputValue * (1-outputValue)) * (previousLayer[neuronNum].outputValue);

                            previousLayer[neuronNum].edgeData[neuronIndex].weight -= (pace * newChangeOfWeight);
                    }

            }

        }

        friend class NeuralNetwork;
};



class NeuralNetwork
{
    private:
        vector<vector<Neuron>> Layers;
        vector<Neuron> neuronLayer;
        double rms;
    public:
        NeuralNetwork(const vector <int> &topology)
        {
            
            int numOfLayers = topology.size();
            for(int i = 0; i < numOfLayers; i++)
            {
                int numOfOutputs = i == topology.size() - 1? 0:topology[i+1];
                Layers.push_back(neuronLayer);
                for(int j = 0; j < topology[i]; j++)
                {
                    Layers[i].push_back(Neuron(numOfOutputs));
                }
            }
        }

        void feedForward(vector<double> &inputVals)
        {
            if(Layers[0].size() != inputVals.size())
            {
                cout << "Input values should be equal to 9\n";
                return;
            }
            else
            {
                for(int i = 0; i < Layers[0].size(); i++)
                {
                    Layers[0][i].outputValue = inputVals[i];
                }

                for(int layerNum = 1; layerNum < Layers.size(); layerNum++)
                {
                    for(int neuronNum = 0; neuronNum < Layers[layerNum].size(); neuronNum++)
                    {
                        Layers[layerNum][neuronNum].feedForward(Layers[layerNum-1], neuronNum);
                    }
                }
            }
        }

        void backwardPropogation(vector <double> &targetValues)
        {
            rms = 0;
            for(int neuronNum = 0; neuronNum < Layers.back().size(); neuronNum++)
            {
                double delta = targetValues[neuronNum] - Layers[2][neuronNum].outputValue;
                rms += sqrt((delta * delta)/Layers.back().size());
            }
            for(int layerNum = Layers.size()-1; layerNum > 0; layerNum--)
            {
                for(int neuronNum = 0; neuronNum < Layers[layerNum].size(); neuronNum++)
                {
                    Layers[layerNum][neuronNum].updateNeuronWeight(Layers[layerNum-1], Layers[2][0].outputValue, targetValues[0]);
                }
            }
        }

        double getResult()
        {
            return Layers[2][0].outputValue;
        }

        friend class Neuron;

};