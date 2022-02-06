#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include"ppo_torch.cpp"
#include "./create_plot/pbPlots.hpp"
#include "./create_plot/supportLib.hpp"



using namespace std;

template<typename A>
double average(std::vector<A> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<double>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}

int main()
{

    int N = 20;
    int batch_size = 5;
    int n_epochs = 4;
    double alpha = 0.0003;
    int action_space_n = 2;
    int observation_space_shape = 4;
    double gamma = 0.99;
    double gae_lambda = 0.95;
    double policy_clip = 0.2;

    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(-0.05, 0.05);

    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();

    Agent agent = Agent(action_space_n, observation_space_shape, gamma, alpha, gae_lambda, policy_clip, batch_size, n_epochs);
    int n_games = 300;

    string figure_file = ".plots/cartpole.png";

    vector<long> score_history;
    
    double best_score = std::numeric_limits<double>::min();
    long learn_iters = 0;
    double avg_score = 0;
    long n_steps = 0;

    for(int i = 0; i < 0/*n_games*/ ; i++)
    {
        T::Tensor observation = T::tensor({{dis(gen),dis(gen),dis(gen),dis(gen)}});
        bool done = false;
        long score = 0;
        while(!done)
        {
            c10::Scalar action ,prob, val;
            
            tie(action, prob, val) = agent.choose_action(observation);
            
            T::Tensor observation_ = observation;
            double reward = 1.0;
            done = false;

            n_steps += 1;
            score += reward;
            
            agent.remember(observation, action, prob, val, reward, done);
            if(n_steps % N == 0)
            {
                
                agent.learn();
                learn_iters += 1;
                done = true; //A SUPPRIMER A L'AVENIR
            }
            observation = observation_;

        }
        score_history.push_back(score);
        int minValue;
        score_history.size() < 100 ? minValue = score_history.size() : minValue = 100;
        vector<int64_t> score_tmp(score_history.end() - minValue, score_history.end());
        
        double avg_score = average(score_tmp);
        if(avg_score > best_score)
        {
            best_score = avg_score;
            agent.save_models();
        }

        cout << "episode " << i << " score "<< score << ", avg_score : " << avg_score
                << ", time steps " << n_steps << ", learning_steps " << learn_iters << endl;

        /*================ PLOT SCORE =================*/

        vector<double> x(score_history.size());
        for(int l = 0; l < score_history.size(); l++)
        {
            x[l] = l+1;
        }
        
        vector<double> running_avg(score_history.size());
        for(int m = 0; m < running_avg.size(); m++)
        {
            int tmpMax;
            m - 100 > 0 ? tmpMax = m-100 : tmpMax = 0;

            vector<int64_t> score_tmp_run(score_history.begin() + tmpMax, score_history.begin() + m + 1);
            running_avg[m] = average(score_tmp_run);
            
        }

        StringReference* error = new StringReference();
        DrawScatterPlot(imageRef, 600, 400, &x, &running_avg, error);

        vector<double> *pngData = ConvertToPNG(imageRef->image);

        WriteToFile(pngData, "../create_plot/plotScore.png");
        DeleteImage(imageRef->image);
        delete error;
        
    }

    /* ---- A SUPPRIMER ---- */
    

    /*-----------------------*/

    return 0;
}