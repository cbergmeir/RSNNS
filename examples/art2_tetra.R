library(RSNNS)

data(snnsData)
patterns <- snnsData$art2_tetra.pat

model <- art2(patterns, f2Units=5)

model$fitted.values
model

testPatterns <- snnsData$art2_tetra_med.pat

predictions <- predict(model, testPatterns)

library(scatterplot3d)

par(mfrow=c(1,2))
scatterplot3d(model$fitted.values[,1:3])
scatterplot3d(predictions[,1:3])
