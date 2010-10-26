library(RSNNS)

data(snnsData)

patterns <- snnsData$art1_letters.pat

model <- assoz(patterns, dimX=7, dimY=5)

#model$fitted.values

par(mfrow=c(3,3))
for (i in 1:9) image(rot90(model$fitted.values[[i]]))

#predict(model, patterns)