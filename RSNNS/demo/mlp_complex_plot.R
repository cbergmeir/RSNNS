# mlp_complex_plot.R

# applies the RSNNS neuralnet package to samples
# in a 2D space which are grouped into different Voronoi
# regions. The classes are no longer convex or simply-
# connected. It is recommended that you initially keep
# Dim to 2 so you can see what is going on.
#
# Please allow a minute or so for the computer
# to do the computation.

Dim <- 2 # Dimension of parameter space
nregions <- 15 # Number of regions
nclasses <- 3 # Number of classes
nsamples <- 1000 # Number of samples
hidden <- c(25)
learningfunc <- 'SCG'
iterations <- 2000

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
centers <- matrix(runif(Dim * nregions, 0.05, 0.95), nrow = Dim)
class.numbers <- sample(1:nclasses,nregions,replace=TRUE)
samples <- matrix(runif(Dim * nsamples, 0.0, 1), nrow = Dim)

euclid_distance <- function (x1, x2) {
  return (sqrt(sum((x1 - x2) ^ 2)))
}

nearest_center <- function (x) {
  d <- c()
  for (i in 1:nregions) {
    d <- c(d, euclid_distance (x, centers[, i]))
  }
  return (which.min(d))
}

# Assign sample to the Voronoi region it belongs to.
# sample.class is a vector containing the class number
# of each sample.
sample.class <- c()
for (j in 1:nsamples) {
  region <- nearest_center(samples[, j])
  class_id <- class.numbers[region]
  sample.class <- c(sample.class, class_id)
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



#pdf('bigvoronoi.pdf')
# Plot training data on left
par(pch = 19)
par(cex.lab=1.3)
par(cex.axis=1.3)
plot(x1, x2, col = sample.class)
#dev.off()


#          Neural Network
#          --------------


xdict <- paste0("x",1:6)
# equivalent to xdict <- c('x1', 'x2', 'x3', 'x4', 'x5', 'x6')
cdict <- paste0("c",1:16)

# Create formula for neural net algorithm
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

test.data <- data.frame(expand.grid(seq(0, 1, 0.025), seq(0, 1, 0.025)))

modeled_output <- predict(nn, test.data)

y1 <- test.data[[1]]
y2 <- test.data[[2]]

#modeled_output <- round(modeled_output)
nsamples <- nrow(modeled_output)

classvalues <- c(rep(0, nsamples))
for (i in 1:nsamples) {
  class <- which.max(modeled_output[i, ])
  classvalues[i] <- class
}

#pdf('voronoicmplxfit.pdf')
par(cex.lab=1.3)
par(cex.axis=1.3)
par(pch = 19)
plot(y1, y2, col = classvalues)
#dev.off()
