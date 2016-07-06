#ifndef __NEURAL_NET_H__
#define __NEURAL_NET_H__

class NeuralNet {
public:
    NeuralNet();
    NeuralNet(const NeuralNet&);
    NerualNet& operator=(const NerualNet&);
    ~NeuralNet();
private:
    class Impl;
    Impl* mImpl;
};

#endif // __NEURAL_NET_H__
