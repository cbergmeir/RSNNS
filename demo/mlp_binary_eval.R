# mlp_binary_eval.R

# Apply mlp to binary categorical features. We investigate
# the effect of dimension and models for the categorical
# features and determine accuracy as a function of training
# set size and hidden layer configurations. There are only
# two classes
#
# The models fill up the entire binary feature space.
# For Dim = 12, this amounts to 4096 possible binary
# feature vectors. For complex binary models, it is
# recommended that you do not exceed Dim = 12; otherwise
# you may need a very large training set to sample
# the entire binary space.
#
# There are 3 possible models you can experiment with:
# random_tree_data(), make_random_binary(), and
# make_random_binary2(). See internal documentation
# for details. Choose one of the models by shifting
# the uncommented function call in the middle of
# this source code.


nclasses <- 2 # Number of classes. Do not change !

# Here are the variables you can experiment with.
Dim <- 14 # Dimension of parameter space
training.size <- 1000 # Number of training samples
test.size <- 5000 # Number of test samples
#hidden <- c(10,3)
hidden <- 7 
ntrials <- 3
learningfunc <- 'SCG'
iterations <- 1000

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

# comment next line if you random results each time you run
set.seed(123456)

data.size <- training.size + test.size
data.class <- c()


# Converts an integer into a vector of binary bits, nbits long.
# The number should be less than 32 bits.
number2binary = function(number, noBits) {
       binary_vector = rev(as.numeric(intToBits(number)))
       if(missing(noBits)) {
          return(binary_vector)
       } else {
          binary_vector[-(1:(length(binary_vector) - noBits))]
       }
    }


# create random binary vectors of length Dim and
# classify them according to this rule. Concatenating
# the Dim bits into an integer s, classify the vector
# as class 2 if s %% jump == 0 otherwise as class 1.
# For example if jump = 2, then the least significant
# bit of s determines whether s is class 1 or 2. If
# jump = 3, the classification is very complex.
make_random_binary <- function(jump) {
for (i in 1:data.size) {
 s <- sample(0:2^Dim,1,replace=TRUE)
 binv <- number2binary(s,Dim)
 if (i == 1) {
   data.samples <<- matrix(t(binv),ncol=Dim)
   } else {
   data.samples <<- rbind(data.samples,binv)
   }
 if ((s %% jump) == 0) {
   classid <- 2
   } else {
   classid <- 1
   }
 data.class <<- c(data.class,classid)
 } 
}


# create random binary vectors of length Dim and
# classify them according to this rule. Sum the
# first splits binary features and set it to s.
# If s %% 2 == 0 then class 2, otherwise class 1.
# For example, if splits is 3 then
# (0, 0, 0, ... ) --> 2
# (1, 0, 0, ... ) --> 1
# (1, 1, 0, ... ) --> 2
# etc.
make_random_binary2 <- function(splits) {
for (i in 1:data.size) {
 s <- sample(0:2^Dim,1,replace=TRUE)
 binv <- number2binary(s,Dim)
 if (i == 1) {
   data.samples <<- matrix(t(binv),ncol=Dim)
   } else {
   data.samples <<- rbind(data.samples,binv)
   }
 s <- sum(binv[1:splits])
 if ((s %% 2) == 0) {
   classid <- 2
   } else {
   classid <- 1
   }
 data.class <<- c(data.class,classid)
 } 
}




# Binary decision tree globals and support functions

features <- seq(1:Dim) 
maxdepth <- 3 # Maximum depth of decision tree. Keep it small.
number_of_nodes <- 1 # initialization



# make_node creates a random binary decision tree up to maxdepth.
# The decision tree is used to classify binary data vectors into
# one of two classes.

make_node <- 
  function(features, depth, tree, number) {
    feature <- sample(features, 1, replace = TRUE)
    features <- features[-feature]
    if (depth < maxdepth) {
      leftnode  <- number_of_nodes + 1 # pointers to next nodes
      rightnode <- number_of_nodes + 2
      node <- c(1,feature,leftnode,rightnode) # create nonterminal node
      tree[[number]] <-  node #insert it in the tree
      depth <- depth + 1
      number_of_nodes <<- number_of_nodes + 2
      tree <- make_node(features, depth, tree, leftnode) #recurse
      tree <- make_node(features, depth, tree, rightnode) #recurse
    } else {
      node <- c(2,feature,1,2) #terminal node
      tree[[number]] <- node
    }
    return (tree)
  }



print_tree <- function(tree) {
  for (i in 1:length(tree)) {
     cat('node ',i,' = ',tree[[i]],'\n')
     }
  }


# classifies a binary vector into one of two classes
# using the random decision_tree.
binary_map <- function (bin) {
  node <- decision_tree[[1]]
  type <- node[1]
  while (type != 2) {
    f <- node[2]
    type <- node[1]
    if (type == 1) {
      if (bin[f] == 0) {
        following_node <- node[3]
      } else {
        following_node <- node[4]
      }
    } else {
      if (bin[f] == 0) {
        class <- 1
      } else {
        class <- 2
      }
    }
  node <- decision_tree[[following_node]]
  }
  return(class)
}


random_tree_data <- function() {
decision_tree <<- make_node (features, 1, list(), 1)
print_tree(decision_tree)
data.samples <<- matrix(round(runif(Dim*data.size)),data.size,Dim)
for (i in 1:data.size) {
  data.class[i] <<- binary_map(data.samples[i,])
  }
}


# Try one of these functions to make random binary features
#random_tree_data()
#make_random_binary2(4)
#make_random_binary2(3)
make_random_binary(3)



classcol <- function (col, dim) {
  # converts a category to a column vector of dimension dim
  m <- matrix(0, dim, 1)
  m[col, 1] <- 1
  m
}



#          Neural Network
#          --------------


cdict <- c('c1', 'c2')

# split the data.samples into training.data and test.data
# split the data.class into training.class and test.class
test.ptr <- training.size + 1
training.data <- data.samples[1:training.size, ]
test.data <- data.samples[test.ptr:data.size,]
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


# converts neural network predicted class matrix to
# a class vector.
class_matrix_to_vector <- function (modeled_output) {
  size <- nrow(modeled_output)
  classvalues <- c(rep(0, size))
  for (i in 1:size) {
    class <- which.max(modeled_output[i, ])
    classvalues[i] <- class
  }
  return(classvalues)
}


# applies the neural network model to the training data
# and determines its accuracy.
apply_nn_to_training_data <- function () {
  modeled_output <- predict(nn, training.data)
  classvalues <- class_matrix_to_vector (modeled_output)
  
  confusion <- table(training.class, classvalues)
  training.accuracy <- sum(diag(confusion)) / training.size
  cat("training data\n confusion matrix")
  print(confusion)
  cat("training accuracy = ", training.accuracy,"\n")
}

# applies the neural network model to the test data
# and determines its accuracy.
apply_nn_to_test_data <- function () {
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
  #pdf('net.pdf')
  library(NeuralNetTools)
  plotnet(net)
  #dev.off()
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
  par(ask=TRUE)
  #plot_neural_net (nn) 
}
