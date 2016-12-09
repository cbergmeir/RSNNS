# mlp_eval6.R

# In this version of rsnns_eval, the 2 dimensional
# space is extended to 6 dimensions by introducing
# four more dimensions each which are related to the
# initial dimensions by some transformation. For example
# you can chose one of these by moving the comment symbol
# '#'
# x3 <- x1*x1 or sqrt(x1) or sin(x1)
# x4 <- x2*x2 or sqrt(x2) or cos(x2*pi/2)
# x5 <- x1*x2
# x6 <- (x1+x2)/2
# in the code following the line, 'Assign x1,x2.... to '
# somewhere in the middle of this file.
# In addition, you can substitute uniform white noise
# runif(length(x1),0,1) for any of these parameters x3,x4...
#
# You should find that this does not have much impact
# on how many more hidden nodes we need because
# the input data is basically still two dimensional.


# Do not change this value
Dim <- 2 # Dimension of parameter space

# You can change these variables
nclasses <- 5 # Number of classes
training.size <- 500 # Number of training samples
test.size <- 5000 # Number of test samples
hidden <- c(5)
iterations <- 1000
#learningfunc <- 'Rprop'
#learningfunc <- 'Std_Backpropagation'
learningfunc <- 'SCG'
ntrials <- 3 # Number of times to run mlp and predict on same data

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
if (nclasses > 12) {
  stop("too many classes")
}
if (Dim > 6) {
  stop("dimension two high")
}

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

# Assign sample to the Voronoi region it belongs to.
# training.class is a vector containing the class number
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




plot_voronoi_data <- function() {
# Plot training data on left
#pdf('voronoi3.pdf',height=4, width=4)
par(pch = 19)
#par(mfrow=c(1,2))
par(pty = 's')
#par(fin = c(7, 7))
par(cex.lab = 1.5)
par(cex.axis = 1.5)
# choose any two variables (out of x1,x2,...x6) you want to plot.
plot(x3[1:training.size],
     x4[1:training.size],
     col = training.class,
     asp = 1,
     ylim = c(0, 1))
#dev.off()
}


# Assign x1,x2... to the columns of the samples matrix
# and append it to the training.data data.frame()
x1 <- data.samples[1,]
x2 <- data.samples[2,]
x3 <- x1 * x1
#x3 <- sqrt(x1)
#x3 <- sin(x1)
#x3 <- runif(length(x1),0, 1)
#x4 <- x2 * x2
#x4 <- sqrt(x2)
x4  <- cos(x2*pi/2)
#x4 <- runif(length(x1),0, 1)
#x5 <- x1 * x2
x5 <- runif(length(x1),0, 1)
#x5 <- rnorm(length(x1),0.5,0.2)
#x6 <- (x1 + x2)/2.0
x6 <- runif(length(x1),0, 1)
data <- data.frame(x1,x2,x3,x4,x5,x6)
#data <- data.frame(x1,x2,x3,x4,x5)



test.ptr <- training.size+1
training.data <- data[1:training.size, ]
test.data <- data[test.ptr:data.size, ]
training.class <- data.class[1:training.size]
test.class <- data.class[test.ptr:data.size]

# convert each training.class to a column vector of length nclasses
# where all entries are zero except for the one at column
# training.class. Join all the column vectors (for each sample)
# into a matrix of size training.size by nclasses.
class_matrix <- sapply(training.class, classcol, nclasses)


# Create formula for neural net algorithm
xdict <- paste0("x",1:6)
# same as xdict <- c('x1', 'x2', 'x3', 'x4', 'x5', 'x6')
cdict <- paste0("c",1:16)

f1 <- paste(cdict[1:nclasses], collapse = '+')
#f2 <- paste(xdict[1:6], collapse = '+')
f2 <- paste(colnames(data), collapse = '+')

f <- paste(f1, f2, sep = ' ~ ', collapse = NULL)
formula <- as.formula(f)
print(formula)
plot_voronoi_data()

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
cat("dimension of space = ", dim(training.data)[2], "\n")

apply_nn_to_training_data <- function () {
  modeled_output <- predict(nn, training.data)
  modeled_output <- round(modeled_output)
  classvalues <- class_matrix_to_vector (modeled_output)
  
  confusion <- table(training.class, classvalues)
  training.accuracy <- sum(diag(confusion)) / training.size
  training.accuracy <- sum(diag(confusion)) / training.size
  cat("training data\n confusion matrix")
  print(confusion)
  cat("training accuracy = ", training.accuracy,"\n")
}

apply_nn_to_test_data <- function () {
  # Assign x1,x2... to the columns of the samples matrix
  # and append it to the training.data data.frame()
  modeled_output <- predict(nn, test.data)
  modeled_output <- round(modeled_output)
  classvalues <- class_matrix_to_vector (modeled_output)
  confusion <- table(test.class, classvalues)
  cat("\n\ntest data\n confusion matrix")
  print(confusion)
  test.accuracy <- sum(diag(confusion)) / test.size
  cat("\n test accuracy = ", test.accuracy,"\n")
}

graph_iteration_error <- function () {
  #pdf('convergence.pdf')
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

#error_scg <- c()
#for (k in 1:length(hidden.selection))

for (k in 1:ntrials)
{
  cat('\n\ntrial ',k,' out of ',ntrials,'\n') 
  
  nn <- mlp(
    training.data,
    t(class_matrix),
    size = hidden,
#    linOut = TRUE,
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
  #error_scg[k] <-   nn$IterativeFitError[length(nn$IterativeFitError)]
}
#save('error_scg',file='mlp_scg.Rdata')
