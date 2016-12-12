# mlp_plot_with_my_predict.R

# applies the RSNNS neuralnet package to samples
# in a 2D space which are grouped into Voronoi
# regions and plots the resulting mlp model.
# This version illustrates how to include the
# code mlp_my_predict.R. This code allows you
# to view the activation response of particular
# nodes or to disable particular nodes in the
# neural network.

# Changing Dim or hidden variables may make
# this example non functional unless you also
# make corresponding changes to the function
# make_activation_plot. (The node numbers of
# the hidden nodes may change.)

Dim <- 2 # Dimension of parameter space
nclasses <- 5 # Number of classes
nsamples <- 500 # Number of samples
learningfunc <- 'SCG'
iterations <- 1000
hidden <- c(3)

if (nclasses > 12) {
  stop("too many classes")
}
if (Dim > 6) {
  stop("dimension two high")
}

set.seed(123456)


#              Create Training Data
#              --------------------

# We create samples in a rectangular space
# 0.0 to 1.0 in all dimensions. The samples
# are divided into perfectly separated nclasses
# depending on which # voronoi region they occur.

# centers are the centroids of the voronoi regions
centers <- matrix(runif(Dim * nclasses, 0.05, 0.95), nrow = Dim)
samples <- matrix(runif(Dim * nsamples, 0.0, 1), nrow = Dim)

euclid_distance <- function (x1, x2) {
  return (sqrt(sum((x1 - x2) ^ 2)))
  #return (sum(abs(x1-x2)))
  #return(max(abs(x1-x2)))
}

nearest_center <- function (x) {
  d <- c()
  for (i in 1:nclasses) {
    d <- c(d, euclid_distance (x, centers[, i]))
  }
  return (which.min(d))
}

# Assign sample to the Voronoi region it belongs to.
# sample.class is a vector containing the class number
# of each sample.
sample.class <- c()
for (j in 1:nsamples) {
  voronoi.index <- nearest_center(samples[, j])
  sample.class <- c(sample.class, voronoi.index)
}

x1 <- samples[1, ]
x2 <- samples[2, ]
#training.data <- data.frame(x1,x2,sample.class)
training.data <- as.matrix(data.frame(x1, x2))


classcol <- function (col, dim) {
  # converts a category to a column vector of dimension dim
  m <- matrix(0, dim, 1)
  m[col, 1] <- 1
  m
}


# convert each sample.class to a column vector of length nclasses
# where all entries are zero except for the one at column
# sample.class. Join all the column vectors (for each sample)
# into a matrix of size nsamples by nclasses.
class_matrix <- sapply(sample.class, classcol, nclasses)



#pdf('voronoi2.pdf')
# Plot training data on left
par(pch = 19)
par(cex.lab=1.3)
par(cex.axis=1.3)
#par(mfrow = c(1, 2))
#library(ggplot2)
plot(x1, x2, col = sample.class)
#qplot(x1,x2,col=factor(sample.class),asp=1)
#dev.off()


#          Neural Network
#          --------------


# Create formula for neural net algorithm
#xdict <- c('x1', 'x2', 'x3', 'x4', 'x5', 'x6')
xdict <- paste0("x",1:16)
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
f1 <- paste(cdict[1:nclasses], collapse = '+')
f2 <- paste(xdict[1:Dim], collapse = '+')
f <- paste(f1, f2, sep = ' ~ ', collapse = NULL)
formula <- as.formula(f)
print(formula)


# Assign variables c1,c2, and etc. to the nrows of the class_matrix.
# c1[i] = 1 if sample i is assigned to class 1 otherwise 0.
# c2[i] = 1 if sample i is assigned to class 2 otherwise 0.
# and etc.
for (i in c(1:nclasses)) {
  assign(cdict[i], class_matrix[i, ])
}

library(Rcpp)
library(RSNNS)

for (j in 1:1) {
nn <-
  mlp(
    training.data,
    t(class_matrix),
    size = hidden,
#    linOut = TRUE,
    learnFunc = learningfunc,
    learnFuncParams = c(0.05, 0.0),
    maxit = iterations
  )

 
  cat(
    "Fit error reduced from ",
    nn$IterativeFitError[1],
    " to ",
    nn$IterativeFitError[length(nn$IterativeFitError)],
    " in ",
    length(nn$IterativeFitError),
    " iterations \n"
  )
}
#           Apply Neural Network on Test Data
#           ---------------------------------

# Create a two dimensional grid of test.data so we can plot the
# results.
test.data <- data.frame(expand.grid(seq(0, 1, 0.025), seq(0, 1, 0.025)))


# My own predict function (simulated in R code) is used instead
# so I have more options.
#modeled_output <- predict(nn, test.data)

setwd("C:/Users/User/Documents/r_work/")
# The standard source() function does not work because
# we also need to import the global variables that were
# set in the module mlp_my_predict.R. Though the use
# of global variables is somewhat discouraged, it makes
# it easier to save state variables set by other functions
# and makes it easier to see what is happening with the
# R interpreter when I am debugging.
sys.source ("mlp_my_predict.R",envir=globalenv())
setup_my_predict()
modeled_output <- my_predict(test.data,0)

y1 <- test.data[[1]]
y2 <- test.data[[2]]

nsamples <- nrow(modeled_output)

classvalues <- c(rep(0, nsamples))

model_to_class <- function(nsamples,modeled_output) {
for (i in 1:nsamples) {
  class <- which.max(modeled_output[i, ])
  classvalues[i] <- class
}
return (classvalues)
}

classvalues <- model_to_class(nsamples,modeled_output)

#pdf('voronoi2b.pdf')
par(cex.lab=1.3)
par(cex.axis=1.3)
#qplot(y1, y2, col = factor(classvalues),asp=1)
par(pch = 19)
plot(y1, y2, col = classvalues)
#dev.off()

make_activation_plots <- function() {
#pdf('activations.pdf',height=2.5)
activation <- my_predict_activation (test.data)
par(mfrow = c(1,3))
plot_activation (y1,y2,activation,3)
plot_activation (y1,y2,activation,4)
plot_activation (y1,y2,activation,5)
#dev.off()
#print("Activation plots can be found in activations.pdf file")
}

# This function illustrates how to view the effect of disabling
# a particular node in the neural network. The second parameter
# of my_predict() is the node number or list of node numbers
# to disable when running the predict function. The results
# are recorded in the pdf file called broken.pdf.
make_broken_net <- function () {
  #pdf('broken.pdf',height=2.5)
  par(mfrow = c(1,3))
  modeled_output <- my_predict(test.data,3)
  classvalues    <- model_to_class(nsamples,modeled_output)
  plot(y1, y2, col = classvalues,xlab='x',ylab='y')
  modeled_output <- my_predict(test.data,4)
  classvalues    <- model_to_class(nsamples,modeled_output)
  plot(y1, y2, col = classvalues,xlab='x',ylab='y')
  modeled_output <- my_predict(test.data,5)
  classvalues    <- model_to_class(nsamples,modeled_output)
  plot(y1, y2, col = classvalues,xlab='x',ylab='y')
  #dev.off()
  #print("resulting plots can be found in broken.pdf file")
  }

make_broken_net()
par(ask=TRUE)

make_activation_plots()

