# mlp_plot.R

# Produces plots either for a report or display of the
# training set and the test set (arranged along a 2D grid).

# You can increase Dim above 2, but only the 2D projection
# will be plotted.
Dim <- 2 # Dimension of parameter space
nclasses <- 5 # Number of classes
nsamples <- 500 # Number of samples
learningfunc <- 'SCG'
iterations <- 1000
hidden <- c(3)

if (nclasses > 16) {
  stop("too many classes")
}
if (Dim > 16) {
  stop("dimension two high")
}


cat("Reference: 'Experiments with Neural Networks using R' by Seymour Shlien\n")
cat("To change the model parameters (number of classes, training size, test size...)\nyou need to edit this file which you can download from the following\nsite")
cat(" http://ifdo.ca/~seymour/R\n\n")

# If you want a random plot each time, comment out the
# next line.
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


# Create the formula for neural net algorithm
# If you need more than 16 dimensions or classes, you should
# extend xdict or cdict.
xdict <- paste0("x",1:16)
cdict <- paste0("c",1:16)
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

test.data <- data.frame(expand.grid(seq(0, 1, 0.025), seq(0, 1, 0.025)))


modeled_output <- predict(nn, test.data)

y1 <- test.data[[1]]
y2 <- test.data[[2]]


# The output of predict is a matrix of class vectors.
# To convert each class vector to a class number we use
# this function.
model_to_class <- function(nsamples,modeled_output) {
for (i in 1:nsamples) {
  class <- which.max(modeled_output[i, ])
  classvalues[i] <- class
}
return (classvalues)
}

nsamples <- nrow(modeled_output)
classvalues <- c(rep(0, nsamples))
classvalues <- model_to_class(nsamples,modeled_output)

#pdf('voronoi2b.pdf')
par(cex.lab=1.3)
par(cex.axis=1.3)
par(pch = 19)
plot(y1, y2, col = classvalues)
#dev.off()

