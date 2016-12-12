# mlp_1dim_eval.R

# This trivial model demonstrates that even a
# a two class problem in one dimensional space
# may require a few hidden nodes to model the
# data. Another advantage of this model, is
# the arithmetic of following the operation
# of the feedforward network can be done with a calculator,
# once you have the weights and thresholds.
#
# Though the data is one dimensional,
# we plot it in two dimensional space (where
# y is random noise) for appearance sake only.

# do not change these variables.
Dim <- 1 # Dimension of parameter space
nclasses <- 2 # Number of classes


# You can change these variables
training.size <- 500 # Number of training samples
test.size <- 5000 # Number of test samples
hidden <- 5 # We need no less than 5 hidden nodes.
#learningfunc <- 'Rprop'
learningfunc <- 'SCG'
#learningfunc <- 'Std_Backpropagation'
iterations <- 2000

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

set.seed(123456)
data.size <- training.size + test.size
x <- runif(data.size,0,1)
y <- runif(data.size,0,1)
data.samples <- matrix(x, nrow =1)


dataset_2_k <- function (k) {
data.class <- c()
for (i in 1:data.size) {
 s <- data.samples[i]
 n <- floor(s*k)
 if ((n %% 2) == 1) {
   classid <- 2
   } else {
  classid <- 1
   }
 data.class <- c(data.class,classid)
} 
 return (data.class)
}

dataset_2_bin <- function (n) {
data.class <- c()
n <- n %% 256 # restrict to 8 bits
mapper <- as.integer(intToBits(n))
mapper <- rev(mapper[1:8])
print(mapper)
for (i in 1:data.size) {
 s <- data.samples[i]
 m <- floor(s*8) + 1
 classid <- mapper[m] + 1
 data.class <- c(data.class,classid)
 } 
return(data.class)
}


dataset_5_1 <- function () {
data.class <- c()
for (i in 1:data.size) {
 s <- data.samples[i]
 n <- floor(s*5)
 classid <- (n + 1)
 data.class <- c(data.class,classid)
} 
nclasses <<- 5
 return (data.class)
}

#data.class <- dataset_5_1()
#data.class <- dataset_2_k(2)
data.class <- dataset_2_bin(69)

par(mfrow=c(2,2))
#pdf('1dtrain.pdf')
plot(x[1:training.size],y[1:training.size],col=data.class,xlab='x',
  ylab='y')
#dev.off()

class_matrix_to_vector <- function (modeled_output) {
  size <- nrow(modeled_output)
  classvalues <- c(rep(0, size))
  for (i in 1:size) {
    class <- which.max(modeled_output[i, ])
    classvalues[i] <- class
  }
  return(classvalues)
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
xdict <- c('x1', 'x2', 'x3', 'x4', 'x5', 'x6')
cdict <-
  c('c1',
    'c2',
    'c3',
    'c4',
    'c5',
    'c6',
    'c7',
    'c8',
    'c9',
    'c10',
    'c11',
    'c12')


test.ptr <- training.size + 1
training.data <- data.samples[1:training.size]
test.data <- data.samples[test.ptr:data.size]
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






library(Rcpp)
library(RSNNS)

cat("\n\ntraining size = ", training.size, " samples\n")
cat("number of classes = ", nclasses, "\n")
cat("dimension of space = ", Dim, "\n")
cat("hidden layer(s) = ",hidden,'\n')
cat("learning function = ",learningfunc,'\n')


apply_nn_to_training_data <- function () {
  training.data <- as.matrix(training.data)
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
  test.data <- as.matrix(test.data)
  modeled_output <- predict(nn, test.data)
  classvalues <- class_matrix_to_vector (modeled_output)
  confusion <- table(test.class, classvalues)
  cat("\n\ntest data\n confusion matrix")
  print(confusion)
  test.accuracy <- sum(diag(confusion)) / test.size
  cat("\n test accuracy = ", test.accuracy,"\n")
#  pdf('1dtest.pdf')
  plot(x[test.ptr:data.size],y[test.ptr:data.size],col=classvalues,
   xlab='x',ylab='y')
#  dev.off()
}

graph_iteration_error <- function () {
  #pdf('convergence_5-5.pdf')
  plotIterativeError(nn,log='xy')
  grid()
  #dev.off()
  }

plot_neural_net <- function (net) {
  #pdf('net.pdf')
  library(NeuralNetTools)
  plotnet(net)
  #dev.off()
  }

error_scg <- c()
for (k in 1:1)
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
  plot_neural_net (nn) 

  cat("\n    Network Description \n\n")
  print(extractNetInfo(nn))
}
