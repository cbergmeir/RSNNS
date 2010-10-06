library(RSNNS)

basePath <- ("/home/bergmeir/")
examplesPath <- ("/home/bergmeir/Download/SNNSv4.3/examples/")

files <- c(
    "art1_letters.pat", 
    "artmap_test.pat", 
    "eight_016.pat", 
    "laser_999.pat", 
    "letters_with_classes.pat", 
    "spirals.pat", 
    "trainValidMAP.pat", 
    "xor_rec1.pat",
    "art2_tetra_high.pat", 
    "artmap_train.pat", 
    "eight_160.pat", 
    "letseq_test.pat", 
    "nettalk.pat",
    "sprach_test.pat",
    "validMAP.pat", 
    "xor_rec2.pat", 
    "art2_tetra_low.pat",
    "bdg_TDNN.pat",
    "encoder.pat",
    "letseq_train.pat",
    "patmat.pat",
    "sprach_train.pat",
    #"watch_2out.pat", 
    "art2_tetra_med.pat", 
    #"bdg_variable.pat", 
    "font.pat", 
    "letters_auto.pat", 
    "som_cube_norm.pat", 
    "testMAP.pat",
    #"watch.pat", 
    "art2_tetra.pat", 
    "dlvq_ziff_100.pat", 
    "laser_1000.pat", 
    "letters.pat", 
    "som_cube.pat", 
    "trainMAP.pat", 
    "xor.pat")

#file <- "dlvq_ziff_100.pat"

#files <- c("art1_letters.pat", "eight_016.pat", "spirals.pat", "som_cube.pat", "xor.pat")

snnsData <- list()

for (file in files)  {
  print(file)
  snnsData[[file]] <- readPatFile(paste(examplesPath,file,sep=""))  
}

#snnsData

save(snnsData, file = paste(basePath,"snnsData.RData",sep=""))

#data(snnsData)
#snnsData$eight_016.pat
