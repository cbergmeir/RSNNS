#library(RSNNS)
#
#res <- demo(package="RSNNS")$results
#
#allDemos <- res[, which(colnames(res)=="Item")]
#
#allDemos <- allDemos[-which(allDemos=="mlp_plot_with_my_predict")]
#
#for(currDemo in allDemos) {
#  print(paste("Running ", currDemo, "\n",sep=""))
#  demo(currDemo, character.only=TRUE, ask=FALSE)
#}
