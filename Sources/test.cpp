#include <torch/torch.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include<vector>
#include"ppo_torch.cpp"

using namespace std;

int generator()
{
    int n = 0;
    return n + 2;

}

int main()
{
    PPO_Memory ppo_memory(2);
    ppo_memory.states = {2, 3, 4, 5, 6, 4, 5};
    vector<double> x;
    vector<double> c;
    vector<double> v;
    vector<double> b;
    vector<double> f;
    vector<double> g;
    vector<vector<long long>> test;
    tie(x, c, v, b, f, g, test) = ppo_memory.generate_batches();
    
    cout << test.size() << endl;
    cout << x << endl;

    
    /*
    for(int i = 0; i < test.size(); i++)
    {
        cout << test[i] << endl;
    }
    */
    

    return 0;
}