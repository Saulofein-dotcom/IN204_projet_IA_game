#include <SFML/Graphics.hpp>
#include <iostream>
#include<vector>
#include"ppo_torch.cpp"

using namespace std;

int main()
{

    int N = 20;
    int batch_size = 5;
    int n_epochs = 4;
    double alpha = 0.0003;
    int action_space_n = 2;
    int observation_space_shape = 4;
    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(-0.05, 0.05);

    

    Agent agent = Agent(3, observation_space_shape, alpha=alpha, n_epochs=n_epochs);
    int n_games = 300;

    string figure_file = ".plots/cartpole.png";

    T::Tensor score_history;
    
    double best_score = std::numeric_limits<double>::min();
    long learn_iters = 0;
    double avg_score = 0;
    long n_steps = 0;

    for(int i = 0; i < n_games ; i++)
    {
        T::Tensor observation = T::tensor({{dis(gen),dis(gen),dis(gen),dis(gen)}});
        bool done = false;
        long score = 0;
        while(!done)
        {
            c10::Scalar action ,prob, val;
            tie(action, prob, val) = agent.choose_action(observation);
            done = true;
        }
        
    }

    return 0;
}