library(RSNNS)

data(snnsData)

patterns <- snnsData$art1_letters.pat

model <- art1(patterns, dimX=7, dimY=5)
#updateFunc="ART1_Synchronous"

model$fitted.values

par(mfrow=c(3,3))
for (i in 1:9) plotActMap(model$fitted.values[[i]])

#summary(model)
