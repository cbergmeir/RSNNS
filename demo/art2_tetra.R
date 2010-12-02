library(RSNNS)

data(snnsData)
patterns <- snnsData$art2_tetra_med.pat

model <- art2(patterns, f2Units=5, learnFuncParams=c(0.99, 20, 20, 0.1, 0), updateFuncParams=c(0.99, 20, 20, 0.1, 0))

model

testPatterns <- snnsData$art2_tetra_med.pat

predictions <- predict(model, testPatterns)

library(scatterplot3d)

par(mfrow=c(2,2))
scatterplot3d(model$fitted.values[,1:3])#, xlim=c(-0.5,1.5), ylim=c(-0.5,1.5), zlim=c(-0.5,1.5))
#scatterplot3d(predictions[,1:3], xlim=c(-0.5,1.5), ylim=c(-0.5,1.5), zlim=c(-0.5,1.5))


scatterplot3d(patterns)#, xlim=c(-0.5,1.5), ylim=c(-0.5,1.5), zlim=c(-0.5,1.5))
#scatterplot3d(testPatterns)#, xlim=c(-0.5,1.5), ylim=c(-0.5,1.5), zlim=c(-0.5,1.5))