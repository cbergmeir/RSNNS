# mlp_eval.R

# Creates a training set and test set of samples uniformly distributed
# inside a square (or hypercube) which are perfectly separable
# into nclasses. The RSNNS mlp() and predict() functions are used
# to create a feed forward neural network model and estimate their
# accuracy. 

# You may experiment with this code by changing any of the following
# variables (dimension of the space, number of classes, number of
# samples in the training and test set, etc.)
Dim <- 2 # Dimension of parameter space
nclasses <- 5 # Number of classes
training.size <- 500 # Number of training samples
test.size <- 5000 # Number of test samples
hidden <- c(2,2) # Hidden layers
learningfunc <- 'SCG'
iterations <- 1000 # Number of iterations of the mlp algorithm
ntrials <- 2 # Number of times to run mlp on the same training set.
# Each time mlp runs it is initialized differently, so you will
# get different results.

learnfunc.selection <- c(
  'Std_Backpropagation',
  'BackpropBatch',
  'BackpropChunk',
  'BackpropMomentum',
  'BackpropWeightDecay',
  'Rprop',
  'QuickProp',
  'SCG'
)

cat("Reference: 'Experiments with Neural Networks using R' by Seymour Shlien\n")
cat("To change the model parameters (number of classes, training size, test size...)\nyou need to edit this file which you can download from the following\nsite")
cat(" http://ifdo.ca/~seymour/R\n\n")


# Comment the next line if you want a different training/test set
# each time you run this program.
set.seed(123456)

data.size <- training.size + test.size


#              Create Training Data

#              --------------------

# We create samples in a rectangular space
# 0.0 to 1.0 in all dimensions. The samples
# are divided into perfectly separated nclasses
# depending on which # voronoi region they occur.

# centers are the centroids of the voronoi regions
centers <- matrix(runif(Dim * nclasses, 0.05, 0.95), nrow = Dim)
data.samples <- matrix(runif(Dim * data.size, 0, 1), nrow = Dim)

euclid_distance <- function (x1, x2) {
  return (sqrt(sum((x1 - x2) ^ 2)))
}

nearest_center <- function (x) {
  d <- c()
  for (i in 1:nclasses) {
    d <- c(d, euclid_distance (x, centers[, i]))
  }
  return (which.min(d))
}


# Assign sample to the Voronoi region it belongs to.
# data.class is a vector containing the class number
# of each sample.
data.class <- c()
for (j in 1:data.size) {
  voronoi.index <- nearest_center(data.samples[, j])
  data.class <- c(data.class, voronoi.index)
}


classcol <- function (col, dim) {
  # converts a category to a column vector of dimension dim
  m <- matrix(0, dim, 1)
  m[col, 1] <- 1
  m
}



#          Neural Network
#          --------------


# Create formula for neural net algorithm
xdict <- paste0("x",1:16)
cdict <- paste0("c",1:16)
#xdict is a vector like this  c('x1', 'x2', 'x3', 'x4', 'x5', 'x6', ... x16)
f1 <- paste(cdict[1:nclasses], collapse = '+')
f2 <- paste(xdict[1:Dim], collapse = '+')
f <- paste(f1, f2, sep = ' ~ ', collapse = NULL)
formula <- as.formula(f)
print(formula)



# Assign x1,x2... to the columns of the samples matrix
# and append it to the training.data data.frame()
x1 <- data.samples[1, ]
data <- data.frame(x1)
for (i in c(2:Dim)) {
  assign(xdict[i], data.samples[i, ])
  data[xdict[i]] <- data.samples[i, ]
}

test.ptr <- training.size + 1
training.data <- data[1:training.size, ]
test.data <- data[test.ptr:data.size,]
training.class <- data.class[1:training.size]
test.class <- data.class[test.ptr:data.size]

# convert each training.class to a column vector of length nclasses
# where all entries are zero except for the one at column
# training.class. Join all the column vectors (for each sample)
# into a matrix of size training.size by nclasses.
class_matrix <- sapply(training.class, classcol, nclasses)

# Assign variables c1,c2, and etc. to the nrows of the class_matrix.
# c1[i] = 1 if sample i is assigned to class 1 otherwise 0.
# c2[i] = 1 if sample i is assigned to class 2 otherwise 0.
# and etc.
for (i in c(1:nclasses)) {
  assign(cdict[i], class_matrix[i, ])
}

plot_centroids <- function () {
par(pch = 19)
par(pty = 's')
par(cex=1.5)
par(cex.lab = 1.5)
par(cex.axis = 1.5)
plot(centers[1,],
     centers[2,],
     col = 1:5,
     asp = 1,
     ylim = c(0, 1),
     xlab='x1',
     ylab='x2')
print(' centroids')
for (k in 1:nclasses) {
  print(centers[,k])
  }
}

#pdf('v-centroids.pdf')
#plot_centroids()
#dev.off()


plot_voronoi_data <- function() {
# Plot training data on left
#pdf('voronoi2.pdf',height=4, width=4)
par(pch = 19)
#par(mfrow=c(1,2))
par(pty = 's')
#par(fin = c(7, 7))
par(cex.lab = 1.5)
par(cex.axis = 1.5)
plot(x1[1:training.size],
     x2[1:training.size],
     col = training.class,
     asp = 1,
     ylim = c(0, 1))
#dev.off()
}

plot_voronoi_data()


library(Rcpp)
library(RSNNS)

cat("\n\ntraining size = ", training.size, " samples\n")
cat("number of classes = ", nclasses, "\n")
cat("dimension of space = ", Dim, "\n")
cat("hidden layer(s) = ",hidden,'\n')
cat("learning function = ",learningfunc,'\n')

# mlp predict() produces a matrix of class vectors. We
# use this function to convert it to class numbers so
# we can plot the results or measure the accuracy.
class_matrix_to_vector <- function (modeled_output) {
  size <- nrow(modeled_output)
  classvalues <- c(rep(0, size))
  for (i in 1:size) {
    class <- which.max(modeled_output[i, ])
    if (modeled_output[i, class] > 0) {
      classvalues[i] <- class
    } else {
      classvalues[i] = NA
    }
  }
  return(classvalues)
}

apply_nn_to_training_data <- function () {
  modeled_output <- predict(nn, training.data)
  classvalues <- class_matrix_to_vector (modeled_output)
  
  confusion <- table(training.class, classvalues)
  training.accuracy <- sum(diag(confusion)) / training.size
  cat("training data\n confusion matrix")
  print(confusion)
  cat("training accuracy = ", training.accuracy,"\n")
}

apply_nn_to_test_data <- function () {
  # Assign x1,x2... to the columns of the samples matrix
  # and append it to the training.data data.frame()
  modeled_output <- predict(nn, test.data)
  classvalues <- class_matrix_to_vector (modeled_output)
  confusion <- table(test.class, classvalues)
  cat("\n\ntest data\n confusion matrix")
  print(confusion)
  test.accuracy <- sum(diag(confusion)) / test.size
  cat("\n test accuracy = ", test.accuracy,"\n")
}

graph_iteration_error <- function () {
  #pdf('convergence_5-5.pdf')
  plotIterativeError(nn,log='xy')
  grid()
  #dev.off()
  }

plot_neural_net <- function (net) {
  pdf('net.pdf')
  library(NeuralNetTools)
  plotnet(net)
  dev.off()
  }


for (k in 1:ntrials)
{
  
  nn <- mlp(
    training.data,
    t(class_matrix),
    size = hidden,
#   linOut = TRUE,
    learnFunc = learningfunc,
    learnFuncParams = c(0.05, 0.0),
    maxit = iterations
  )
  
  cat(k,'  ',
    "Fit error reduced from ",
    nn$IterativeFitError[1],
    " to ",
    nn$IterativeFitError[length(nn$IterativeFitError)],
    " in ",
    length(nn$IterativeFitError),
    " iterations \n"
  )
  
  graph_iteration_error()
  apply_nn_to_training_data()
  apply_nn_to_test_data()
  #plot_neural_net (nn) 
}
