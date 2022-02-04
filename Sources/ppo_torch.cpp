#include <torch/torch.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <string>

namespace T = torch;
namespace nn = torch::nn;
namespace optim = torch::optim;
using namespace std;

class PPO_Memory
{
public:
    T::Tensor states;
    T::Tensor probs;
    T::Tensor vals;
    T::Tensor actions;
    T::Tensor rewards;
    T::Tensor dones;
    long batch_size;

    /*
    template <typename T>
    vector<T> arange(T begin, T end, int jump)
    {
        vector<T> myVector = {begin};
        int tmp = begin + jump;
        while (tmp < end)
        {
            myVector.push_back(tmp);
            tmp += jump;
        }
        return myVector;
    }
    */

    // random generator function:
    static int myrandom(int i) { return rand() % i; }

public:
    PPO_Memory(long batch_size)
    {
        this->states = T::Tensor();
        this->probs = T::Tensor();
        this->vals = T::Tensor();
        this->actions = T::Tensor();
        this->rewards = T::Tensor();
        this->dones = T::Tensor();

        this->batch_size = batch_size;
    }

    auto generate_batches()
    {
        srand(time(NULL));

        long n_states = this->states.size(0);

        T::Tensor batch_start = T::arange(0, n_states, this->batch_size);

        T::Tensor indices = T::arange(0, n_states, 1);

        T::Tensor random_indices = T::randperm(n_states);

        indices = indices[random_indices];

        T::Tensor batches;

        for(int i = 0; i < batch_start.size(0); i++)
        {
            T::Tensor newTensor = T::zeros(this->batch_size);
            for(int j = 0; j < this->batch_size; j++)
            {
                newTensor[j] = indices[batch_start[i] + j];
            }
            batches = T::cat({batches, newTensor});
        }

        return make_tuple(this->states,
                          this->actions,
                          this->probs,
                          this->vals,
                          this->rewards,
                          this->dones,
                          batches); // tie(x, c, v, b, f, g, test) = ppo_memory.generate_batches()
    }

    void store_memory(double state, double action, double probs, double vals, double reward, double done)
    {
        this->states = T::cat({this->states, T::tensor(state)});
        this->actions = T::cat({this->states, T::tensor(action)});
        this->probs = T::cat({this->states, T::tensor(probs)});
        this->vals = T::cat({this->states, T::tensor(vals)});
        this->rewards = T::cat({this->states, T::tensor(reward)});
        this->dones = T::cat({this->states, T::tensor(done)});
    }

    void clear_memory()
    {
        this->states = T::Tensor();
        this->probs = T::Tensor();
        this->vals = T::Tensor();
        this->actions = T::Tensor();
        this->rewards = T::Tensor();
        this->dones = T::Tensor();
    }
};




class ActorNetwork : public nn::Module
{
public:
    string checkpoint_file;
    nn::Sequential actor;
    optim::Adam *optimizer;
    T::Device* device;

    ActorNetwork(long int n_actions, int input_dims, double alpha, long int fc1_dims=256, long int fc2_dims=256, string chkpt_dir = "../tmp/ppo") : nn::Module()
    {
        this->checkpoint_file = chkpt_dir;
        this->checkpoint_file += "/actor_torch_ppo.pt";
        actor = nn::Sequential(
            nn::Linear(input_dims, fc1_dims),
            nn::ReLU(),
            nn::Linear(fc1_dims, fc2_dims),
            nn::ReLU(),
            nn::Linear(fc2_dims, n_actions),
            nn::Softmax(-1) // Comment on met dim = -1 en argument ?
        );
        this->optimizer = new optim::Adam(this->parameters(), T::optim::AdamOptions(alpha));
        this->device = new T::Device(T::cuda::is_available() ? T::kCUDA : T::kCPU);
        this->actor->to(*this->device);
    }

    /*
    T::Tensor forward(T::Tensor input)
    {
    }
    */

    void save_checkpoint()
    {
        T::serialize::OutputArchive output_archive;
        this->actor->save(output_archive);
        output_archive.save_to(this->checkpoint_file);
    }

    void load_checkpoint()
    {
        T::load(this->actor, this->checkpoint_file);
    }
};

class CriticNetwork : public nn::Module
{
    public:
        string checkpoint_file;
        nn::Sequential critic;
        optim::Adam* optimizer;
        T::Device* device;
    


    public:
        CriticNetwork(long long input_dims, double alpha, long fc1_dims=256, long fc2_dims=256, string chkpt_dir="../tmp/ppo" ) : nn::Module()
        {
            this->checkpoint_file = chkpt_dir + "/critic_torch_ppo.pt";
            this->critic = nn::Sequential(
                                    nn::Linear(input_dims, fc1_dims),
                                    nn::ReLU(),
                                    nn::Linear(fc1_dims, fc2_dims),
                                    nn::ReLU(),
                                    nn::Linear(fc2_dims, 1)
            );
            
            this->optimizer = new optim::Adam(this->parameters(), optim::AdamOptions(alpha));
            this->device = new T::Device(T::cuda::is_available() ? T::kCUDA : T::kCPU);
            this->critic->to(*this->device);
        }

        T::Tensor forward(T::Tensor state)
        {
            T::Tensor tmp = this->critic->forward(state);
            cout << tmp << endl;
            return tmp;
        }

        void save_checkpoint()
        {
            T::serialize::OutputArchive output_archive;
            this->critic->save(output_archive);
            output_archive.save_to(this->checkpoint_file);
        }

        void load_checkpoint()
        {
            T::load(this->critic, this->checkpoint_file);
        }

};


class Agent
{
    public:
        double gamma;
        double policy_clip;
        long n_epochs;
        double gae_lambda;

        ActorNetwork* actor;
        CriticNetwork* critic;
        PPO_Memory* memory;

    public:
        Agent(long n_actions, long input_dims, double gamma=0.99, double alpha=0.0003, double gae_lambda=0.95,
                double policy_clip=0.2, long batch_size=64, long N=2048, long n_epochs=10)
        {
            this->gamma = gamma;
            this->policy_clip = policy_clip;
            this->n_epochs = n_epochs;
            this->gae_lambda = gae_lambda;

            this->actor = new ActorNetwork(n_actions, input_dims, alpha);
            this->critic = new CriticNetwork(input_dims, alpha);
            this->memory = new PPO_Memory(batch_size);

        }

        
        void remember(auto state, auto action, auto probs, auto vals, auto reward, auto done)
        {
            this->memory->store_memory(state, action, probs, vals, reward, done);
        }

        void save_models()
        {
            cout << "...saving models..." << endl;
            this->actor->save_checkpoint();
            this->critic->save_checkpoint();
        }

        void load_models()
        {
            cout << "...loading models..." << endl;
            this->actor->load_checkpoint();
            this->critic->load_checkpoint();
        }

        
        void choose_action(vector<double> observation)
        {
            T::Tensor state = T::tensor(observation, T::dtype(T::kFloat32)).to(*this->actor->device);
            
            T::Tensor dist = this->actor->actor->forward(state);
            T::Tensor value = this->critic->critic->forward(state);
            /*A COMPLETER*/
        }

        void learn()
        {
            for(int NULL_VAR = 0; NULL_VAR < this->n_epochs; NULL_VAR++)
            {
                vector<double> state_arr; 
                vector<double> action_arr;
                vector<double> old_prob_arr; 
                vector<double> vals_arr; 
                vector<double> reward_arr; 
                vector<double> dones_arr;
                vector<vector<long long>> batches;
                

                tie(state_arr, action_arr, old_prob_arr, vals_arr,
                    reward_arr, dones_arr, batches) = this->memory->generate_batches();
                
                auto values = vals_arr;
                auto advantage = T::zeros(reward_arr.size(), T::dtype(T::kFloat32));
                
                for(int t = 0; t < reward_arr.size() - 1; t++)
                {
                    
                    double discount = 1;
                    double a_t = 0;
                    for(int k = t; k < reward_arr.size() - 1; k++)
                    { 
                        cout << reward_arr[k] << endl;
                        cout << "OK" << endl;
                        a_t += discount*(reward_arr[k] + this->gamma*values[k+1]* (1 - (int) dones_arr[k]) - values[k]);
                        
                        discount *= this->gamma * this->gae_lambda;
                    }
                    advantage[t] = a_t;
                }

                advantage = advantage.to(*this->actor->device);
            }
        }
        

};