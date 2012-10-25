RSNNS
=====

RSNNS: Neural Networks in R using the Stuttgart Neural Network Simulator (SNNS)


TODOs for the next version:

- Remove printf throughout the code (replace with Rprintf)
- use the "colorspace" package for heatmaps
- add JSS paper as vignette
- elman: shufflePatterns should be FALSE by default
- switch to roxygen2
- bug: in regression plot, the axis labels are inverted
- mention JavaNNS in documentation
- change in demos using iris the shuffling to something like: 
  irisShuffled <- iris[sample(nrow(iris)),]


TODOs for a far away future:

- Implement Softmax/Entropy for elman?
- Make it possible to implement learning functions in R
