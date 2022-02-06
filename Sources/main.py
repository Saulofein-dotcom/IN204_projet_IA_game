import gym
from matplotlib.pyplot import figure
import numpy as np
from ppo_torch import Agent
from utils import plot_learning_curve
import os
import io
import base64
import subprocess
import json
import tempfile
import time
from gym import wrappers

def get_make_env_fn(**kargs):
    def make_env_fn(env_name, seed=None, render=None, record=False,
                    unwrapped=False, monitor_mode=None, 
                    inner_wrappers=None, outer_wrappers=None):
        mdir = tempfile.mkdtemp()
        env = None
        if render:
            try:
                env = gym.make(env_name, render=render)
            except:
                pass
        if env is None:
            env = gym.make(env_name)
        if seed is not None: env.seed(seed)
        env = env.unwrapped if unwrapped else env
        if inner_wrappers:
            for wrapper in inner_wrappers:
                env = wrapper(env)
        env = wrappers.Monitor(
            env, mdir, force=True, 
            mode=monitor_mode, 
            video_callable=lambda e_idx: record) if monitor_mode else env
        if outer_wrappers:
            for wrapper in outer_wrappers:
                env = wrapper(env)
        return env
    return make_env_fn, kargs

if __name__ == '__main__':
    env = gym.make('SpaceInvaders-v0')
    
    N = 20
    batch_size = 5
    n_epochs = 4
    alpha = 0.0003
    make_env_fn, make_env_kargs = get_make_env_fn(env_name='CartPole-v0')

    agent = Agent(n_actions=env.action_space.n, batch_size=batch_size,
                    alpha=alpha, n_epochs=n_epochs,
                    input_dims=env.observation_space.shape,
                    make_env_fn=make_env_fn, make_env_kargs=make_env_kargs)
    n_games = 300

    figure_file = "./plots/cartpole.png"
    
    best_score = env.reward_range[0]
    print(best_score)
    score_history = []

    learn_iters = 0
    avg_score = 0
    n_steps = 0   



    for i in range(n_games):
        observation = env.reset()
        done = False
        score = 0
        while not done:
            action, prob, val = agent.choose_action(observation)
            observation_, reward, done, info = env.step(action)
            n_steps += 1
            score += reward
            agent.remember(observation, action, prob, val, reward, done)
            if n_steps % N == 0:
                agent.learn()
                learn_iters +=1
            observation = observation_
        score_history.append(score)
        avg_score = np.mean(score_history[-100:])

        if avg_score > best_score:
            best_score = avg_score
            agent.save_models()

        print("episode", i, "score %.1f" % score, "avg_score %.1f" % avg_score,
                "time steps", n_steps, "learning_steps", learn_iters)

        x = [i+1 for i in range(len(score_history))]
        plot_learning_curve(x, score_history, figure_file)

    agent.demo_last()
            