#include <torch/torch.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <string>
#include <random>
#include <math.h>

namespace T = torch;
namespace nn = torch::nn;
namespace optim = torch::optim;
using namespace std;

template<typename A>
vector<A> appliedOn(vector<A> myVect, T::Tensor batch)
{
    vector<A> myNewVect = {};
    for(int i = 0; i < batch.size(-1); i++)
    {
        myNewVect.push_back(myVect[batch[i].item<int>()]);
    }
    return myNewVect;
}

class PPO_Memory
{
public:
    vector<vector<double>> states;
    vector<double> probs;
    vector<double> vals;
    vector<int> actions;
    vector<double> rewards;
    vector<bool> dones;
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
    //static int myrandom(int i) { return rand() % i; }

public:
    PPO_Memory(long batch_size)
    {
        this->states = vector<vector<double>>({});
        this->probs = vector<double>({});
        this->vals = vector<double>({});
        this->actions = vector<int>({});
        this->rewards = vector<double>({});
        this->dones = vector<bool>({});

        this->batch_size = batch_size;
    }

    auto generate_batches()
    {

        long n_states = this->states.size();

        T::Tensor batch_start = T::arange(0, n_states, this->batch_size);

        //T::Tensor indices = T::arange(0, n_states, 1);

        T::Tensor random_indices = T::randperm(n_states);
        vector<T::Tensor> batches = {};
        
        if(this->states.size()==0)
        {
            cout << "states of size 0" << endl;
            //exit(0); 
        }
        else
        {
            for(int i = 0; i < batch_start.size(-1); i++)
            {
                T::Tensor newTensor = T::tensor({});
                for(int j = 0; j < this->batch_size && batch_start[i].item<int>() + j < random_indices.size(-1); j++)
                {
                    int tmp = random_indices[batch_start[i].item<int>() + j].item<int>();
                    if (j==0) newTensor = T::tensor({tmp});
                    else newTensor = T::cat({newTensor, T::tensor({tmp})});
                }
                batches.push_back(newTensor);
            }
        }

        return make_tuple(this->states,
                          this->actions,
                          this->probs,
                          this->vals,
                          this->rewards,
                          this->dones,
                          batches); // tie(x, c, v, b, f, g, test) = ppo_memory.generate_batches()
    }

    vector<double> tensorToVector(T::Tensor state)
    {
        vector<double> myVec(state.size(-1));
        for(int i = 0; i < state.size(-1) ; i++)
        {
            myVec[i] = state[0][i].item<double>();
        }
        return myVec;
    }

    void store_memory(T::Tensor state, c10::Scalar action, c10::Scalar probs, c10::Scalar vals, double reward, bool done)
    {
       
       this->states.push_back(tensorToVector(state));
       this->actions.push_back(action.to<int>());
       this->probs.push_back(probs.to<double>());
       this->vals.push_back(vals.to<double>());
       this->rewards.push_back(reward);
       this->dones.push_back(done);
        
    }

    void clear_memory()
    {
        this->states = vector<vector<double>>({});
        this->probs = vector<double>({});
        this->vals = vector<double>({});
        this->actions = vector<int>({});
        this->rewards = vector<double>({});
        this->dones = vector<bool>({});
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

    
    T::Tensor forward(T::Tensor state)
    {
        T::Tensor dist = this->actor->forward(state);
        
        return dist;
    }
    

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
                double policy_clip=0.2, long batch_size=64, long n_epochs=10, long N=2048)
        {
            this->gamma = gamma;
            this->policy_clip = policy_clip;
            this->n_epochs = n_epochs;
            this->gae_lambda = gae_lambda;

            int fc1_dims = 4096;
            int fc2_dims = 4096;

            this->actor = new ActorNetwork(n_actions, input_dims, alpha, fc1_dims, fc2_dims);
            this->critic = new CriticNetwork(input_dims, alpha, fc1_dims, fc2_dims);
            this->memory = new PPO_Memory(batch_size);

        }

        T::Tensor sampleAction(T::Tensor dist)
        {
            random_device rd;  // Will be used to obtain a seed for the random number engine
            mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
            uniform_real_distribution<> dis(0.0, 1.0);

            double random_number = dis(gen);

            T::Tensor action = T::tensor({0});
            float tmp = 0;

            bool done = false;
            for(int i = 0; i < dist.size(1) && !done; i++)
            {
                tmp += dist[0][i].item<double>();
                if(random_number<=tmp)
                {
                    action[0] = i;
                    done = true;
                }

            }

            return action;
        }

        
        void remember(T::Tensor state, c10::Scalar action, c10::Scalar probs, c10::Scalar vals, double reward, bool done)
        {
            this->memory->store_memory(state, action, probs, vals, reward, done);
        }

        void save_models()
        {
            cout << "...saving models..." << endl;
            this->actor->save_checkpoint();
            this->critic->save_checkpoint();
            cout << "...models saved..." << endl;
        }

        void load_models()
        {
            cout << "...loading models..." << endl;
            this->actor->load_checkpoint();
            this->critic->load_checkpoint();
            cout << "...models loaded..." << endl;
        }

        
        auto choose_action(T::Tensor observation)
        {
            
            T::Tensor state = observation.to(*this->actor->device);
            
            
            T::Tensor dist = this->actor->actor->forward(state);
            T::Tensor value = this->critic->critic->forward(state);
            c10::Scalar probsReturn;
            
            long action;

            if(dist.size(0)==0 || dist.size(-1)==0 )
            {
            }else
            {
                action = this->sampleAction(dist).item<long>();
                double logAction = log(dist[-1][action]).item<double>();
                probsReturn = T::squeeze(T::tensor({logAction})).item();
            }
            
            c10::Scalar actionReturn = T::squeeze(T::tensor(action)).item();
            c10::Scalar valueReturn = T::squeeze(value).item();

            return make_tuple(actionReturn, probsReturn, valueReturn);

        }


        
        void learn()
        {
            for(int NULL_VAR = 0; NULL_VAR < this->n_epochs; NULL_VAR++)
            {
                vector<vector<double>> state_arr;
                vector<double> old_prob_arr;
                vector<double> vals_arr;
                vector<int> action_arr;
                vector<double> reward_arr;
                vector<bool> dones_arr;

                vector<T::Tensor> batches;
                tie(state_arr, action_arr, old_prob_arr, vals_arr,
                    reward_arr, dones_arr, batches) = this->memory->generate_batches();

                

                vector<double> values = vals_arr;
                T::Tensor advantage = T::zeros(reward_arr.size(), T::dtype(T::kFloat32));

                
                
                for(int t = 0; t < reward_arr.size() - 1; t++)
                {
                    
                    double discount = 1;
                    double a_t = 0;
                    for(int k = t; k < reward_arr.size() - 1; k++)
                    { 

                        a_t += discount*(reward_arr[k] + this->gamma*values[k+1] * (1 - dones_arr[k]) - values[k]);

                        discount *= this->gamma * this->gae_lambda;
                    }
                    advantage[t] = a_t;
                }
                
                
                advantage.size(0)==0 ? advantage = advantage.to(*this->actor->device) : advantage = advantage.data().to(*this->actor->device);
                T::Tensor valuesToReturn;
                values.size()==0 ? valuesToReturn = T::tensor(values).to(*this->actor->device) : valuesToReturn = T::tensor(values).data().to(*this->actor->device);

                for(int i = 0; i < batches.size() ; i++)
                {
                    T::Tensor batch = batches[i];

                    vector<vector<double>> myArray = appliedOn(state_arr, batch);
                    
                    auto options = torch::TensorOptions().dtype(at::kFloat);
                    long n = myArray.size();
                    long m = myArray[0].size();
                    T::Tensor states = torch::zeros({n, m}, options);
                    for (int i = 0; i < n; i++)
                        states.slice(0, i,i+1) = torch::from_blob(myArray[i].data(), {m}, options);
                    
                    states = states.to(*this->actor->device);
                    T::Tensor old_probs = T::tensor(appliedOn(old_prob_arr,batch)).to(*this->actor->device);
                    T::Tensor actions = T::tensor(appliedOn(action_arr,batch)).to(*this->actor->device);
                    T::Tensor dist = this->actor->actor->forward(states);

                    T::Tensor critic_value = this->critic->critic->forward(states);
                    critic_value = T::squeeze(critic_value);
                    
                    
                    T::Tensor new_probs = T::zeros(actions.size(-1));
                    for(int i = 0; i < actions.size(-1); i++) new_probs[i] = log(dist[i][actions[i]]);
                    
                    T::Tensor prob_ratio = new_probs.exp() / old_probs.exp();
                    
                    
                    
                    T::Tensor advantage_batch = appliedOnTensor(advantage, batch);

                    T::Tensor weighted_probs = advantage_batch * prob_ratio;
                    T::Tensor weighted_clipped_probs = T::clamp(prob_ratio, 1 - this->policy_clip, 1 + this->policy_clip) * advantage_batch;
                     
                    T::Tensor actor_loss = -T::min(weighted_probs, weighted_clipped_probs).mean();
                    
                    T::Tensor valuesToReturnBatch = appliedOnTensor(valuesToReturn, batch);
                    T::Tensor returns = advantage_batch + valuesToReturnBatch;
                    
                    T::Tensor critic_loss = (returns - critic_value)*(returns - critic_value);
                    
                    critic_loss = critic_loss.mean();
                     

                    T::Tensor total_loss = actor_loss + 0.5 * critic_loss;
                    this->actor->optimizer->zero_grad();
                    this->critic->optimizer->zero_grad();
                    total_loss.backward();
                    this->actor->optimizer->step();
                    this->critic->optimizer->step();
                    
                }
                
            }
            this->memory->clear_memory();
            
        }

        T::Tensor appliedOnTensor(T::Tensor myTensor, T::Tensor batch)
        {
            T::Tensor newTensor = T::zeros(batch.size(-1));
            for(int i = 0; i < batch.size(-1); i++)
            {
                newTensor[i] = myTensor[batch[i]];
            }
            return newTensor;
        }
        

};