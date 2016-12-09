#mlp_my_predict.R

# Sets up the neural network emulator which
# is implemented in R.

# The file contains functions for simulating RSNNS predict()
# function which is implemented in R.
# This file does nothing when sourced by itself. It
# is meant to be sourced in by another R file. 
# See example rsnns_plot_with_simulator.R on how
# to do this.

Act_logistic <- function (x) {
1 / (1 + exp(-x))}

# setup_my_predict uses low level functions to get the
# activation biases, weighting matrix, and neural network
# configuration which are needed to run my_predict().
# We assume the network does not have more than 3 hidden layers.
# The setup function also creates a vector
# layers = c('layer2', 'layer3') depending on the number of
# hidden layers. Each layer(n) contains the node numbers
# of the nodes in that layer.
setup_my_predict <- function () {
  bias <<- c() # biases for activation for all nodes
  sObj <<- nn$snnsObject
  weightvalues <<- weightMatrix(nn)
  layers <<- c() # list of layers need for activation
  layer2 <<- c() # list of nodes in first hidden layer
  layer3 <<- c() # list of nodes in 2nd hidden layer
  layer4 <<- c() # list of nodes in 3rd hidden layer
  output <<- c() # list of nodes in output layer
  nunits <<- sObj$getNoOfUnits() # number of nodes
  #cat ('\nnumber of units = ', nunits, '\n')
  # assign each node to one of the layers, extract bias
  for (i in 1:nunits) {
    units <- sObj$getUnitName(i)
    bias <<- c(bias, sObj$getUnitBias(i))
    node_descriptor <- strsplit(units, '_')
    node_descriptor <- unlist(node_descriptor)
    if (node_descriptor[1] == 'Hidden') {
      if (node_descriptor[2] == '2') {
        layer2 <<- c(layer2, i)
      }
      if (node_descriptor[2] == '3') {
        layer3 <<- c(layer3, i)
      }
      if (node_descriptor[2] == '4') {
        layer4 <<- c(layer3, i)
      }
    }
    if (node_descriptor[1] == 'Output') {
      output <<- c(output, i)
    }
  }
  # create list of layers needed in neural network
  if (length(layer2) > 0) {
    layers <<- c(layers, 'layer2')
  }
  if (length(layer3) > 0) {
    layers <<- c(layers, 'layer3')
  }
  if (length(layer4) > 0) {
    layers <<- c(layers, 'layer4')
  }
  layers <<- c(layers, 'output')
}


# given a single data vector, this function returns the activation
# levels of each node in the neural network.
my_predict_single <- function (data) {
  xinput <- as.matrix(data)
  nrows = nrow(xinput)
  nextra <- (nunits - Dim)*nrows 
  # copy data into hdata and fill remainder with zeros.
  hdata <- matrix(c(xinput, rep(0.0, nextra)), ncol = nunits, nrow = 1)
  nlayers <- length(layers) # number of layers in network
  activity <- rep(0.0,nunits) # initialize activation vector

  for (i in 1:Dim) {activity[i] <- data[i]}
  n <- 1
  while (n <= nlayers) {
    hdata <- hdata %*% weightvalues
    nodes_in_layer <- get(layers[n])
    for (i in nodes_in_layer) {
      hdata[i] <- Act_logistic(hdata[i] + bias[i])
      activity[i] <- hdata[i]
    }
    n <- n + 1
  }
  return (unlist(activity))
}

# Given the test or train data as a matrix, the function
# returns the classification vectors (just like the mlp
# predict() function). There is an option for disabling
# particular hidden nodes given a list of their node
# numbers. If a node is disabled, its activation is locked
# to zero. This way you can determine the effect of a
# particular hidden node in a neural network.
my_predict <- function (data,disabled_nodes=NULL) {
  xinput <- as.matrix(data)
  nrows = nrow(xinput)
  #nextra <- (nunits - Dim)*nrows 
  nextra <- (nunits - Dim) 
  # copy data into hdata and fill remainder with zeros.
  zeros <- matrix(rep(0.0,nextra),ncol=nextra,nrow=nrows)
  hdata <- cbind(xinput,zeros)
  nlayers <- length(layers) # number of layers in network
  activity <- c()
  for (i in 1:Dim) {activity[i] <- data[i]}
  n <- 1
  while (n <= nlayers) {
    hdata <- hdata %*% weightvalues
    nodes_in_layer <- get(layers[n])
    for (i in nodes_in_layer) {
      if (i %in% disabled_nodes) {
        hdata[,i] = 0.0
      } else {
        hdata[,i] <- Act_logistic(hdata[,i] + bias[i])
        }
      }
    n <- n + 1
  }
  return (hdata[,(ncol(hdata)-nclasses+1):ncol(hdata)])
}

# computes activation distributions of hidden nodes
# for plotting.
my_predict_activation <- function (data) {
  xinput <- as.matrix(data)
  nrows = nrow(xinput)
  activity <- c()
  #nextra <- (nunits - Dim)*nrows 
  nextra <- (nunits - Dim) 
  # copy data into hdata and fill remainder with zeros.
  zeros <- matrix(rep(0.0,nextra),ncol=nextra,nrow=nrows)
  hdata <- cbind(xinput,zeros)
  activation <- hdata
  nlayers <- length(layers) # number of layers in network
  for (i in 1:Dim) {activity[i] <- data[i]}
  n <- 1
  while (n <= nlayers) {
    hdata <- hdata %*% weightvalues
    nodes_in_layer <- get(layers[n])
    for (i in nodes_in_layer) {
      hdata[,i] <- Act_logistic(hdata[,i] + bias[i])
      activation[,i] <- hdata[,i]
      }
    n <- n + 1
  }
  return (activation)
}

library(plot3D)

plot_activation <- function (y1,y2,activation,node) {
  scatter2D(y1,y2,clim = c(0.0,1.0), colvar=activation[,node])
  }


