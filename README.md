# squareworld-q-learning

Q-learning to avoid bullets

- uses a neural network as our Q-function approximator

## build/run

### build

- `make` - for full build with visualizer
- `make VISUALIZER=0` - for "headless" build without visualizer, no SDL2 dependency

### run examples

#### train
- `./learn t` - start training, press ENTER to halt and save
- `./learn tr` - resume training from a saved model
- `./learn t 10000` - train a new model for 10K generations

#### visualize
- `./learn v` - run the visualizer from the saved model
- `./learn v 2` - run the visualizer, with scale 2

## tweaking the model

`src/Learner.h` contains constants that control the neural network topology and training.

- `REPLAY_MEMORY_SIZE` is the number of "attempts" or `observation`s to store before training
- `TRAIN_LOOPS` is the number of times to train on each observation set
- `HIDDEN_LAYERS` is the number of hidden layers
- `HIDDEN_LAYER_NEURONS` is the number of neurons in each hidden layer

`src/main.cpp` also contains constants for learning.

- `DEFAULT_LEARN_RATE` - the learning rate for the neural network
- `DEFAULT_DISCOUNT_FACTOR` - the discount factor for Q-learning
- `DEFAULT_RANDOM_RATE` - the rate of random moves (for exploration)

![](AMAZING_NEURAL_NETWORK_MEME.jpg)
